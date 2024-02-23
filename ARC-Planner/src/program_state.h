#ifndef __PROGRAM_STATE_H__
#define __PROGRAM_STATE_H__

#include "common.h"
#include "state.h"

class ProgramState{
public:
	ProgramState(){
		_state = nullptr;
		_line = 0;
	}
	
	~ProgramState(){
		delete _state;
	}
	
	void setState( State *s ){
		_state = s;
	}
	
	void setLine( int l ){
		_line = l;
	}

	
	State* getState() const{
		return _state;
	}
	
	int getLine() const{
		return _line;
	}


	vector< int > asVector() const{
        vector< int > res( 1, _line );
        auto vars = _state->getStateVars();
        for( auto &v : vars ){
            res.insert( res.end(), v.begin(), v.end() );
        }
        return res;
	}


	string toString() const{
		string ret = "[PROGRAM STATE]:\nLINE:" + to_string( _line ) + "\n";
		if( _state )
			ret += _state->toString();
		return ret;
	}

	string toString( StateDescriptor *sd ) const{
        string ret = "[PROGRAM STATE]:\nLINE:" + to_string( _line ) + "\n";
        if( _state )
            ret += _state->toString(sd);
        return ret;
	}
	
private:
	State *_state;
	int _line;

};

#endif
