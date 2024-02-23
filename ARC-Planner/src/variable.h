#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include "common.h"

// Counters and Pointers represented as variables, 
// where only the latters have access to registers
class Variable{
public:
	explicit Variable( const string &name = "1", const string &vtype = "constant",
              int id = 0, const vector< int > &param_ids = {} ){
        assert(_param_ids.empty() or (!_param_ids.empty() and vtype == "predicate" ) );
        assert( vtype == "constant" or vtype == "pointer" or vtype == "predicate" or vtype == "external" );
		_name = name;
		_vtype = vtype;
		_id = id;
		_param_ids = param_ids;
		//if( (name[0] == '*' and vtype == "pointer") or vtype == "predicate")
		if( vtype == "predicate" )
		    _access_memory = true;
		else _access_memory = false;
	}
	
	~Variable()= default;
	
	void setName( const string &name = "1" ){
		_name = name;
	}

	void setVType( const string &vtype = "constant" ){
		_vtype = vtype;
	}
	
	void setID( int id ) {
		_id = id;
	}

	void setParameterIDs( vector< int > &params ){
	    _param_ids = params;
	}
	
	void setAccessMemory( bool am ){
		_access_memory = am;
	}
	
	string getName() const{
		return _name;
	}
	
	string getVType() const{
		return _vtype;
	}
	
	int getID() const{
		return _id;
	}

	vector<int> getParameterIDs() const{
	    return _param_ids;
	}
	
	bool accessMemory() const{
		return _access_memory;
	}
	
	string toString( bool info = false ) const{
		string ret;
		if( info )
			ret = "[VARIABLE]: " + _vtype + " " ;
		ret += _name;
		if( info ){
			ret += " with id: " + to_string( _id );
			if( !_param_ids.empty() ){
			    ret += "(";
			    for( unsigned i = 0; i < _param_ids.size(); i++ )
			        ret += (i?",":"") + to_string( _param_ids[i]);
			    ret +=")";
			}
		}
		return ret;
	}
	
private:
	string _name; // full name, e.g. on(i,j), i, *i or 10
	string _vtype; // predicate, pointer or constant
	int _id; // id of the first symbol
	vector< int > _param_ids; // list of pointer ids or constants (only for predicates)
	bool _access_memory;
};

#endif