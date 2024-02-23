#ifndef __STATE_H__
#define __STATE_H__

#include "common.h"
#include "state_descriptor.h"
#include "abstractgraph.h"
#include "pointer.h"
//auto hash_state = [](const vector<int>& vec){return vec[0]*1000 + vec[1];};
//auto equal_state = [](const vector<int>& l, const vector<int>& r){return hash_state(l) == hash_state(r);};

//int hash_state(const vector<int>& vec) {
//    return vec[0]*1000 + vec[1];
//}
//
//bool equal_state(const vector<int>& l, const vector<int>& r){
//    return hash_state(l) == hash_state(r);
//}

class State{
public:
    // Default constructor
    State(){
        _instance_id = 0;
    }

    // Constructor mainly used for initial states
	explicit State( StateDescriptor *sd, int instance_id = 0 ){
//        auto var_types = sd->getVarTypes();
//        _typed_pointers.resize( var_types.size() );
//        for( int i = 0; i < int( var_types.size() ); i++ ){
//            auto num_typed_pointers = sd->getNumTypedPointers( var_types[i] );
//            _typed_pointers[i].resize( num_typed_pointers, 0 );
//        }

        for (auto& name: sd->getPointerNames()){
            _pointer_map[sd->getTypeID(name)]=0;
        }

//        _registers_map={};


        auto pred_types = sd->getPredicateTypes();
        _typed_registers.resize( pred_types.size() );
        _instance_id = instance_id;
        _graph= nullptr;
	}

	// Constructor mainly used to make copies and propagate effects
	explicit State( State *s ){
//		_typed_pointers = s->getTypedPointers();
		_typed_registers = s->getTypedRegisters();
		_instance_id = s->getInstanceID();
        _obj_to_address=s->getObjAddress();
        _obj_to_type=s->getObjType();
        _pointer_to_node=s->getPointertoNode();
        _pointer_to_color=s->getPointertoColor();
        _pointer_to_size=s->getPointertoSize();
        _pointer_to_shape=s->getPointertoShape();
        _pointer_to_direction=s->getPointertoDirection();
         _pointer_to_direction_diagonal=s->getPointertoDirectionDiagonal();

        _pointer_to_output_node=s->getPointertoOutputNode();

        _graph=s->getGraph()->copy();
        _pointer_map=s->getPointer();
//        _registers_map=s->getRegister();

	}

    ~State(){

        _pointer_map.clear();
//    unordered_map<int,int> _registers_map;

         _typed_pointers.clear(); // VarType -> {pointer1 = value1; ...; pointerN = valueN }
        _typed_registers.clear(); // PredType ( size = |Obj1| x ... x |ObjM|;  or size=1 for 0-ary)
//    vector< std::unordered_map< vector< int >, int, decltype(hash_state), decltype(equal_state)> > _typed_registers; // PredType ( size = |Obj1| x ... x |ObjM|;  or size=1 for 0-ary)

         _obj_to_address.clear();
        _obj_to_type.clear();


       _pointer_to_node.clear();

        _pointer_to_color.clear();

        _pointer_to_size.clear();

        _pointer_to_shape.clear();

        _pointer_to_direction.clear();

        _pointer_to_direction_diagonal.clear();

        _pointer_to_output_node.clear();


        delete _graph;


    }

	State* copy() {
		return new State( this );
	}

//	vector< vector< int > > getTypedPointers() const{
//	    return _typed_pointers;
//	}

	vector< map< vector<int>, int > > getTypedRegisters() const{
	    return _typed_registers;
	}



//	void setPointersSize( unsigned p_size ){
//        _typed_pointers.resize( p_size );
//    }

    void setRegistersSize( unsigned r_size ){
        _typed_registers.resize( r_size );
    }

//    void addPointer( StateDescriptor *sd, const string &var_type, const string &pointer_name ){
//        bool add = sd->addPointer( var_type, pointer_name );
//        assert( add );
//        auto var_idx = sd->getVarIDX( var_type );
//        assert( var_idx < (int)_typed_pointers.size() );
//        _typed_pointers[ var_idx ].resize( sd->getNumTypedPointers( var_type ) );
//    }

//	void setPointer( StateDescriptor *sd, const string &var_type, int idx, int value ){
//        auto var_idx = sd->getVarIDX( var_type );
//        assert( var_idx < (int)_typed_pointers.size() );
//        assert( idx < (int)_typed_pointers[ var_idx ].size() );
//        _typed_pointers[ var_idx ][ idx ] = value;
//    }

