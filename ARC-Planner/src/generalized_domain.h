#ifndef __GENERALIZED_DOMAIN_H__
#define __GENERALIZED_DOMAIN_H__
#include "common.h"
#include "state_descriptor.h"
#include "action.h"
#include "domain.h"
#include "condition.h"
#include "instruction.h"
#include "parser.h"


class GeneralizedDomain{
public:
	// Instructions can be created only once but we keep the logic
	// of which horizontal_action can be programmed in each line here, so the
	// engine does not have to reason about it
	GeneralizedDomain( Domain *d, int program_lines, vector<string> pruned_action ){


		StateDescriptor *sd = d->getStateDescriptor();
	
		_instructions_line.resize( program_lines );


        auto action_schemas=d->getActionsSchema();
        for (auto  action_schema : action_schemas){
            if (std::find(pruned_action.begin(), pruned_action.end(),action_schema->getName())!=pruned_action.end())
                continue;
            vector< string > grounded_params;
            auto param_type_list=action_schema->getParamTypeList();
            auto act_name=action_schema->getName();
            auto act_type=action_schema->getType();
            auto conditions=action_schema->getActionPreconditionsStructure();
            auto effects=action_schema->getActionEffectStructure();

            auto act_param_type=action_schema->getActionParaType();

            auto act_param_idx=action_schema->getActionParaIndex();
            // partial instantiation introduced in the paper Generalized Planning for the Abstraction and Reasoning Corpus
            sd->computeGroundedObjectIDs(param_type_list,grounded_params);


            //partial instantiated  actions
            for( const auto& gp : grounded_params ){
                vector< string > pointer_names = Parser::split( gp );
                string grounded_name = act_name + "(" + gp + ")";

                // Only create grounded horizontal_action
                d->addAction( new Action( grounded_name, act_type ) );
                Action *act = d->getAction( grounded_name );
                act->setActName(act_name);
                act->setPlanningAction(true);
                act->set_external_function(action_schema->checkExternalFunction());

                vector< int > param_ids;
                for (auto pointer: pointer_names){
                    int typeID=sd->getTypeID(pointer);
                    param_ids.push_back(typeID);
                }
                Variable *external = new Variable( "external-variable", "external", 0, param_ids  );
                Operation *oper = Parser::readOperation( sd, "@=", external, external);
                act->addExteralFunction(oper);
                act->setActionParaIndexVector(param_ids);

                if (!conditions.empty()){
                        vector<string> condWord;

                        for( auto c : conditions ){
                            Variable *lhs = Parser::groundingVar_object( sd, c[0], act_param_idx, pointer_names );
                            Variable *rhs = Parser::groundingVar_object( sd, c[2], act_param_idx, pointer_names );
                            Condition *cond = Parser::readCondition( sd, c[1], lhs, rhs );
                            act->addCondition( cond );
                            if (rhs->getName()=="1"){
                                string name=lhs->getName();
                                condWord.push_back(name.substr(0,name.find("(")));
                            }
                            if (rhs->getName()=="0"){
                                string name=lhs->getName();
                                condWord.push_back("not-"+name.substr(0,name.find("(")));
                            }
                        }
                        act->setActionPreWord(condWord);
                    }

                    if (!effects.empty()){
                        vector<string> effectWord;

                        for( auto eff : effects ){
                            Variable *lhs = Parser::groundingVar_object( sd, eff[0], act_param_idx, pointer_names );
                            Variable *rhs = Parser::groundingVar_object( sd, eff[2], act_param_idx, pointer_names );
                            Operation *oper = Parser::readOperation( sd, eff[1], lhs, rhs );
                            act->addOperation( oper );
                            if (rhs->getName()=="1"){
                                string name=lhs->getName();
                                effectWord.push_back(name.substr(0,name.find("(")));
                            }
                            if (rhs->getName()=="0"){
                                string name=lhs->getName();
                                effectWord.push_back("not-"+name.substr(0,name.find("(")));
                            }
                        }

                        act->setActionEffectWord(effectWord);

                    }

            }

            // when the pointer number is smaller than the object type number and the object type has to
            // be pointed by a pointer, we set it as the Uninstantiated Action and it will not be programmed

            if (grounded_params.size()==0){
                d->addAction( new Action( act_name, act_type ) );
                Action *act = d->getAction( act_name );
                act->setActName(act_name);
                act->setUninstantiatedAction(true);
                act->setPlanningAction(true);

            }

        }
		// Planning Actions
		auto actions = d->getActions();
		// [2] Symmetry Breaking - Only End can be programmed in last line
		for( int i = 0; i + 1 < program_lines; i++ ){
			for(auto & action : actions){
				_instructions_line[ i ].push_back( new PlanningAction( action ) );
			}
		}
        for( int j = 0; j < int( actions.size() ); j++ ){
            _plan_actions.push_back( actions[ j ]  );
        }
		
		// Extending planning horizontal_action with pointer instructions
        auto v_pointers = sd->getPointerNames();
        // [2] Symmetry Breaking - extra horizontal_action cannot be programmed in the last two lines
        for( int line = 0; line + 2 < program_lines; line++ ){
            for( const auto& p : v_pointers ){
                // zf and cf flags are indirectly modified by other pointer math updates
                if( p == "zf" or p == "cf" )
                    continue;

                //1. inc(pointer). Increase a pointer
                    auto *inc_act = new Action("inc(" + p + ")");

                    Condition *inc_cond = new Add(sd,
                                                  new Variable(p, "pointer", sd->getTypeID(p)),
                                                  new Variable("1", "constant", 1));
                    inc_act->addCondition(inc_cond);

                    Operation *add_assign_op = new AddAssign(sd,
                                                             new Variable(p, "pointer", sd->getTypeID(p)),
                                                             new Variable("1", "constant", 1));
                    inc_act->addOperation(add_assign_op);

                    _extra_actions.push_back(inc_act);
                    Instruction *inc_ins = new PlanningAction(inc_act);
                    _instructions_line[line].push_back(inc_ins);
                    inc_act->setPointerOperation(true);
                //2. dec(pointer). Decrease a pointer
                if (line!=0 and FIRST ) {
                    auto *dec_act = new Action("dec(" + p + ")");

                    Condition *dec_cond = new Subtract(sd,
                                                       new Variable(p, "pointer", sd->getTypeID(p)),
                                                       new Variable("1", "constant", 1));
                    dec_act->addCondition(dec_cond);

                    Operation *sub_assign_op = new SubtractAssign(sd,
                                                                  new Variable(p, "pointer", sd->getTypeID(p)),
                                                                  new Variable("1", "constant", 1));
                    dec_act->addOperation(sub_assign_op);

                    _extra_actions.push_back(dec_act);
                    Instruction *dec_ins = new PlanningAction(dec_act);
                    _instructions_line[line].push_back(dec_ins);
                    dec_act->setPointerOperation(true);
                }


                // New 3. clear a pointer (reset the pointer to the first position)
                if (line!=0 and FIRST) {
                    auto *clear_act = new Action("clear(" + p + ")");
                    // NO CONDITION
                    Operation *clear_op = new Assign(sd, new Variable(p, "pointer", sd->getTypeID(p)),
                                                     new Variable("", "constant", 0));
                    clear_act->addOperation(clear_op);
                    _extra_actions.push_back(clear_act);
                    Instruction *clear_ins = new PlanningAction(clear_act);
                    _instructions_line[line].push_back(clear_ins);
                    clear_act->setPointerOperation(true);
                }

                for( const auto& p2 : v_pointers ){
                    if( p2 == "zf" or p2 == "cf" or p == p2 or
                        sd->getPointerVarType(p) != sd->getPointerVarType(p2) )
                        continue;
                    if (line!=0 and FIRST ) {
                        //5. set(pointer1,pointer2). Assign pointer2 to pointer1
                        auto *set_act = new Action("set(" + p + "," + p2 + ")");
                        //NO CONDS
                        Operation *assign_op = new Assign(sd, new Variable(p, "pointer", sd->getTypeID(p)),
                                                          new Variable(p2, "pointer", sd->getTypeID(p2)));
                        set_act->addOperation(assign_op);

                        _extra_actions.push_back(set_act);
                        Instruction *set_ins = new PlanningAction(set_act);
                        _instructions_line[line].push_back(set_ins);
                        set_act->setPointerOperation(true);
                    }

                    // [3] Symmetry breaking - allow to compare only in one direction
                    if( sd->getTypeID(p) >= sd->getTypeID(p2) )
                        continue;

                }
            }
        }

        // Extending planning horizontal_action with register instructions
        auto pruned_type=d->getPruneType();
        auto v_pred_types = sd->getPredicateTypes();

        // check instruction generation with partial instantiation predicates (check instruction is the same as the test instruction in paper Generalized Planning for the Abstraction and Reasoning Corpus)
        for( const auto& pred_type : v_pred_types ){

            // argument constraint

            if (std::find(pruned_type.begin(), pruned_type.end(),pred_type)!=pruned_type.end())
                continue;

            auto pred_id = sd->getTypeID( pred_type );
            auto param_var_ids = sd->getPredicateVarTypeIDs( pred_type );
            vector< string > param_var_names( param_var_ids.size() );
            for( unsigned i = 0; i < param_var_ids.size(); i++ )
                param_var_names[i] = sd->getVarName( param_var_ids[i] );

            vector< string > object_params;
            vector<vector<int> > param_object_ids = sd->computeGroundedObjectIDs( param_var_names, object_params );
            vector< string > pointer_params;
            vector<vector<int> > param_pointer_ids = sd->computeGroundedParamIDs( param_var_names, pointer_params );

            // Add an empty param when the predicate has no parameters
            if( param_var_ids.empty() ) {
                pointer_params.emplace_back("");
                param_pointer_ids.emplace_back( );
            }

            for(int line = 0; line + 1 < program_lines; line++){
                for (unsigned i = 0; i < object_params.size(); i++ ){
                    auto object1_name = pred_type + "(" + object_params[i] + ")";
                    if( CHECK_ALLOWED ) {

                        auto *chk_act = new Action("check-condition(" + object1_name + ")");

                        // NO CONDS
                        Operation *cmp_op = new Compare(sd, new Variable(object1_name, "predicate", pred_id,
                                                                         param_object_ids[i]),
                                                        new Variable("0", "constant", 0));
                        chk_act->addOperation(cmp_op);
                        chk_act->setActionParaIndexVector(param_object_ids[i]);
                        _extra_actions.push_back(chk_act);
                        Instruction *cmp_ins = new PlanningAction(chk_act);
                        _instructions_line[line].push_back(cmp_ins);
                        chk_act->setCheckAction(true);

                    }

                }
            }


        }

        // dummy check(true) action whose interpretation is always true introduced in the paper Generalized Planning for the Abstraction and Reasoning Corpus

        for(int line = 0; line + 1 < program_lines; line++) {
            auto *chk_act = new Action("check-condition(true)");
            _extra_actions.push_back(chk_act);
            Instruction *cmp_ins = new PlanningAction(chk_act);
            _instructions_line[line].push_back(cmp_ins);
            chk_act->setCheckAction(true);
        }




		// Conditional Gotos
		// Set 4 different conditions ZF, !ZF, CF, !CF
        int num_of_conditions = 4;
        if( CARRY_FLAG ) num_of_conditions = 4;
		_conds.resize( num_of_conditions );
        _conds[ 0 ] = new Equals( sd,
                                  new Variable( "zf", "pointer", sd->getTypeID( "zf" ) ),
                                  new Variable( "1", "constant", 1 ) );
        _conds[ 1 ] = new Equals( sd,
                                  new Variable( "zf", "pointer", sd->getTypeID( "zf" ) ),
                                  new Variable( "0", "constant", 0 ) );

             // ONLY ZERO FLAG
            for( int from = 0; from + 1 < program_lines; from++ ) {
                for (int to = 0; to < program_lines; to++) {
                    // A goto cannot jump over itself or next line
                    if (from == to || from + 1 == to)
                        continue;
                    if( not IFS_ALLOWED and from < to ) continue;
                    for( int cond_id = 0; cond_id < 2; cond_id++ ){
                        Instruction *ins = new Goto( to );
                        ins->addCond( _conds[ cond_id ] );
                        _instructions_line[ from ].push_back( ins );
                    }
                }
            }
		
		// End
		for( int i = 0; i < program_lines; i++ ){
			_instructions_line[ i ].push_back( new End() );
		}

		//cout << toString();
	}
	
