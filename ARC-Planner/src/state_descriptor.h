#ifndef __STATE_DESCRIPTOR_H__
#define __STATE_DESCRIPTOR_H__

#include "common.h"
#include "graphnode.h"
#include "pixel.h"

class StateDescriptor{
public:
	StateDescriptor(){
        addPointer( "_ram", "zf" ); // zero flag
        if( CARRY_FLAG )
            addPointer( "_ram", "cf" ); // carry flags
	}

	virtual ~StateDescriptor(){
        _predicate_var_types.clear();
        _typed_pointers.clear();
        _type_id.clear();
        _predicate_types.clear();
        _var_types.clear();
        _pointer_names.clear();

        _object_name.clear();
        _object_id_to_name_id.clear();
        _type_id_to_object_id.clear();


        _pointer_names.clear();

        _pointer_id_to_var_type.clear();
        _pointer_id_to_typed_idx.clear(); // POINTER ID -> VAR TYPE POINTER IDX
         _pointer_id_to_name_idx.clear(); // POINTER ID -> NAME IDX
        _var_id_to_name_idx.clear(); // VAR ID -> NAME (TYPE) IDX
        _pred_id_to_name_idx.clear(); // PRED ID -> NAME (TYPE) IDX
         _var_bounds.clear(); // (POINTER/VAR/PRED) ID & INSTANCE ID -> VAR BOUND


    }
	
	int getTypeID( const string &typed_name = "" ) const{
		auto it = _type_id.find( typed_name );
		if( it == _type_id.end() )
			return -1; // typed_name does not exist
		return it->second;
	}

	// Add to the system VarType
	bool addVar( const string &var_type ){
	    if( _type_id.find( var_type ) != _type_id.end() )
	        return false;

	    _var_types.push_back( var_type );
	    int next_type_id = int( _type_id.size() );
	    _type_id[ var_type ] = next_type_id;
	    _typed_pointers[ next_type_id ] = vector<int>();
	    _var_id_to_name_idx[ next_type_id ] = int(_var_types.size())-1;
	    return true;
	}

	// Add to the system: PredicateType( VarType1, VarType2, ..., VarTypeN )
	bool addPredicate( const string &predicate_type, const vector< string > &var_types = vector<string>() ){
	    if( _type_id.find( predicate_type ) != _type_id.end() )
	        return false; // predicate_type already exists

        vector< int > var_ids(var_types.size());
        for(int v_idx = 0; v_idx < (int)var_types.size(); v_idx++){
            auto it = _type_id.find( var_types[v_idx] );
            // If the var type does not exist add it to the system
            if( it == _type_id.end() ){
                if( !addVar( var_types[v_idx] ) )
                    return false; // error while adding a new var type
                it = _type_id.find( var_types[v_idx] );
                assert( it != _type_id.end() );
            }
            var_ids[v_idx] = it->second;
        }

	    _predicate_types.push_back( predicate_type );
	    int next_type_id = int(_type_id.size());
	    _type_id[ predicate_type ] = next_type_id;
	    _predicate_var_types[ next_type_id ] = var_ids;
        _pred_id_to_name_idx[ next_type_id ] = int(_predicate_types.size())-1;
	    return true;
	}

	// Add to the system: VarType pointerX
	bool addPointer( const string &pointer_type, const string &pointer_name ){
        if( _type_id.find( pointer_name ) != _type_id.end() )
            return false; // pointer_name already exists

        auto it = _type_id.find( pointer_type );
        // If the pointer type does not exist, add a new var type
        if( it == _type_id.end() ){
            if( !addVar( pointer_type ) ){
                return false; // error while adding a new var type
            }
            it = _type_id.find( pointer_type );
            assert( it != _type_id.end() );
        }

        // Add pointer name and assign id
        _pointer_names.push_back( pointer_name );
        int next_type_id = int( _type_id.size() );
        _type_id[ pointer_name ] = next_type_id;

        // Link var type with pointer id
        auto it_var = _typed_pointers.find( it->second );
        assert( it_var != _typed_pointers.end() );
        (it_var->second).push_back(next_type_id);

        _pointer_id_to_var_type[ next_type_id ] = (it_var->first);
        _pointer_id_to_typed_idx[ next_type_id ] = int((it_var->second).size())-1;
        _pointer_id_to_name_idx[ next_type_id ] = int(_pointer_names.size())-1;
		return true;
	}


