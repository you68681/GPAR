#ifndef __OPERATION_H__
#define __OPERATION_H__

#include "common.h"
#include "state.h"
#include "expression.h"

class Operation : public Expression{
public:
	// Non-parameterized horizontal_action
	explicit Operation( StateDescriptor *sd, const string &name = "",
                        Variable *lhs = nullptr, Variable *rhs = nullptr ) : Expression( name, lhs, rhs ) {
	    _sd = sd;
	}

	virtual ~Operation()= default;
	
	int update( State *s ) const override{
		int effect = getEffect( s );
		setEffect( s, effect ); 
		return effect; 
	}
	
	virtual int getEffect( State *s ) const{ return 0; }
	
	virtual void setEffect( State *s, int val ) const{
		if( val >= 0 )
			setLeftVarNum(_sd, s, val);
	}

    virtual int update_color( State *s, int val ){

        return Expression::update_color(_sd, s, val);
    }

    virtual int swap_color( State *s, int val ){

        return Expression::swap_color(_sd, s, val);
    }

    virtual int copy_color( State *s, int val ){

        return Expression::copy_color(_sd, s, val);
    }


    virtual int rotate_node( State *s, int val ) {
        return Expression::rotate_node(_sd, s, val);
    }


    virtual int mirror( State *s, int val ) {
        return Expression::mirror(_sd, s, val);
    }

    virtual int flip(State *s, int val){
        return Expression::flip(_sd, s, val);
    }

    virtual int hollow_rectangle(State *s, int val){
        return Expression::hollow_rectangle(_sd, s, val);
    }

    virtual int fill_rectangle(State *s, int val){
        return Expression::fill_rectangle(_sd, s, val);
    }


    virtual int insert_input_node(State *s, int val){
        return Expression::insert_input_node(_sd, s, val);
    }

    virtual int insert_output_node(State *s, int val){
        return Expression::insert_output_node(_sd, s, val);
    }


    virtual int move_node_direction( State *s, int val ){

        return Expression::move_node_direction(_sd, s, val);
    }

    virtual int move_node_max( State *s, int val ){

        return Expression::move_node_max(_sd, s, val);
    }

    virtual int move_node_max_direction( State *s, int val ){

        return Expression::move_node_max_direction(_sd, s, val);
    }


    virtual int move_between_nodes_direction( State *s, int val ){

        return Expression::move_between_nodes_direction(_sd, s, val);
    }

    virtual int add_border( State *s, int val ){

        return Expression::add_border(_sd, s, val);
    }

    virtual int extend_node( State *s, int val ){

        return Expression::extend_node(_sd, s, val);
    }

    virtual int extend_node_direction( State *s, int val ){

        return Expression::extend_node_direction(_sd, s, val);
    }




    virtual bool check ( State *s ) {
        return Expression::check(_sd, s);
    }

    string toString( bool titled = true ) const override {
		string ret = "";
		if( titled ){
			ret += "[OPERATION]: ";
		}
		ret += "( " + _lhs->toString() + " " + _name + " " + _rhs->toString() + " )\n";
		return ret;
	}

protected:
    StateDescriptor *_sd;
};

class AddAssign : public Operation{
public:
	AddAssign( StateDescriptor *sd, Variable *lhs = 0, Variable *rhs = 0 ) : Operation( sd, "+=", lhs, rhs ) {}
	
	virtual ~AddAssign(){}
	
	/*virtual int update( State *s ) const{
		int effect = getEffect( s );
		setEffect( s, effect );
		return effect;
	}*/
	
	virtual int getEffect( State *s ) const{
		int lhs = getLHS( _sd, s );
		int rhs = getRHS( _sd, s );
		return lhs + rhs;
	}

}; 

class SubtractAssign : public Operation{
public:
	SubtractAssign( StateDescriptor *sd, Variable *lhs = 0, Variable *rhs = 0 ) : Operation( sd, "-=", lhs, rhs ) {}
	
	virtual ~SubtractAssign(){}
	
	virtual int getEffect( State *s ) const{
		int lhs = getLHS( _sd, s );
		int rhs = getRHS( _sd, s );
		return lhs - rhs;
	}

}; 

class Compare : public Operation{
public:
	Compare( StateDescriptor *sd, Variable *lhs = 0, Variable *rhs = 0 ) : Operation( sd, "-", lhs, rhs ) {}
	
	virtual ~Compare(){}
	
	virtual int getEffect( State *s ) const{
		int lhs = getLHS( _sd, s );
		int rhs = getRHS( _sd, s );
		assert( lhs >= 0 and rhs >= 0);
		return lhs - rhs;
	}
	
	virtual void setEffect( State *s, int val ) const{
		// no updates for cmp
	}
};
class FunctionsAssign : public Operation{
public:
    FunctionsAssign( StateDescriptor *sd, Variable *lhs = 0, Variable *rhs = 0 ) : Operation( sd, "@=", lhs, rhs ) {}

    virtual ~FunctionsAssign(){}

    virtual int getEffect( State *s ) const{
        //int lhs = getLHS( _sd, s );
        int rhs = getRHS( _sd, s );
        return rhs;
    }

};

class Assign : public Operation{
public:
	Assign( StateDescriptor *sd, Variable *lhs = 0, Variable *rhs = 0 ) : Operation( sd, "=", lhs, rhs ) {}

	virtual ~Assign(){}

	virtual int getEffect( State *s ) const{
		//int lhs = getLHS( _sd, s );
		int rhs = getRHS( _sd, s );
		return rhs;
	}

};



// ToDo test this class
class TestMax : public Operation{
public:
    TestMax( StateDescriptor *sd, Variable *lhs = 0, Variable *rhs = 0 ) : Operation( sd, "?", lhs, rhs ){}

    virtual ~TestMax(){}

    virtual  int getEffect( State *s ) const{
        int lhs = getLHS( _sd, s );
        auto bound = _sd->getBound( _lhs->getID(), s->getInstanceID() );
        return bound - (lhs+1);
    }

    virtual void setEffect( State *s, int val ) const{
        // no updates for test
    }
};

#endif