    void setPointer(StateDescriptor *sd,int id,int value){
        _pointer_map[id]=value;
    }

//    void setPointer(StateDescriptor *sd,string name,int value){
//        _pointer_map[sd->getTypeID(name)]=value;
//    }


    int getPointer(StateDescriptor *sd,int id) const{
        auto iter = _pointer_map.find(id);
        assert(iter!=_pointer_map.end());
        return iter->second;
    }

//
//    int getPointer(StateDescriptor *sd,string name) const{
//        int id=sd->getTypeID(name);
//        auto iter = _pointer_map.find(id);
//        assert(iter!=_pointer_map.end());
//        return iter->second;
//    }


    unordered_map<int,int> getPointer() const{
        return _pointer_map;
    }

//    unordered_map<int,int> getRegister() const{
//        return _registers_map;
//    }


    void setGraph( AbstractGraph *graph ){
        _graph = graph;
    }

//
//    void setPointer( StateDescriptor *sd, int pointer_id, int value ){
//        auto var_id = sd->getPointerVarType( pointer_id );
//        string var_type = sd->getVarName( var_id );
//        auto pointer_idx = sd->getPointerTypedIDX( pointer_id );
//        setPointer(sd, var_type, pointer_idx, value );
//    }
//
    void setPointer( StateDescriptor *sd, const string &pointer_name, int value ) {
        auto pointer_id = sd->getTypeID( pointer_name );
        setPointer( sd, pointer_id, value );
    }




    void add_pointer_to_obj( const int &pointer_value, const int &node_type_id){
        _pointer_to_node[pointer_value]=node_type_id;
    }


    void add_pointer_to_output_obj( const int &pointer_value, const int &node_type_id){
        _pointer_to_output_node[pointer_value]=node_type_id;
    }


    void add_pointer_to_color( const int &pointer_value, const int &color_type_id){
        _pointer_to_color[pointer_value]=color_type_id;
    }

    void add_pointer_to_size( const int &pointer_value, const int &size_type_id){
        _pointer_to_size[pointer_value]=size_type_id;
    }
    void add_pointer_to_shape( const int &pointer_value, const int &shape_type_id){
        _pointer_to_shape[pointer_value]=shape_type_id;
    }


    void add_pointer_to_direction( const int &pointer_value, const int &direction_id){
        _pointer_to_direction[pointer_value]=direction_id;
    }

    void add_pointer_to_direction_diagonal( const int &pointer_value, const int &direction_id){
        _pointer_to_direction_diagonal[pointer_value]=direction_id;
    }



//    vector<vector<int>> getTypedPointers(){
//        return  _typed_pointers;
//    }



    AbstractGraph* getGraph() const{
        return _graph;
    }

//
//    int getPointer( StateDescriptor *sd, int pointer_id ) const {
//        auto var_id = sd->getPointerVarType( pointer_id );
//        string var_type = sd->getVarName( var_id );
//        auto pointer_idx = sd->getPointerTypedIDX( pointer_id );
//        return getPointer( sd, var_type, pointer_idx );
//    }

    int getPointer( StateDescriptor *sd, const string &pointer_name ) const {
        auto pointer_id = sd->getTypeID( pointer_name );
        return getPointer( sd, pointer_id );
    }

    int getObject(StateDescriptor *sd, int object_id) const {
        string object_name=sd->getObjectName(object_id);
        int object_value= getObjectValue(object_name);
        if (object_value==-1){
            return object_id;
        }
        else{
            return object_value;
        }

    }


    // In "var_obj" is encoded the object ids
    void addRegister( StateDescriptor *sd, const string &pred_type, const vector<int> &var_obj_idx, int value = 1 ){
        auto var_id_list = sd->getPredicateVarTypeIDs( pred_type );
        auto pred_idx = sd->getPredicateIDX( pred_type );
        assert( pred_idx < (int)_typed_registers.size() );
        _typed_registers[ pred_idx ][ var_obj_idx ] = value;
    }

//    void addRegister( StateDescriptor *sd, const string &pred_type, const vector<int> &var_obj_idx, int value = 1 ){
//        auto pred_idx = sd->getPredicateIDX( pred_type );
//        int key= simpleHash(pred_idx,var_obj_idx);
//         _registers_map[key]=value;
//    }