    bool addObject( const string &object_name, const string &object_type ){
        if( _type_id.find( object_type ) == _type_id.end() )
            return false; // pointer_name already exists
        if (_type_id.find( object_name ) != _type_id.end() )
            return false;

        int next_object_id = int(_type_id.size());
        _type_id[ object_name ] = next_object_id;





        auto it = _type_id.find( object_type );
        // If the pointer type does not exist, add a new var type

        // Add pointer name and assign id
        if(std::find(_object_name.begin(), _object_name.end(),object_name)==_object_name.end()){
            _object_name.push_back( object_name );

            _object_id_to_name_id[ next_object_id ] = int(_object_name.size())-1;

            // Link var type with pointer id
            _type_id_to_object_id[it->second].push_back(next_object_id);
        }

    }

    void setNewTypeID(){
        _new_type_id=_type_id.size();
    }

    void increaseNewTypeID(){
        _new_type_id+=1;
    }

    int getNewTypeID(){
        return  _new_type_id;
    }


    bool link_object_pointer(){
        for (auto pointer: _typed_pointers){
            for (auto &object: _type_id_to_object_id){
                if (pointer.first==object.first){
                    for(auto p: pointer.second){
                        if (std::find(object.second.begin(), object.second.end(),p)==object.second.end())
                        object.second.push_back(p);
                    }
                }
            }
        }

    }

	vector< int > getTypedPointerIDs( const string &type_name ) const{
        auto id = getTypeID( type_name );
        auto it_p = _typed_pointers.find( id );
        assert( it_p != _typed_pointers.end() );
	    return (it_p->second);
	}

    vector<int> getObjectIDs(const string &type_name) const{
        if (type_name=="node"){
            return getTypedPointerIDs(type_name);
        }

        if (type_name=="output-node"){
            return getTypedPointerIDs(type_name);
        }
        auto id = getTypeID( type_name );
        auto it_p = _type_id_to_object_id.find( id );
        assert( it_p != _type_id_to_object_id.end() );
        return (it_p->second);
    }


    int getTypeIDSize() const{
        return _type_id.size();
    }

    vector<int> getObjectIDs(int type_id) const{
        auto it_p = _type_id_to_object_id.find( type_id );
        assert( it_p != _type_id_to_object_id.end() );
        return (it_p->second);
    }




    vector< int > getTypedPointerIDs( int type_id ) const{
        auto it_p = _typed_pointers.find( type_id );
        assert( it_p != _typed_pointers.end() );
        return (it_p->second);
    }

	int getNumTypedPointers( const string &type_name ) const {
        auto id = getTypeID( type_name );
        auto it_p = _typed_pointers.find( id );
        assert( it_p != _typed_pointers.end() );
	    return int(( it_p->second ).size());
	}

	int getNumPointers() const{
	    return int(_pointer_names.size());
	}

	int getPointerVarType( int pointer_id ) const{
	    auto it = _pointer_id_to_var_type.find( pointer_id );
	    assert( it != _pointer_id_to_var_type.end() );
	    return it->second;
	}

    bool checkpointer( int pointer_id ) const{
        auto it = _pointer_id_to_name_idx.find( pointer_id );
        if (it != _pointer_id_to_name_idx.end()){
            return true;
        }
        return false;
    }

	int getPointerVarType( const string &pointer_name ) const{
	    auto pointer_id = getTypeID( pointer_name );
	    return getPointerVarType( pointer_id );
	}

	int getPointerTypedIDX( int pointer_id ) const{
        auto it = _pointer_id_to_typed_idx.find( pointer_id );
        assert( it != _pointer_id_to_typed_idx.end() );
        return (it->second);
	}

	int getPointerTypedIDX( const string &pointer_name ) const{
	    auto pointer_id = getTypeID( pointer_name );
	    return getPointerTypedIDX( pointer_id );
	}

	int getPointerIDX( int pointer_id ) const {
        auto it = _pointer_id_to_name_idx.find( pointer_id );
        assert( it != _pointer_id_to_name_idx.end() );
        auto idx = (it->second);
        assert( (unsigned)idx < _pointer_names.size() );
        return idx;
	}

    int getPointerIDX( const string &pointer_name ) const{
        auto pointer_id = getTypeID( pointer_name );
        return getPointerIDX( pointer_id );
    }

	string getPointerName( int pointer_id ) const {
	    auto idx = getPointerIDX( pointer_id );
	    return _pointer_names[ idx ];
	}

    string getObjectName( int object_id ) const {
        auto idx = getObjectIDX( object_id );
        return _object_name[ idx ];
    }

    int getObjectIDX( int var_id ) const{
        auto it = _object_id_to_name_id.find( var_id );
        assert( it != _object_id_to_name_id.end() );
        auto idx = (it->second);
        assert( (unsigned)idx < _object_name.size() );
        return idx;
    }


