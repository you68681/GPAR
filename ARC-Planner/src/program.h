#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "common.h"
#include "program_state.h"
#include "instruction.h"
#include "generalized_planning_problem.h"
#include "landmark_graph.h"

class Program{
public:
	explicit Program( int program_lines = 1, Instruction* end_instruction = nullptr ){
        assert( program_lines >= 1 );
		_instructions = vector<Instruction*>( program_lines, nullptr );
        setInstruction( program_lines - 1, end_instruction, true);
        _num_of_steps = 0;
        _lm_count = -1;
        _failed_instance_id = -1;
        _my_map={};
        _location_of_null={};
        _num_of_test=0;
        _num_of_goal=0;
        _num_of_max_line=-1;
        _num_of_check=0;
        _goal_achieved = true;
        _num_of_cmp_pointer = 0;
        _num_of_cmp_location = 0;
        _end= {};
        _action_name={};
        _num_of_unrelated_actions=0;
        _checked= false;
        _actions_checked={};
        _actions_locations={};
        _default_action_line=0;
        _current_line=-1;
	};
	
	explicit Program( Program* p ){
        map<string,int> my_map=p->getMap();
        _my_map=my_map;
		vector< Instruction* > vi = p->getInstructions();
		_instructions.resize( vi.size() );
		for( int i = 0; i < int( _instructions.size() ); i++ )
			_instructions[ i ] = vi[ i ];
        _actions_checked=p->getActionChecked();
        _actions_locations=p->getActionsLocations();
        vector<int> end_vector=p->getEnd();
        _checked=p->getChecked();
        _default_action_line=p->getDefaultActionLine();
        _end=end_vector;
        vector<string> my_action_name=p->getActionName();
        _action_name=my_action_name;
        _num_of_steps = 0;
        _lm_count = -1;
        _failed_instance_id = -1;
        /*if( _landmark_graphs.size() > 0u ) {
            _num_visited_landmarks = 0;
            setLandmarkGraphs(p->getLandmarks());
            assert( p->getLandmarks().size() > 0u );
            assert( getLandmarks().size() > 0u );
        }*/
        _num_of_steps = 0;
        _location_of_null={};
        _num_of_test=0;
        _num_of_goal=0;
        _num_of_max_line=-1;
        _goal_achieved = true;
        _num_of_cmp_pointer = 0;
        _num_of_cmp_location = 0;
        _end={};
        _num_of_unrelated_actions=0;
        _num_of_check=0;
        _current_line=-1;

	}
	
	~Program()= default;

	
	Program* copy(){
		return new Program( this );
	}
	
	vector< Instruction* > getInstructions() const{
		return _instructions;
	}
    map<string,int> getMap() const{
        return _my_map;
    }
    int getDefaultActionLine () {
        return _default_action_line;
    }

    void setDefaultActionLine(int l){
        _default_action_line=l;
    }


    void setInstruction( int pl, Instruction* ins,bool flag ){
        assert( pl < int( _instructions.size() ) );
        _instructions[ pl ] = ins;
        if (flag) {
            PlanningAction *act = dynamic_cast< PlanningAction * >( ins );
            // not goto instruction.
            if (act) {
                string ActName = act->getActName();
                if (ActName != "") {
                    if (_my_map.find(ActName) == _my_map.end()) {
                        _my_map.insert(std::pair<string, int>(ActName, 1));
                    } else {
                        _my_map[ActName]++;
                    }
                } else {
                    string name = act->getName();
                    if (_my_map.find(name) == _my_map.end()) {
                        _my_map.insert(std::pair<string, int>(name, 1));
                    } else {
                        _my_map[name]++;
                    }
                }
            } else {
                if (ins and ins->getName() == "end") {
                    _end.push_back(pl);
                }
            }
        }
    }
	Instruction* getInstruction( int pl ){
	    assert( pl < int( _instructions.size() ) );
		return _instructions[ pl ];
	}

