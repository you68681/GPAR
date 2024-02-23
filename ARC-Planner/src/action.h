#ifndef __ACTION_H__
#define __ACTION_H__

#include "common.h"
#include "state.h"
#include "state_descriptor.h"
#include "condition.h"
#include "operation.h"

class Action{
public:
	explicit Action( const string &name = "", const string &atype = "math" ) : _name( name ), _atype( atype ){
        _planning_action= false;
        _check_action= false;
        _pointer_operation= false;
        _uninstantiated_action=false;
        _act_name="";
        _checked=false;
        _external_function= false;
        _param_type_list={};
        _act_param_type={};
         _act_param_idx={};
         _param_index_list={};
        _external_functions= nullptr;
    }
	
	virtual ~Action(){
		for(auto & precondition : _preconditions){
			delete precondition;
		}
		
		for(auto & effect : _effects){
			delete effect;
		}
        delete _external_functions;
        _act_name.clear();
        _param_type_list.clear();
        _act_param_type.clear();
        _act_param_idx.clear();
        _param_index_list.clear();
        _preconditionsWord.clear();
        _effectsWord.clear();
        _preconditionsStructure.clear();
        _effectsStructure.clear();


	}
	
	virtual bool isApplicable( const State *s ){
		for(auto & precondition : _preconditions){
			if( ! precondition->evalCondition( s ) )
				return false;
		}
		return true;
	}

    virtual int apply_external( State *s ){
        int res = 1;
        // map the action name to specific implementation methods. (will be updated in future )
        if (_act_name=="update-color")
            res=_external_functions->update_color(s,0);

        if (_act_name=="swap-color")
            res=_external_functions->swap_color(s,0);

        if (_act_name=="copy-color")
            res=_external_functions->copy_color(s,0);

        if (_act_name=="mirror")
            res=_external_functions->mirror(s,0);
        if (_act_name=="rotate")
            res=_external_functions->rotate_node(s,0);
        if (_act_name=="flip")
            res=_external_functions->flip(s,0);

        if (_act_name=="insert-input-node")
            res=_external_functions->insert_input_node(s,0);

        if (_act_name=="insert-output-node")
            res=_external_functions->insert_output_node(s,0);


        if (_act_name=="hollow-rectangle")
            res=_external_functions->hollow_rectangle(s,0);

        if (_act_name=="fill-rectangle")
            res=_external_functions->fill_rectangle(s,0);

        if (_act_name=="move-node-direction")
            res=_external_functions->move_node_direction(s,0);

        if (_act_name=="move-node-max")
            res=_external_functions->move_node_max(s,0);

        if (_act_name=="move-node-max-direction")
            res=_external_functions->move_node_max_direction(s,0);

        if (_act_name=="move-between-nodes-direction")
            res=_external_functions->move_between_nodes_direction(s,0);


        if (_act_name=="add-border")
            res=_external_functions->add_border(s,0);

        if (_act_name=="extend-node")
            res=_external_functions->extend_node(s,0);

        if (_act_name=="extend-node-direction")
            res=_external_functions->extend_node_direction(s,0);


        return res;

    }

    virtual int apply_check( State *s ){

         if (_effects[0]->check(s))
             return 1;
        return 0;


    }
	
	virtual int apply( State *s ){
	   //int res = 1;

		vector< int > num_effects( _effects.size(), 0 );
		for( int i = 0; i < int( _effects.size() ); i++ ){
			num_effects[ i ] = ( _effects[ i ]->getEffect( s ) );
		//	res &= (num_effects[i] > 0?1:0);
		}
		// Following the planning community convention
		// first apply the negative effects, then positives
		for( int neg = 0; neg < 2; neg++ ){
			for( int i = 0; i < int( _effects.size() ); i++ ){
				if( ( neg == 0 and num_effects[i] == 0 ) or // first negative effect
                    ( neg == 1 and num_effects[i] > 0 ) ) // second positive effect
                    _effects[ i ]->setEffect( s, num_effects[ i ] );
			}
		}
	
		// Return the last effect
		if( int( num_effects.size() ) == 0 ) return 0; // 0 by default if no effects, e.g. CMP action
		return num_effects[ int(num_effects.size()) - 1 ];
		//return res;
	}
	
	virtual void addCondition( Condition *cond ){
		_preconditions.push_back( cond );
	}



	virtual void addOperation( Operation* op ){
		_effects.push_back( op );
	}


