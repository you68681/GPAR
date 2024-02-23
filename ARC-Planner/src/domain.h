#ifndef __DOMAIN_H__
#define __DOMAIN_H__

#include "common.h"
#include "state_descriptor.h"
#include "action.h"

class Domain{
public:
	explicit Domain( const string &name = "" ){
		_name = name;
		_sd = nullptr;
        _prune_object={};
        _prune_type={};
	}
		
	~Domain(){
        _action_name_to_idx.clear();
//		for(auto & action : _actions){
//			delete action;
//		}
        _action_schema_name_to_idx.clear();
        for(auto & action : _actions_schema){
            delete action;
        }

        delete _sd;

        _prune_object.clear();
        _prune_type.clear();

	}

	void setName( const string &name = "" ){
		_name = name;
	}

    void setPrune(const int objID){
        _prune_object.push_back(objID);
    }

    void setPruneType( const string& type){
        _prune_type.push_back(type);
    }


	void setStateDescriptor( StateDescriptor *sd ){
		_sd = sd;
	}

    void addActionSchema( Action *a ){
        _actions_schema.push_back( a );
        _action_schema_name_to_idx[ a->getName() ] = int( _actions_schema.size() ) - 1;
    }

	void addAction( Action *a ){
		_actions.push_back( a );
        _action_name_to_idx[ a->getName() ] = int( _actions.size() ) - 1;
	}
	
	StateDescriptor* getStateDescriptor(){
		return _sd;
	}

    vector< Action* > getActionsSchema() const{
        return _actions_schema;
    }


    vector<int> getPruneObject() const{
        return _prune_object;
    }

    vector<string> getPruneType() const{
        return _prune_type;
    }

    vector< Action* > getActions() const{
		return _actions;
	}


    Action* getActionSchema( int id ){
        return _actions_schema[ id ];
    }


	Action* getAction( int id ){
		return _actions[ id ];
	}

    Action* getActionSchema( const string &name = "" ){
        auto it = _action_schema_name_to_idx.find( name );
        if( it == _action_schema_name_to_idx.end() )
            return nullptr;
        return getActionSchema( it->second );
    }


    Action* getAction( const string &name = "" ){
	    auto it = _action_name_to_idx.find( name );
	    if( it == _action_name_to_idx.end() )
	        return nullptr;
	    return getAction( it->second );
	}


	int getNumActionsSchema() const{
		return (int)_actions_schema.size();
	}
	
	string getName() const{
		return _name;
	}
    int getWidth (){
        return _width;
    }
    void  setWidth (int width){
        _width= width;
    }
	string toString() const{
		string ret = "[DOMAIN]: " + _name + "\n";
		if( _sd )
			ret += "\n" + _sd->toString();
		for(auto action : _actions){
			ret += "\n" + action->toString(true);
		}
		return ret;
	}
	
private:
	string _name;

	StateDescriptor *_sd;
	vector< Action* > _actions;
    vector< Action* > _actions_schema;
	map< string, int > _action_name_to_idx;
    map<string,int> _action_schema_name_to_idx;

    vector<int> _prune_object;
    vector<string> _prune_type;
    int _width;
}; 

#endif