	int getNumInstructions() const{
		return int( _instructions.size() );
	}
	
	bool haltingCondition( ProgramState* ps, int &error ){
		int line = ps->getLine();

        if( _instructions[ line ] == nullptr ){
            if (line==0) {
                return true;
            }
            // current line saves the location of first empty line during execution
            _current_line=line,
            // if current execution line is empty  then set the next execution line to the first line in the loop section
            ps->setLine(this->getDefaultActionLine());
            return false;
        }


        // Either True or False evaluation of End instructions is a halting condition
		End* ins_end = dynamic_cast< End* > ( _instructions[ line ] );
		if( ins_end ) return true;

        // Conditional effects, horizontal_action do not have to be necessarily applicable
		// (OLD) If it's a planning action, it must be applicable
		/*PlanningAction* ins_act = dynamic_cast< PlanningAction* > ( _instructions[ line ] );
		if( ins_act && !ins_act->isApplicable( ps ) ){
            #ifdef VALIDATOR
		    cout << ps->toString() << endl;
		    cout << ins_act->toString() << endl;
            #endif
			error = -2; // ERROR 2: inapplicable instruction
			return true;
		}*/
		
		return false;
	}
	

    bool checkGoal( ProgramState *ps, Instance *ins, int &error ){
        int line = ps->getLine();
        if( _instructions[ line ] == NULL ) return false;
        // all program lines are programed
        if (not _location_of_null.empty()) return false;
        End *end = dynamic_cast<End*>( _instructions[ line ] );
        if( end and !end->isGoalState( ins, ps ) ){
#ifdef DEBUG
            cout << ps->toString() << endl;
#endif
            error = -1; // ERROR 1: Incorrect program
            return false;
        }
        if (end and end->isGoalState( ins, ps )) {
            return true;
        }
        return false;
    }


    static bool checkDeadEnd(StateDescriptor *sd, ProgramState *ps, LandmarkGraph *lm, int &error ){
	    unique_ptr<RelaxedState> rs = make_unique<RelaxedState>( sd );
	    auto reg = ps->getState()->getTypedRegisters();
	    for( int pred_idx = 0; pred_idx < (int)reg.size(); pred_idx++ ) {
	        for( const auto& m : reg[ pred_idx ] )
                rs->addFact(pred_idx, m.first, m.second );
        }
	    bool res = lm->isDeadEnd( rs );
	    if( res ) error = -4;

	    return res;
	}
	
