#ifndef __NODE_H__
#define __NODE_H__

#include "common.h"
#include "instruction.h"
#include "program.h"

class Node{
public:	
	explicit Node( Program *p, const vector<int> &evaluations = {0}, int id = 0 ){
		_program = p;
		_evaluations = evaluations;
		_id = id;
	}
	
	~Node(){
		delete _program;
        _evaluations.clear();
	}
	
	void setF( const vector<int> &eval ){
		_evaluations = eval;
	}

	void setID( int id ){
	    _id = id;
	}
	
	vector< int > f() const{
		return _evaluations;
	}

	int getID() const{
	    return _id;
	}
	
	Program* getProgram(){
		return _program;
	}
	
	string toString(){
		string ret = "[NODE]:";
		for( int i = 0; i < int( _evaluations.size() ); i++ ){
			ret+=" f"+to_string(i+1)+"="+to_string(_evaluations[i]);
		}
		ret+="\n";
		if( _program )
			ret += _program->toString( false );
		return ret;
	}
	
private:
	vector< int > _evaluations;
	int _id;
	Program *_program;
};

#endif