    virtual void addExteralFunction (Operation* op){
        _external_functions=op;
    }

	virtual string getName() const{
		return _name;
	}
	
	virtual string getType() const{
		return _atype;
	}

	virtual vector<Condition*> getPreconditions() const{
	    return _preconditions;
	}
    virtual vector< Operation* > getActionEffect() const{
        return _effects;
    }
	virtual vector<Operation*> getEffects() const{
	    return _effects;
	}
    virtual void setActName(string actName){
        _act_name=actName;
    }
    virtual string getActName() const{
        return _act_name;
    }

    virtual void setParamTypeList(vector<string> parameters){
        _param_type_list=parameters;
    }
    virtual vector<string> getParamTypeList() const{
        return _param_type_list;
    }

    virtual void setChecked(bool flag){
        _checked=flag;
    }
    virtual bool getChekced() const{
        return _checked;
    }



    virtual bool getPlanningAction() const{
        return _planning_action;
    }
    virtual void setPlanningAction(bool actionFlag){
        _planning_action=actionFlag;
    }

    virtual  void set_external_function(bool externalFlag) {
        _external_function=externalFlag;
    }

    virtual bool checkExternalFunction(){
        return _external_function;
    }

    virtual bool getCheckAction() const{
        return _check_action;
    }
    virtual void setCheckAction(bool actionFlag){
        _check_action=actionFlag;
    }


    virtual bool getUninstantiatedAction() const{
        return _uninstantiated_action;
    }
    virtual void setUninstantiatedAction(bool actionFlag){
        _uninstantiated_action=actionFlag;
    }


    virtual bool getPointerOperation() const{
        return _pointer_operation;
    }
    virtual void setPointerOperation(bool pointer_operation){
        _pointer_operation=pointer_operation;
    }

    virtual void setActionEffectWord(vector<string> actioneffect){
        _effectsWord=actioneffect;
    }
    virtual void setActionPreWord(vector<string> actionPre){
        _preconditionsWord=actionPre;
    }

    virtual void setActionEffectStructure(vector<vector<string>> actioneffect){
        _effectsStructure=actioneffect;
    }
    virtual void setActionPreconditionsStructure(vector<vector<string>> actionPre){
        _preconditionsStructure=actionPre;
    }

    virtual vector<vector<string>> getActionEffectStructure(){
        return _effectsStructure;
    }
    virtual vector<vector<string>>  getActionPreconditionsStructure(){
        return  _preconditionsStructure;
    }


    virtual void setActionParaType(map< string, string > actionParaType){
        _act_param_type=actionParaType;
    }
    virtual void setActionParaIndex(map< string, int > actionParaIndex){
        _act_param_idx=actionParaIndex;
    }


    virtual void setActionParaIndexVector(vector<int> actionParaIndex){
        _param_index_list=actionParaIndex;
    }
    virtual vector<int> getActionParaIndexVector() const{
        return _param_index_list;
    }

    virtual map< string, string > getActionParaType(){
        return _act_param_type;
    }
    virtual map< string, int >  getActionParaIndex(){
        return  _act_param_idx;
    }


    virtual vector<string> getActionEffectWord(){
        return _effectsWord;
    }
    virtual vector<string> getActionPreWord(){
        return _preconditionsWord;
    }
	virtual string toString( bool full_info ) const{
		if( !full_info ) return _name+"\n";
		string ret = "[ACTION]: " + _name + "\n";
		ret += "TYPE: " + _atype + "\n";
		ret += "PRECONDITIONS:\n";
		for(auto precondition : _preconditions){
			ret += precondition->toString( false ) + "\n";
		}
		ret += "EFFECTS:\n";
		for(auto effect : _effects){
			ret += effect->toString( false );
		}
		return ret;
	}
	
private:
	string _name;
    string _act_name;
	string _atype;
    bool _uninstantiated_action;
    bool _planning_action;
    bool _external_function;
    bool _check_action;
    bool _pointer_operation;
	vector< Condition* > _preconditions;
	vector< Operation* > _effects;

    Operation* _external_functions;

    vector< string > _preconditionsWord;
    vector< string > _effectsWord;



    vector< vector<string> > _preconditionsStructure;
    vector< vector<string> > _effectsStructure;

    bool _checked;
    vector<string> _param_type_list;
    vector<int> _param_index_list;
    map< string, string > _act_param_type;
    map< string, int > _act_param_idx;
};

#endif