	vector< ProgramState* > run( GeneralizedPlanningProblem *gpp, bool infinite_detection = false,
                                 bool progressive_eval = false ){
        _num_of_steps = 0;

		int num_instances = gpp->getNumInstances();
		StateDescriptor *sd = (gpp->getDomain())->getStateDescriptor();
		auto pred_names = sd->getPredicateTypes();

		// One program state per instance
		vector< ProgramState* > pss( num_instances ) ;
		for( int i = 0; i < num_instances; i++ )
			pss[ i ] = new ProgramState();


        #ifdef VALIDATOR
        time_t start_instance, end_instance;
        time( &start_instance );
        #endif
        int errors = 0;


        bool false_instance=true;
        for( int id = 0; id < num_instances; id++ ){
            if(progressive_eval and gpp->isInstanceActive(id) ) continue;
			// Initialize local initial state
			Instance *ins = gpp->getInstance( id );			
			ProgramState *ps = pss[ id ];
			State *s = ( ins->getInitialState() )->copy();
			int line = ps->getLine();
			ps->setState( s );

			// For detecting infinite loops (it can be enhanced with a hash or bigint identifier)
			set< vector<int> > visited;

			int error = 0;

			while( !haltingCondition( ps, error ) ){
				if( infinite_detection ){
					// Checking infinite loop (only for backward loops)
					Goto *g = dynamic_cast<Goto*>( _instructions[ line ] );
					if( g and g->getDestinationLine() < line ){
						// Infinite loop detected
#ifdef MAX_STEPS
                        if( MAX_STEPS*num_instances < _num_of_steps ){
                            error = -3; break; // ERROR 3: Infinite program
                        }
#else
                        vector<int> state_id = ps->asVector();
                        if( visited.find( state_id ) != visited.end() ){
                            error = -3; // ERROR 3: Infinite program
                            break;
                        }
                        visited.insert( state_id );
#endif
					}
				}
				
				// Retrieving program line
				line = ps->getLine();


				int res = _instructions[ line ]->apply( ps );
				_num_of_steps++;

				// Mathematical planning horizontal_action update zero and carry flags
				auto *act = dynamic_cast< PlanningAction* >( _instructions[ line ] );

				if( act && act->getType() == "math" ){
					s->setPointer(sd, "zf", ( res == 0 ?1:0) ); // zero-flag
                    if( CARRY_FLAG )
					    s->setPointer(sd, "cf", ( res > 0 ? 1:0) ); // carry-flag
				}

			}


            if( error == 0 )
			    if (not checkGoal( ps, ins, error ) and false_instance) {
                    _failed_instance_id = id;
                    false_instance= false;
                }
			// [ENHANCEMENT] Continue execution in validation for coverage metric
			#ifdef VALIDATOR
            time(&end_instance);
			if( error < 0 ){
			    errors++;
				cout << "INSTANCE #" << (id+1) << " ERROR " << error << "... :( [" << difftime(end_instance,start_instance) << "]" << endl;
				for( int i = 0; i < int( pss.size() ); i++ ){
				    cout << pss[i]->toString( sd ) << endl;
				}
			}
			else
				cout << "INSTANCE #" << (id+1) << " SOLVED! [" << difftime(end_instance,start_instance) <<"]" << endl;
			start_instance = end_instance;
            #else

            if( error < 0 ){
                _failed_instance_id = id;
                errors++;
                break;
                /*for( int i = 0; i < int( pss.size() ); i++ ){
                    //cout << pss[i]->toString( sd ) << endl;
                    if( pss[i] ) delete pss[i];
                }
                return vector<ProgramState*>();*/
            }
			#endif
		}

        if( errors ){
            for(auto & ps : pss){
                delete ps;
            }
            return {};
        }


		
		return pss;
	}