	~GeneralizedDomain(){
		for(auto & instructions : _instructions_line){
            for( auto instruction : instructions ){
                delete instruction;
			}
		}
		
		for(auto & cond : _conds){
            delete cond;
		}
		
		for(auto & extra_action : _extra_actions){
			delete extra_action;
		}

        for(auto & plan_actions : _plan_actions){
            delete plan_actions;
        }
         _RelaxedActions.clear();

//        for(auto & possibleActions : _possibleActions){
//            for (auto & pa: possibleActions.second){
//                delete pa;
//            }
//        }

//        _possibleActions.clear();

         pruned_actions.clear();

	}
	
	Instruction* searchInstruction( int program_line, const string &name ){
		if( program_line >= int( _instructions_line.size() ) )
			return nullptr;
		for(auto & instruction : _instructions_line[ program_line ]){
			if(instruction->toString(false ) == (name + "\n") )
				return instruction;
		}
		return nullptr;
	}
	
	vector< vector< Instruction* > > getInstructions(){
		return _instructions_line;
	}
	
	vector< Instruction* > getInstructions( int program_line ){
		if( program_line >= int( _instructions_line.size() ) )
			return {};
		return _instructions_line[ program_line ];
	}
	
	Instruction* getInstruction( int program_line, int id ){
		return _instructions_line[ program_line ][ id ];
	}
    Action*  getPlanaction (int program_line ){
        if( program_line >= int( _plan_actions.size() ) )
            return NULL;
        return _plan_actions[program_line];
    }