	int getVarIDX( int var_id ) const{
        auto it = _var_id_to_name_idx.find( var_id );
        assert( it != _var_id_to_name_idx.end() );
        auto idx = (it->second);
        assert( (unsigned)idx < _var_types.size() );
        return idx;
	}

    int getVarIDX( const string &var_type ) const{
        auto var_id = getTypeID( var_type );
        return getVarIDX( var_id );
    }

	string getVarName( int var_id ) const{
	    auto idx = getVarIDX( var_id );
	    return _var_types[ idx ];
	}

    int getPredicateIDX( int pred_id ) const {
        auto it = _pred_id_to_name_idx.find( pred_id );
        assert( it != _pred_id_to_name_idx.end() );
        auto idx = (it->second);
        assert( (unsigned)idx < _predicate_types.size() );
        return idx;
    }

    int getPredicateIDX( const string &pred_type ) const{
        auto pred_id = getTypeID( pred_type );
        return getPredicateIDX( pred_id );
    }

	string getPredicateName( int pred_id ) const {
	    auto idx = getPredicateIDX( pred_id );
	    return _predicate_types[ idx ];
	}


	vector< int > getPredicateVarTypeIDs( const string &pred_type ) const{
	    int pred_id = getTypeID( pred_type );
	    assert( pred_id != -1 );
	    auto it_pred2vars = _predicate_var_types.find( pred_id );
	    assert( it_pred2vars != _predicate_var_types.end() );
	    return (it_pred2vars->second);
	}

	vector< string > getPredicateVarNames( const string &pred_type ) const{
	    auto var_ids = getPredicateVarTypeIDs( pred_type );
	    vector< string > var_names(var_ids.size());
        for( int i = 0; i < (int)var_ids.size(); i++){
            var_names[i] = getVarName( var_ids[i] );
	    }
	    return var_names;
	}

	void computeGroundedParams( const vector< string > &params, vector< string > &grounded_params,
                                unsigned param_idx = 0, const string &current_grounding = "" ) const{
	    assert( param_idx <= params.size() );
	    if( param_idx == params.size() ) {
	        grounded_params.push_back( current_grounding );
            return;
        }
	    auto typed_pointers = getTypedPointerIDs( params[ param_idx ] );
	    for( auto tp_id : typed_pointers ){
	        auto p_name = getPointerName( tp_id );
            // Avoid duplicate pointers as parameters (if required, add an extra pointer of the same type)
	        if( current_grounding.find(p_name) == string::npos )
                computeGroundedParams( params, grounded_params, param_idx+1,
                             current_grounding + (param_idx?",":"") + getPointerName(tp_id) );
	    }
	}

	vector< vector< int > > computeGroundedParamIDs( const vector< string > &params,
                                                     vector< string > &grounded_params ) const{
	    if( params.empty() )
	        return {};
        computeGroundedParams( params, grounded_params );
        vector< vector< int > > grounded_params_ids;
        for( const auto& gp : grounded_params ){
            vector< int > gp_ids;
            size_t pos = 0, last_pos = 0;
            while( (pos = gp.find(',',last_pos)) != string::npos ){
                auto s = gp.substr( last_pos, pos-last_pos );
                gp_ids.push_back( getTypeID( s ) );
                last_pos = pos + 1;
            }
            gp_ids.push_back( getTypeID( gp.substr( last_pos ) ) );
            grounded_params_ids.push_back( gp_ids );
        }
        return grounded_params_ids;
	}



    vector< vector< int > > computeGroundedObjectIDs( const vector< string > &objects,
                                                     vector< string > &grounded_params ) const{
        if( objects.empty() )
            return {};
        computeGroundedObjectParams( objects, grounded_params );
        vector< vector< int > > grounded_object_ids;
        for( const auto& gp : grounded_params ){
            vector< int > gp_ids;
            size_t pos = 0, last_pos = 0;
            while( (pos = gp.find(',',last_pos)) != string::npos ){
                auto s = gp.substr( last_pos, pos-last_pos );
                gp_ids.push_back( getTypeID( s ) );
                last_pos = pos + 1;
            }
                gp_ids.push_back( getTypeID( gp.substr( last_pos ) ) );
                grounded_object_ids.push_back( gp_ids );
        }
        return grounded_object_ids;
    }