    vector< ProgramState* > runNovelty( GeneralizedPlanningProblem *gpp, bool infinite_detection = true,
                                 bool progressive_eval = false ){
        _num_of_steps = 0;
        _num_of_test  = 0;
        _num_of_goal  = 0;
        _num_of_max_line =-1;
        _goal_achieved = true;
        _num_of_cmp_pointer = 0;
        _num_of_cmp_location = 0;
        _end={};

        int num_instances = gpp->getNumInstances();
        StateDescriptor *sd = (gpp->getDomain())->getStateDescriptor();
        auto pred_names = sd->getPredicateTypes();

        // One program state per instance
        vector< ProgramState* > pss( num_instances ) ;
        for( int i = 0; i < num_instances; i++ )
            pss[ i ] = new ProgramState();


#ifdef VALIDATOR
        time_t start_instance, end_instance;
        time( &start_instance );
#endif
        int errors = 0;
        int width= gpp->getDomain()->getWidth();
        bool check_action_used= false;
        set< vector<int> > visited;

        // prune the newly generated planning program if its action novelty rank v > threshold.
        for( auto & actions : _my_map ){
            if (actions.second>width){
                for(auto & ps : pss){
                    delete ps;
                }
                return {};
            }
        }

            set<string> relaxactions = gpp->getRelaxes();
            for (int i=0; i<_instructions.size();i++){

                 // record the first empty line
                if (_instructions[i]==NULL){
                    _location_of_null.push_back(i);
                }

                Goto *gofrom = dynamic_cast<Goto*>( _instructions[ i ] );
                // the destination line of an goto action cannot be another goto action ( Novelty and Lifted Helpful Actions in Generalized Planning)
                if (gofrom){
                    Goto *godes = dynamic_cast<Goto*>( _instructions[gofrom->getDestinationLine()]);
                    if (godes){
                        for(auto & ps : pss){
                            delete ps;
                        }
                        return {};

                    }
                    // we restrict the destination line of an goto action cannot be a check instruction
                    PlanningAction *act1 = dynamic_cast< PlanningAction * >( _instructions[gofrom->getDestinationLine()]);
                    if (gofrom->getDestinationLine()>i) {
                        if (act1 and act1->getCheckAction()) {
                            for(auto & ps : pss){
                                delete ps;
                            }
                            return {};
                        }

                    }
                }
            }



        for( int id = 0; id < num_instances; id++ ){
            if(progressive_eval and not gpp->isInstanceActive(id) ) continue;
            // Initialize local initial state
            Instance *ins = gpp->getInstance( id );
            ProgramState *ps = pss[ id ];
            State *s = ( ins->getInitialState() )->copy();
            int line = ps->getLine();
            ps->setState( s );


            // For detecting infinite loops (it can be enhanced with a hash or bigint identifier)
            set< vector<int> > visited;
            int error = 0;
            vector<vector<int>> test_visited;
            vector<vector<int>> cmp_pointers;
            vector<vector<int>> cmp_locations;
            vector<vector<int>> check_visited;
            // method haltingCondition will allocate the execution to the loop section if current execution line is empty.
            while( !haltingCondition( ps, error ) ){
                if( infinite_detection ){
                    // Checking infinite loop (only for backward loops)
                    Goto *g = dynamic_cast<Goto*>( _instructions[ line ] );
                    if( g and g->getDestinationLine() < line ){
                        // Infinite loop detected
#ifdef MAX_STEPS
                        if( MAX_STEPS*num_instances < _num_of_steps ){
                            error = -3; break; // ERROR 3: Infinite program
                        }
#else
                        vector<int> state_id = ps->asVector();
                        if( visited.find( state_id ) != visited.end() ){
                            error = -3; // ERROR 3: Infinite program
                            break;
                        }
                        visited.insert( state_id );
#endif
                    }
                }


                line = ps->getLine();

                // apply the instruction in the current program state (ps) and update the execution line to the next lie in sequence.
                int res = _instructions[ line ]->apply( ps );



                _num_of_steps++;


                auto *act = dynamic_cast< PlanningAction* >( _instructions[ line ] );
                auto pointers=sd->getPointerNames();


                if (act) {
                    string name = act->getName();
                    // variant of h_cn introduced in the Novelty and Lifted Helpful Actions in Generalized Planning
                    if (CHECK_CHECK) {
                        if (act->getCheckAction()) {

                            vector<int> iterms;
                            if (act->getName()!="check-condition(true)" and sd->checkpointer(act->getActioParamIndexVector()[0])) {

                                iterms.push_back(s->getPointer(sd, act->getActioParamIndexVector()[0]));
                                if (std::find(check_visited.begin(), check_visited.end(), iterms) == check_visited.end()) {
                                    _num_of_check++;
                                    check_visited.push_back(iterms);

                                }

                            }


                        }
                    }

                }
                // update the yz based on the result of the RAM actions (Novelty and Lifted Helpful Actions in Generalized Planning)
                if( act && act->getType() == "math" ){
                    s->setPointer(sd, "zf", ( res == 0 ?1:0) ); // zero-flag
                    if( CARRY_FLAG )
                        s->setPointer(sd, "cf", ( res > 0 ? 1:0) ); // carry-flag
                }

            }
            // save the first empty line during the execution
            line=_current_line;
            if (_current_line!=-1) {
                ps->setLine(_current_line);
            }


            // check goal achieved
            if( error == 0 )
            {
                if (_goal_achieved)
                {
                    _goal_achieved=checkGoal( ps, ins, error );
                }
                else{
                    checkGoal( ps, ins, error );
                }
            }
//                checkGoal( ps, ins, error );

            // [ENHANCEMENT] Continue execution in validation for coverage metric
#ifdef VALIDATOR
            time(&end_instance);
			if( error < 0 ){
			    errors++;
				cout << "INSTANCE #" << (id+1) << " ERROR " << error << "... :( [" << difftime(end_instance,start_instance) << "]" << endl;
				for( int i = 0; i < int( pss.size() ); i++ ){
				    cout << pss[i]->toString( sd ) << endl;
				}
			}
			else
				cout << "INSTANCE #" << (id+1) << " SOLVED! [" << difftime(end_instance,start_instance) <<"]" << endl;
			start_instance = end_instance;
#else


            if( error < 0 ){
                _failed_instance_id = id;
                errors++;
                break;
            }

            // calculation details for  h_p introduced in the paper Generalized Planning for the Abstraction and Reasoning Corpus
            auto initial_pixels=ins->getInitialState()->getGraph()->getImagePixel();
            auto ps_pixels=ps->getState()->getGraph()->getImagePixel();
            auto goal_pixels=ps->getState()->getGraph()->getGoalPixelColor();
            int penalty=0;
            for (auto pixel: ps_pixels) {
                int pixelId=pixel.first;
                int pixelColor=pixel.second->getColor();


                if (pixelColor==goal_pixels[pixelId])
                    continue;
                if (pixelColor!=goal_pixels[pixelId] and pixelColor!=initial_pixels[pixelId]->getColor()) {
                    penalty += 3;
                }
                if (pixelColor!=goal_pixels[pixelId] and pixelColor==initial_pixels[pixelId]->getColor()) {
                    penalty+=1;
                }

            }
            _num_of_goal += penalty;

            // record the maximum reached line as the next programable line
            int max_line = ps->getLine();
            if(_instructions[max_line] == NULL ){
                _num_of_max_line = max( _num_of_max_line, max_line );
            }

#endif
        }



        if( errors ){
            for(auto & ps : pss){
                delete ps;
            }
            return {};
        }
        return pss;
    }