    // In "value" is encoded the var-object binding
    /*void delRegister( StateDescriptor *sd, const string &pred_type, const vector<int> &value ){
        auto pred_idx = sd->getPredicateIDX( pred_type );
        assert( pred_idx < (int)_typed_registers.size() );
        auto it = _typed_registers[ pred_idx ].find( value );
        if( it != _typed_registers[ pred_idx ].end() )
            _typed_registers[ pred_idx ].erase( it );
    }*/

    int getRegister( StateDescriptor *sd, const string &pred_type, const vector<int> &var_obj_idx ) const{
        auto pred_idx = sd->getPredicateIDX( pred_type );
        assert( pred_idx < (int)_typed_registers.size() );
        auto it = _typed_registers[ pred_idx ].find( var_obj_idx );
        if( it == _typed_registers[ pred_idx ].end() )
            return 0;
        return (it->second);
    }

//    int getRegister( StateDescriptor *sd, const string &pred_type, const vector<int> &var_obj_idx ) const{
//
//        auto pred_idx = sd->getPredicateIDX( pred_type );
//        int key= simpleHash(pred_idx,var_obj_idx);
//        auto iter = _registers_map.find(key);
//        if (iter==_registers_map.end())
//            return 0;
//        return iter->second;
//    }



	vector< vector< int > > getStateVars() const{
        vector< vector< int > > state_vars = _typed_pointers;
//        vector< vector< int > > state_vars;
//        std::copy(_typed_pointers.begin(), _typed_pointers.end(), state_vars.end());
		for( const auto& pred_regs : _typed_registers ) {
		    for( const auto& sreg : pred_regs) {
		        // add only strictly positive valued predicates
		        if( sreg.second == 0 )
		            continue;
		        state_vars.emplace_back( sreg.first );
		        state_vars[ int( state_vars.size() )-1 ].emplace_back( sreg.second );
            }
        }
		return state_vars;
	}
	
//	int size() const{
//	    int res = 0;
//	    for( const auto& tp : _typed_pointers )
//	        res += int(tp.size());
//	    for( const auto& tr : _typed_registers )
//	        res += int(tr.size());
//	    return res;
//	}

	string toString() const{
		string ret = "[STATE]:";
		for( unsigned i = 0; i< _typed_pointers.size(); i++ ) {
            ret += "\nPOINTERS #" + to_string(i) +":";
            for( const auto p : _typed_pointers[i] ){
                ret += " " + to_string(p);
            }
        }
		for( unsigned i = 0; i < _typed_registers.size(); i++ ){
            ret += "\nREGISTERS #" + to_string(i) + ":";
            for( const auto& sv : _typed_registers[ i ] ){
                ret += " (";
                for( int k = 0; k < int( sv.first.size()); k++ ){
                    ret += (k?",":"") + to_string( sv.first[k] );
                }
                ret += ")=" + to_string( sv.second );
            }
		}
		ret += "\n";
		return ret;
	}

	string toString( StateDescriptor *sd ){//, map< int, int > & var_id_to_num_objects ){
		string ret = "[STATE]:\n";

		vector< string > pointer_names = sd->getPointerNames();
		vector< string > var_types = sd->getVarTypes();
		vector< string > pred_types = sd->getPredicateTypes();

		for( unsigned i = 0; i < var_types.size(); i++ ){
            vector< int > pointer_ids = sd->getTypedPointerIDs( var_types[i] );
            for( unsigned j = 0; j < pointer_ids.size(); j++ ){
                int p_idx = sd->getPointerIDX( pointer_ids[j] );
                ret += "( " + var_types[i] + " " + pointer_names[ p_idx ] + ": " + to_string(_typed_pointers[i][j]) + " )\n";
            }
		}

		for( unsigned i = 0; i < pred_types.size(); i++ ){
		    ret += "+ " + pred_types[i] + "(";
		    vector< string > v_names = sd->getPredicateVarNames( pred_types[ i ] );
		    for( unsigned j = 0; j < v_names.size(); j++ ){
		        ret += (j?",":"") + v_names[j];
		    }
		    ret += "):";
		    for( const auto& sv : _typed_registers[ i ] ){
		        ret += " (";
		        for( int k = 0; k < int( sv.first.size()); k++ ){
		            ret += (k?",":"") + to_string( sv.first[k] );
		        }
                ret += ")="+ to_string(sv.second);
		    }
		    ret += "\n";
		}
		return ret;
	}