    vector<struct Action *>  getPlanactions (){
        return _plan_actions;
    }
    vector<vector<string>> getRelaxedActions (){
        return _RelaxedActions;
    }
    void  setRelaxedActions (vector<vector<string>> actions){
        _RelaxedActions= actions;
    }

    vector<Instruction*> getPossibleActions (){
        return _possibleActions;
    }
    void  setPossibleActions (vector<Instruction*> actions){
        _possibleActions=actions ;
    }


	Instruction* getInstruction( int program_line, const string &name ){
	    assert( program_line < (int)_instructions_line.size() );
	    for(auto & instruction : _instructions_line[ program_line ]){
	        if(instruction->toString(false).find(name ) != string::npos ){
	            return instruction;
	        }
	    }
	    return nullptr;
	}
	
	string toString(){
		string ret = "[GENERALIZED DOMAIN]:\n";
		for( int i = 0; i < int( _instructions_line.size() ); i++ ){
			ret += "[LINE " + to_string( i ) +"]:\n";
			for(auto & instruction : _instructions_line[ i ]){
				//ret += "ID: " + to_string(instruction) + " ";
				ret += instruction->toString(true) + "\n";
			}
		}
		return ret;
	}

    void setPrunedAction(vector<string> actions){
        pruned_actions=actions;
    }

    vector<string> getPrunedAction() const{
        return pruned_actions;
    }
	
	
private:
	vector< vector< Instruction* > > _instructions_line;
	vector< Condition* > _conds;
	vector< Action* > _extra_actions;
    vector< Action* > _plan_actions;
    vector<vector<string>> _RelaxedActions;
    vector<Instruction*> _possibleActions;

    vector<string> pruned_actions;
};

#endif