    void processReachedLandmarks( StateDescriptor *sd, RelaxedGraph *rg, State *s, int landmark_graph_id,
                                  set<Landmark*> &landmarks, set<int> &accepted, bool pointer_landmarks = false ){
        vector<int> reached;
        for( auto l :  landmarks ){
            if( pointer_landmarks and not l->isPointerLandmark() ) continue;
            if( not pointer_landmarks and l->isPointerLandmark() ) continue;
            int lm_id = l->getID();
            // If the landmark was already accepted continue;
            if( accepted.find( lm_id ) != accepted.end() ) continue;
            bool landmark_holds;
            if( l->isPointerLandmark() ) landmark_holds = l->holds(sd,s);
            else landmark_holds = l->holds(rg, s);
            // New landmark reached
            if( landmark_holds ){
                bool is_reached = true;
                // check if gn landmarks hold
                auto gn_landmarks = _landmark_graph[ landmark_graph_id ]->getInverseGreedyNecessaryLandmarkOrdering( lm_id );
                for( auto gn_l_id : gn_landmarks ){
                    // check if a gn condition has not been accepted yet
                    if( accepted.find( gn_l_id ) == accepted.end() ){
                        is_reached = false; break;
                    }
                }
                auto nat_landmarks = _landmark_graph[ landmark_graph_id ]->getInverseNaturalLandmarkOrdering( lm_id );
                for( auto nat_l_id : nat_landmarks ){
                    // check if a nat condition has not been accepted yet
                    if( accepted.find( nat_l_id ) == accepted.end() ){
                        is_reached = false; break;
                    }
                }
                // if all gn and nat landmarks have been accepted, include the new landmark as reached
                if( is_reached )
                    reached.push_back( lm_id );
            }
        }
        // Union of reached and accepted
        std::copy( reached.begin(), reached.end(), inserter( accepted, accepted.end() ) );
    }

