#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "common.h"
#include "variable.h"
#include "state_descriptor.h"
#include "state.h"
#include "expression.h"

class Condition : public Expression{
public:
	// Non-parameterized horizontal_action
	explicit Condition( StateDescriptor *sd, const string &name = "",
                        Variable *lhs = nullptr, Variable *rhs = nullptr ) : Expression( name, lhs, rhs ){
	    _sd = sd;
	}

	~Condition() override = default;
	
	virtual  bool evalCondition( const State *s ) const{
		return true;
	}

	string toString( bool titled ) const override{
		string ret;
		if( titled ){
			ret += "[CONDITION]: ";
		}
		ret += "( " + _lhs->toString() + " " + _name + " " + _rhs->toString() + " )";
		if( titled ) ret+= "\n";
		return ret;
	}

protected:
    StateDescriptor *_sd;
};

class GreaterEqual : public Condition{
public:
	GreaterEqual( StateDescriptor *sd, Variable *lhs, Variable *rhs ) : Condition( sd, ">=", lhs, rhs ) {}
	
	~GreaterEqual() override = default;
	
	bool evalCondition( const State *s ) const override{
		int lhs = getLHS( _sd, s );
		int rhs = getRHS( _sd, s );
		return lhs >= rhs;
	}

}; 

class LesserEqual : public Condition{
public:
	LesserEqual( StateDescriptor *sd, Variable *lhs, Variable *rhs ) : Condition( sd, "<=", lhs, rhs ) {}
	
	~LesserEqual() override = default;
	
	bool evalCondition( const State *s ) const override{
		int lhs = getLHS( _sd, s );
		int rhs = getRHS( _sd, s );
		return lhs <= rhs;
	}

}; 
class Greater : public Condition{
public:
	Greater( StateDescriptor *sd, Variable *lhs, Variable *rhs ) : Condition( sd, ">", lhs, rhs ) {}
	
	~Greater() override = default;
	
	bool evalCondition( const State *s ) const override{
		int lhs = getLHS( _sd, s );
		int rhs = getRHS( _sd, s );
		return lhs > rhs;
	}

}; 

class Lesser : public Condition{
public:
	Lesser( StateDescriptor *sd, Variable *lhs, Variable *rhs ) : Condition( sd, "<", lhs, rhs ) {}
	
	~Lesser() override = default;
	
	bool evalCondition( const State *s ) const override{
		int lhs = getLHS( _sd, s );
		int rhs = getRHS( _sd, s );
		return lhs < rhs;
	}

};

class FunctionsEquals : public Condition{
public:
    FunctionsEquals( StateDescriptor *sd, Variable *lhs, Variable *rhs ) : Condition( sd, "@=", lhs, rhs ) {}

    ~FunctionsEquals() override = default;

    bool evalCondition( const State *s ) const override{
        int lhs = getLHS( _sd, s );
        int rhs = getRHS( _sd, s );
        return lhs == rhs;
    }

};


class Equals : public Condition{
public:
	Equals( StateDescriptor *sd, Variable *lhs, Variable *rhs ) : Condition( sd, "=", lhs, rhs ) {}

	~Equals() override = default;

	bool evalCondition( const State *s ) const override{
		int lhs = getLHS( _sd, s );
		int rhs = getRHS( _sd, s );
		return lhs == rhs;
	}

};


class Add : public Condition{
public:
	Add( StateDescriptor *sd, Variable *lhs, Variable *rhs ) : Condition( sd, "+", lhs, rhs ){}
	
	~Add() override = default;
	
	bool evalCondition( const State *s ) const override{
		int lhs = getLHS( _sd, s );
		int rhs = getRHS( _sd, s );
		auto bound = _sd->getBound( _lhs->getID(), s->getInstanceID() );
		return lhs + rhs < bound;
	}
};



class Subtract : public Condition{
public:
	Subtract( StateDescriptor *sd, Variable *lhs, Variable *rhs ) : Condition( sd, "-", lhs, rhs ) {}
	
	~Subtract() override = default;
	
	bool evalCondition( const State *s ) const override{
		int lhs = getLHS( _sd, s );
		int rhs = getRHS( _sd, s );
		return lhs >= rhs;
	}

}; 

class Different : public Condition{
public:
	Different( StateDescriptor *sd, Variable *lhs, Variable *rhs ) : Condition( sd, "!=", lhs, rhs ) {}
	
	~Different() override = default;
	
	bool evalCondition( const State *s ) const override{
		int lhs = getLHS( _sd, s );
		int rhs = getRHS( _sd, s );
		return lhs != rhs;
	}

};

#endif