    void computeGroundedObjectParams( const vector< string > &objects, vector< string > &grounded_params,
                                unsigned param_idx = 0, const string &current_grounding = "" ) const{
        assert( param_idx <= objects.size() );
        if( param_idx == objects.size() ) {
              // ground anything
//            grounded_params.push_back( current_grounding );
//            return;


            map<string,int> mapCount;
            for (auto object:objects){
                if (mapCount.find(object)!=mapCount.end())
                    mapCount[object]++;
                if (mapCount.find(object)==mapCount.end())
                    mapCount[object]=1;
            }
            bool flag= false;
            for (auto object:objects){
//                if (object=="output-node")
//                    continue;
                auto pointer_ids=this->getTypedPointerIDs(object);
                if (pointer_ids.size()==0){
                    continue;
                }
                else{
                    if (pointer_ids.size()>=mapCount[object]){
                        int number=0;
                        for (auto id: pointer_ids){
                            auto name= this->getPointerName(id);
                            if (current_grounding.find(name)!=std::string::npos){
                                number++;
                            }
                        }
                        if (number!=mapCount[object]) {
                            flag = true;
                            break;
                        }
                    }

                    if (pointer_ids.size()<mapCount[object]){
                        int number=0;
                        for (auto id: pointer_ids){
                            auto name= this->getPointerName(id);
                            if (current_grounding.find(name)==std::string::npos){
                                flag= true;
                                break;
                            }
                        }
                    }

                }
            }
            if (!flag)
                grounded_params.push_back( current_grounding );
            return;

        }
        auto object_ids = getObjectIDs( objects[ param_idx ] );
        vector<int> additional_ids;
//        if (objects[ param_idx ]=="output-node")
//        {
//            additional_ids= getObjectIDs("insert-flag");
//            for (auto id: additional_ids){
//                object_ids.push_back(id);
//            }
//        }
        for( auto ob_id : object_ids ){
            string p_name="";

            if (objects[ param_idx ]=="node"){
                p_name = getPointerName( ob_id );
                if( current_grounding.find(p_name) == string::npos )
                    computeGroundedObjectParams( objects, grounded_params, param_idx+1,
                                                 current_grounding + (param_idx?",":"") + getPointerName(ob_id));
            }
            else if (objects[ param_idx ]=="output-node")
            {
//                if (checkpointer(ob_id)){
                    p_name = getPointerName( ob_id );
                    if( current_grounding.find(p_name) == string::npos )
                        computeGroundedObjectParams( objects, grounded_params, param_idx+1,
                                                     current_grounding + (param_idx?",":"") + getPointerName(ob_id));
//                }
//                else{
//                    p_name = getObjectName( ob_id );
//                    if( current_grounding.find(p_name) == string::npos )
//                        computeGroundedObjectParams( objects, grounded_params, param_idx+1,
//                                                     current_grounding + (param_idx?",":"") + getObjectName(ob_id));
//                }
            }
            else{

                if (checkpointer(ob_id)){
                    p_name = getPointerName( ob_id );
                    if( current_grounding.find(p_name) == string::npos )
                        computeGroundedObjectParams( objects, grounded_params, param_idx+1,
                                                     current_grounding + (param_idx?",":"") + getPointerName(ob_id) );
                }
                else{
                    p_name = getObjectName( ob_id );
                    if( current_grounding.find(p_name) == string::npos )
                        computeGroundedObjectParams( objects, grounded_params, param_idx+1,
                                                     current_grounding + (param_idx?",":"") + getObjectName(ob_id) );
                }
            }
            // Avoid duplicate pointers as parameters (if required, add an extra pointer of the same type)
        }
    }

	void setStateVarBound( int var_id, int instance_id, int var_domain ){
	    auto var_ins = make_pair( var_id, instance_id );
	    _var_bounds[ var_ins ] = var_domain;
	}

	void setStateVarBound( const string &var_type, int instance_id, int var_domain ){
	    auto var_id = getTypeID( var_type );
        setStateVarBound( var_id, instance_id, var_domain );
	}

	int getStateVarBound( int var_id, int instance_id ) const{
	    auto var_ins = make_pair( var_id, instance_id );
	    auto it = _var_bounds.find( var_ins );
	    if( it != _var_bounds.end() )
	        return (it->second);
	    return 0;
	}

	int getStateVarBound( const string &var_type, int instance_id ) const{
	    auto var_id = getTypeID( var_type );
	    return getStateVarBound( var_id, instance_id );
	}

	bool isPointer( int id ) const{
	    return _pointer_id_to_name_idx.find( id ) != _pointer_id_to_name_idx.end();
	}

	bool isVar( int id ) const{
	    return _var_id_to_name_idx.find( id ) != _var_id_to_name_idx.end();
	}

	bool isPredicate( int id ) const{
	    return _pred_id_to_name_idx.find( id ) != _pred_id_to_name_idx.end();
	}