	int getNumOfSteps() const{
	    return _num_of_steps;
	}
    int getNumOfgoal() const{
        return _num_of_goal;
    }
    int getNumOfmaxline() const{
        return _num_of_max_line;
    }

    void setNumOfmaxline(int l) {
        _num_of_max_line=l;
    }

    bool getGoalAchieved() const{
        return _goal_achieved;
    }
    vector<int> getEnd() const{
        return _end;
    }
    vector<string> getActionName() const{
        return _action_name;
    }
    int getNumOftest() const{
        return _num_of_test;
    }

    int getNumOfcheck() const{
        return _num_of_check;
    }

    int getNumOfcmpPointer() const{
        return _num_of_cmp_pointer;
    }
    int getNumOfcmpLocation() const{
        return _num_of_cmp_location;
    }
    int getNumOfrelatedActions() const{
        return _num_of_unrelated_actions;
    }
    vector<int> getLocofNull() const{
        return _location_of_null;
    }

	void setLandmarkGraphs(const vector< LandmarkGraph* > &landmark_graph ){
        _landmark_graph = landmark_graph;
	}

    int getLMCount() const{
	    return _lm_count;
    }

    int getFailedInstanceID() const{
        return _failed_instance_id;
    }
    void setChecked(bool flag){
        _checked=flag ;
    }
    bool getChecked(){
        return _checked;
    }

    void setActionsLocations(int location){
        _actions_locations.push_back(location) ;
    }
    vector<int> getActionsLocations(){
        return _actions_locations;
    }

    map<string,bool> getActionChecked(){
        return _actions_checked;
    }

    int getFirstEmptyLine() const {
        int line = 0;
        for( auto ins : _instructions ){
            if( ins == nullptr )
                return line;
            line++;
        }
        return -1;
    }

    int getTotalLandmarks(int id) {
        assert(id < (int)_landmark_graph.size() );
        return _landmark_graph[id]->getNumLandmarks();
    }

    int getUnachieveLandmarks(int id) {
        assert(id < (int)_unachieved_landmarks.size());
        return _unachieved_landmarks[id];
    }
	
	string toString( bool full_info = true ) const{
		string ret;
		if( full_info ){
			ret = "[PROGRAM]:\n";
			for( int i = 0; i < int( _instructions.size() ); i++ ){
				ret += "LINE " + to_string( i ) + ": ";
				if( _instructions[ i ] != nullptr )
					ret += _instructions[ i ]->toString(true);
				else ret += "[INSTRUCTION] EMPTY\n";
			}
		}
		else{
			for( int i = 0; i < int( _instructions.size() ); i++ ){
				ret += to_string( i ) + ". ";
				if( _instructions[ i ] != nullptr )
					ret += _instructions[ i ]->toString( full_info );
				else ret += "empty\n";
			}
		}
		return ret;
	}
	
private:
    vector< LandmarkGraph* > _landmark_graph;
	int _lm_count;
	vector< Instruction* > _instructions;
	int _num_of_steps;
    int _failed_instance_id;
    vector<int> _unachieved_landmarks;
    int _num_of_test;
    int _num_of_cmp_pointer;
    int _num_of_cmp_location;
    int _num_of_check;
    vector<int> _location_of_null;
    int _num_of_goal;
    int _num_of_max_line;
    bool _goal_achieved;
    int _num_of_unrelated_actions;
    std::map<string, int> _my_map;
    vector<int> _end;
    vector<string> _action_name;
    bool _checked;
    vector<int> _actions_locations;
    map<string,bool> _actions_checked;
    int _default_action_line;
    int _current_line;

};

#endif