	// ToDo test the next two functions
	void setInstanceID( int ins_id ){
        _instance_id = ins_id;
    }

	int getInstanceID() const{
        return _instance_id;
    }


    unordered_map<string,int> getObjAddress() const{
        return _obj_to_address;
    }

    void addObjAddress(string name, int number){
        _obj_to_address[name]=number;
    }
    void addObjType(string name,string type){
        _obj_to_type[name]=type;
    }

    unordered_map<string,string> getObjType() const{
        return _obj_to_type;
    }

    unordered_map<int,int> getPointertoNode() const{
        return _pointer_to_node;
    }

    unordered_map<int,int> getPointertoOutputNode() const{
        return _pointer_to_output_node;
    }

    unordered_map<int,int> getPointertoColor() const{
        return _pointer_to_color;
    }


    unordered_map<int,int> getPointertoSize() const{
        return _pointer_to_size;
    }


    unordered_map<int,int> getPointertoDirection() const{
        return _pointer_to_direction;
    }

    unordered_map<int,int> getPointertoDirectionDiagonal() const{
        return _pointer_to_direction_diagonal;
    }


    unordered_map<int,int> getPointertoShape() const{
        return _pointer_to_shape;
    }


    int getNodeID(const int&pointer_value) const{
        auto it=_pointer_to_node.find(pointer_value);
        assert(it!=_pointer_to_node.end());
        return it->second;
    }


    int getOutputNodeID(const int&pointer_value) const{
        auto it=_pointer_to_output_node.find(pointer_value);
        assert(it!=_pointer_to_output_node.end());
        return it->second;
    }


    int getColorID(const int&pointer_value) const{
        auto it=_pointer_to_color.find(pointer_value);
        assert(it!=_pointer_to_color.end());
        return it->second;
    }


    int getSizeID(const int&pointer_value) const{
        auto it=_pointer_to_size.find(pointer_value);
        assert(it!=_pointer_to_size.end());
        return it->second;
    }

    int getShapeID(const int&pointer_value) const{
        auto it=_pointer_to_shape.find(pointer_value);
        assert(it!=_pointer_to_shape.end());
        return it->second;
    }


    int getDirectionID(const int&direction_value) const{
        auto it=_pointer_to_direction.find(direction_value);
        assert(it!=_pointer_to_direction.end());
        return it->second;
    }

    int getDirectionDiagonalID(const int&direction_value) const{
        auto it=_pointer_to_direction_diagonal.find(direction_value);
        assert(it!=_pointer_to_direction_diagonal.end());
        return it->second;
    }


    int getObjectValue(string name) const {
        auto it_p=_obj_to_address.find(name);
        if (it_p==_obj_to_address.end()){
            return -1;
        }
        else{
         return it_p->second;
        }

    }

//    int simpleHash(int predicate_id, const vector<int> &var_obj_idx ) const{
//        int key;
//        key=(predicate_id+1)*10000000;
//        key=key+var_obj_idx[0]*10000;
//        key=key+var_obj_idx[1]*10;
//        return key;
//
//    }


private:

    unordered_map<int,int> _pointer_map;
//    unordered_map<int,int> _registers_map;

    vector< vector< int > > _typed_pointers; // VarType -> {pointer1 = value1; ...; pointerN = valueN }
    vector< map< vector< int >, int> > _typed_registers; // PredType ( size = |Obj1| x ... x |ObjM|;  or size=1 for 0-ary)
//    vector< std::unordered_map< vector< int >, int, decltype(hash_state), decltype(equal_state)> > _typed_registers; // PredType ( size = |Obj1| x ... x |ObjM|;  or size=1 for 0-ary)

    unordered_map<string,int> _obj_to_address;
    unordered_map< string, string > _obj_to_type;

    int _instance_id;

    unordered_map<int,int> _pointer_to_node;

    unordered_map<int,int> _pointer_to_output_node;

    unordered_map<int,int> _pointer_to_color;

    unordered_map<int,int> _pointer_to_size;

    unordered_map<int,int> _pointer_to_shape;

    unordered_map<int,int> _pointer_to_direction;

    unordered_map<int,int> _pointer_to_direction_diagonal;


    AbstractGraph *_graph;



};

#endif