	bool isPointer( const string &name ) const{
	    auto id = getTypeID(name);
	    if( -1 == id ) return false;
	    return isPointer( id );
	}

	bool isPredicate( const string &name ) const{
        auto id = getTypeID(name);
        if( -1 == id ) return false;
	    return isPredicate( id );
	}

	int getBound( int id, int instance_id ) const{
		if (NUMBER){
			int bound = MAX_VAL; // default value
			if( isVar(id) )
				bound = getStateVarBound( id, instance_id );
			else if( isPointer(id) ){
				auto var_id = getPointerVarType( id );
				bound = getStateVarBound( var_id, instance_id );
			}
			return bound;
		}
		if (!NUMBER){
			int bound = MAX_VAL; // default value
			if( isVar(id) or isPredicate(id) )
				bound = getStateVarBound( id, instance_id );
			else if( isPointer(id) ){
				auto var_id = getPointerVarType( id );
				bound = getStateVarBound( var_id, instance_id );
			}
			return bound;
		}
	}

	int getBound( const string &name_type, int instance_id ) const{
        auto id = getTypeID( name_type );
        return getBound( id, instance_id );
	}

	vector< string > getPointerNames() const{
	    return _pointer_names;
	}

	vector<string> getVarTypes() const{
	    return _var_types;
	}

    vector<string> getPredicateTypes() const{
        return _predicate_types;
    }

	
	string toString() const{
		string ret = "[STATE DESCRIPTOR]:\n";
		ret += "POINTERS:\n";
		for( const auto& p : _pointer_names ){
		    auto it = _type_id.find( p );
		    assert( it != _type_id.end() );
		    ret += "\t" + p + ": " + to_string(it->second) + "\n";
		}
		ret += "VARIABLE TYPES:\n";
		for( const auto& var : _var_types ){
		    auto it = _type_id.find( var );
		    assert( it != _type_id.end() );
		    auto it_p = _typed_pointers.find( it->second );
		    assert( it_p != _typed_pointers.end() );
            ret += "\t" + var + ": with id " + to_string(it->second) + " and " +
                    (!it_p->second.empty()?"pointers:":"no pointers");
		    for( auto p_ids : (it_p->second) ){
		        ret += " " + to_string(p_ids) + ";" ;
		    }
		    ret +="\n";
		}
		ret += "PREDICATE TYPES:\n";
		for( const auto& pred : _predicate_types ){
		    auto it_pred2id = _type_id.find( pred );
		    assert(it_pred2id != _type_id.end());
		    auto it_pred2vars = _predicate_var_types.find( it_pred2id->second );
		    assert(it_pred2vars != _predicate_var_types.end() );
		    ret += "\t" + pred + ": with id " + to_string(it_pred2id->second) +
		            " and " + (!it_pred2vars->second.empty()?"variable types:":"no variable types");
		    for( auto v : (it_pred2vars->second)){
		        ret += " " + to_string(v) ;
		    }
		    ret += "\n";
		}

		return ret;
	}

private:
    // TYPE NAMES
    vector< string > _pointer_names; // POINTER NAMES
    vector< string > _var_types; // VAR NAMES
	vector< string > _predicate_types; // PREDICATE NAMES
	// TYPE to ID
    map< string, int > _type_id; // MAP OF TYPES TO IDs
    // ID to list of IDs
    map< int, vector<int> > _typed_pointers; // VAR TYPE ID -> LIST OF TYPED POINTERS IDs
	map< int, vector<int> > _predicate_var_types; // PRED TYPE ID -> LIST OF TYPED VARS IDs
	// ID to IDX (for name retrieval)
	map< int, int > _pointer_id_to_var_type; // POINTER ID -> VAR TYPE ID
	map< int, int > _pointer_id_to_typed_idx; // POINTER ID -> VAR TYPE POINTER IDX
	map< int, int > _pointer_id_to_name_idx; // POINTER ID -> NAME IDX
	map< int, int > _var_id_to_name_idx; // VAR ID -> NAME (TYPE) IDX
	map< int, int > _pred_id_to_name_idx; // PRED ID -> NAME (TYPE) IDX
	// POINTER/VAR/PRED BOUNDS FOR EACH INSTANCE
	map< pair<int,int>, int > _var_bounds; // (POINTER/VAR/PRED) ID & INSTANCE ID -> VAR BOUND
    vector <string>  _object_name;
    map<int,int> _object_id_to_name_id;
    map<int,vector<int>> _type_id_to_object_id;
    int  _new_type_id;



};

#endif
