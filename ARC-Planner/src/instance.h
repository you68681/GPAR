#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include "common.h"
#include "state.h"
#include "condition.h"
#include "abstractgraph.h"

class Instance{
public:
	explicit Instance( const string &name = "", int instance_id = 0 ){
		_name = name;
		_init = nullptr;
		_instance_id = instance_id;
	}
	
	~Instance(){
		delete _init;

		for( auto g : _goal ){
			delete g;
		}
		_goal.clear();

        _goalWord.clear();
        _initWord.clear();


	}
	
	void setName( const string &name = "" ){
		_name = name;
	}
	
	void setInitialState( State *init ){
		_init = init;
		_init->setInstanceID( _instance_id );
	}

	
	void setGoalCondition( vector< Condition* > &goal ){
		_goal = goal;
	}
	
	void addCondition( Condition* cond ){
		_goal.push_back( cond );
	}
	
	string getName() const{
		return _name;
	}
	
	State* getInitialState(){
		return _init;
	}

	
	vector< Condition* > getGoalCondition(){
		return _goal;
	}
	
	Condition* getCondition( int id ){
		return _goal[ id ];
	}

	void addObject( StateDescriptor *sd,AbstractGraph *graph, const string &name, const string &obj_type ){
	    auto num_of_objects = sd->getStateVarBound( obj_type, _instance_id );
        auto obj_to_type=_init->getObjType();
        assert( obj_to_type.find( name ) == obj_to_type.end() );
        _init->addObjType(name,obj_type);
        _init->addObjAddress(name,num_of_objects);


//        assert( obj_to_type.find( name ) == obj_to_type.end() );
//        obj_to_type[ name ] = obj_type;
//
//	    obj_to_address[ name ] = num_of_objects;

        if (obj_type=="node"){
            this->_init->add_pointer_to_obj(num_of_objects,sd->getTypeID(name));
            graph->addNode(sd->getTypeID(name),new GraphNode(sd->getTypeID(name)));
        }


        if (obj_type=="output-node"){

            this->_init->add_pointer_to_output_obj(num_of_objects,sd->getTypeID(name));
            graph->addOutputNode(sd->getTypeID(name),new GraphNode(sd->getTypeID()));
            }

        if (obj_type=="color"){
            this->_init->add_pointer_to_color(num_of_objects,sd->getTypeID(name));
        }

        if (obj_type=="size"){
            this->_init->add_pointer_to_size(num_of_objects,sd->getTypeID(name));
        }


        if (obj_type=="shape"){
            this->_init->add_pointer_to_shape(num_of_objects,sd->getTypeID(name));
        }

        if (obj_type=="move-direction"){
            this->_init->add_pointer_to_direction(num_of_objects,sd->getTypeID(name));
        }


//        if (obj_type=="move-direction-diagonal"){
//            this->_init->add_pointer_to_direction_diagonal(num_of_objects,sd->getTypeID(name));
//        }



        if (obj_type=="pixel"){
            int x_location;
            auto start_location=name.find("-");
            for (int i=1;i<name.size();i++){
                if (name.substr(start_location-i,i).find("l")!=std::string::npos)
                {
                    x_location=stoi(name.substr(start_location-i+1,i-1));
                    break;
                }
            }

            int y_location=stoi(name.substr(start_location+1,name.size()-1-start_location));
            graph->addImagePixel(sd->getTypeID(name),new Pixel(sd->getTypeID(name),x_location,y_location));
        }

        // we need a structure to save <node,0>=node1 where node is obj_type and 0 is num_of_objects
	    sd->setStateVarBound( obj_type, _instance_id, num_of_objects+1 );
	}

	string getObjectType( const string &obj_name ) const{
        auto obj_to_type=_init->getObjType();
	    auto it = obj_to_type.find( obj_name );
	    assert( it != obj_to_type.end() );
//        auto it = _init->getObjType().find( obj_name );
//        assert( it != _init->getObjType().end() );
	    return it->second;
	}

	int getObjectAddress( const string &obj_name ) const{
        auto obj_to_address=_init->getObjAddress();
	    auto it = obj_to_address.find( obj_name );
	    assert( it != obj_to_address.end() );
//        auto it = _init->getObjAddress().find( obj_name );
//        assert( it != _init->getObjAddress().end() );
	    return it->second;
	}

	void setInstanceID( int instance_id ){
	    _instance_id = instance_id;
	    if( _init ) _init->setInstanceID( _instance_id );
	}

	int getInstanceID() const {
	    return _instance_id;
	}

    void setgoalWord( vector<string> goal ){
        _goalWord=goal;
    }
    vector<string> getgoalWord( )const{
        return _goalWord;
    }
    void setinitWord( vector<string> init ){
        _initWord=init;
    }
    vector<string> getinitWord( )const{
        return _initWord;
    }
    unordered_map< string, int > getObjAddress ()const{
//       return obj_to_address;
        return _init->getObjAddress();

    }

	// Full info printing
	string toString() const{
		string ret = "[INSTANCE]: " + _name + "\n";
        auto obj_to_type=_init->getObjType();
        auto obj_to_address=_init->getObjAddress();
        ret += "\nOBJECTS:\n";
        for( const auto& it : obj_to_type ){
            auto it2 = obj_to_address.find(it.first);
            ret += (it.first) + ":" + (it.second) +" @" + to_string(it2->second) +"\n";
        }

		ret += "\nINIT:\n";
		if( _init )
			ret += _init->toString();

		ret += "\nGOAL:\n";
		for(auto g : _goal)
			ret += g->toString(true);
		return ret;
	}


	// Pretty printing
	string toString( StateDescriptor *sd ){
		string ret = "[INSTANCE]: " + _name + "\n";
        auto obj_to_type=_init->getObjType();
        auto obj_to_address=_init->getObjAddress();
		ret += "\nOBJECTS:\n";
		for( const auto& it : obj_to_type ){
		    ret += (it.first) + ":" + (it.second) +" @" + to_string(obj_to_address[ (it.first) ]) +"\n";
		}
		
		ret += "\nINIT:\n";
		if( _init )
			ret += _init->toString( sd );
		ret += "\nGOAL:\n";
		for(auto & g : _goal)
			ret += g->toString(false ) + "\n";
		return ret;
	}
	
private:
	string _name;
	State *_init;
	vector< Condition* > _goal;
	int _instance_id;
//	map< string, string > obj_to_type;
//	map< string, int > obj_to_address;
    vector<string> _goalWord;
    vector<string> _initWord;

};

#endif
