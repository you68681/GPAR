#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include "common.h"
#include "state.h"
#include "condition.h"
#include "action.h"
#include "program_state.h"

class Instruction{
public:
	Instruction()= default;
	
	virtual ~Instruction()= default;
	
	// Maybe conds are not required, since only ZF and CF
	// are affected but already in a fix position
	virtual void addCond( Condition *c ){
		_conds.emplace_back( c );
	}
	
	virtual void setConds( vector< Condition* > &conds ){
		_conds = conds;
	}

    virtual vector< Condition* > getConds(){
        return _conds;
    }
	
	virtual bool isApplicable( ProgramState *ps ){
		return false;
	}
	
	virtual bool isGoalState( Instance *ins, ProgramState *ps ){
		return false;
	}
	
	virtual int apply( ProgramState *ps ){ return 0; }

    virtual string toString( bool full_info ) const{ return "[INSTRUCTION] EMPTY\n"; }
    virtual string getName() const { return "[INSTRUCTION] EMPTY\n"; };

protected:
	vector< Condition* > _conds;
};

class PlanningAction : public Instruction{
public:
	explicit PlanningAction( Action *act ){
		_act = act;
	}
	
	~PlanningAction() override = default;
	
	bool isApplicable( ProgramState *ps ) override{
		State *s = ps->getState();
		return _act->isApplicable( s );
	}
	
	int apply( ProgramState *ps ) override{
		State *s = ps->getState();
		int line = ps->getLine();

		int res = 0;
        {
			// planning actions
            if (_act->checkExternalFunction())
            {
                if( isApplicable( ps ) )
                res=_act->apply_external(s);
            }
			// check instructions
            if (_act->getCheckAction()){
                if (_act->getName()=="check-condition(true)")
                {
                    res=1;

                } else{
                    res=_act->apply_check(s);
                }

            }
			// other pointer operation instructions
            if (! _act->checkExternalFunction() and !_act->getCheckAction() )
            {
                if( isApplicable( ps ) )
                res = _act->apply( s );
            }
		}
		
		ps->setLine( line + 1 );
		
		return res;
	}
	
	virtual string getName() const{
		return _act->getName();
	}
    virtual string getActName() const{
        return _act->getActName();
    }
    virtual bool getChecked() const{
        return _act->getChekced();
    }

    virtual void  setCheck(bool flag){
        return _act->setChecked(flag);
    }
	virtual string getType() const{
		return _act->getType();
	}
    virtual bool getPlanningAction() const{
        return _act->getPlanningAction();
    }

    virtual bool getCheckAction() const{
        return _act->getCheckAction();
    }

    virtual bool getUninstantiatedAction() const{
        return _act->getUninstantiatedAction();
    }

    virtual bool getPointerOperation() const{
        return _act->getPointerOperation();
    }

	string toString( bool full_info ) const override{
		if( full_info )	
			return "[INSTRUCTION][PLANNING ACTION]:\n" + _act->toString(true);
		return _act->toString( full_info );
	}
    virtual string getActioVartype() const{
        return _act->getActionEffect()[0]->getVarType();
    }

    virtual vector<int> getActioParamIndexVector() const{
        return _act->getActionParaIndexVector();
    }





private:
	Action *_act;
};

class Goto : public Instruction{
public:
	explicit Goto( int dest = 0 ){
		_dest_line = dest;
	}
	
	~Goto() override = default;
	
	int getDestinationLine() const{
		return _dest_line;
	}
    void setDestinationLine(int line){
        _dest_line=line;
    }

    int getVariable() const{
        return _variables;
    }
    void setVariable(int variables){
        _variables=variables;
    }


    virtual vector< Condition* > getConds(){
        return _conds;
    }

	// apply goto instruction
	int apply( ProgramState *ps ) override{
		State *s = ps->getState();
		int line = ps->getLine();
		
		bool evals_true = true;
		for( int i = 0; evals_true && i < int( _conds.size() ); i++ ){
			evals_true = _conds[ i ]->evalCondition( s );
		}
		
		if( evals_true ) ps->setLine( line + 1 );
		else ps->setLine( _dest_line );
		
		return 0;
	}
	
	string toString( bool full_info ) const override{
		string ret;
		if( full_info ){
			ret = "[INSTRUCTION][GOTO]:\n"; 
			ret += "DESTINATION LINE: " + to_string( _dest_line ) + "\n";
		}
		else{
			ret = "goto(" + to_string( _dest_line ) + ",!(";
		}
		
		for(auto cond : _conds)
			ret += cond->toString( full_info );
			
		if( !full_info ) ret += "))\n";
		return ret;
	}
    virtual string getName() const{
        return "goto";
    }
	
private:
	int _dest_line;
    int _variables=-1;
    bool _active=false;
};

class End : public Instruction{
public:
	End()= default;
	
	~End() override =default;
	
	bool isGoalState( Instance *ins, ProgramState *ps ) override{
		State *s = ps->getState();
		vector< Condition* > goal = ins->getGoalCondition();
		for(auto & g : goal){
			if( !g->evalCondition( s ) )
				return false;
		}
		
		return true;
	}
	
	string toString( bool full_info ) const override{
		string ret = "end\n";
		if( full_info ){
			ret= "[INSTRUCTION][END]:\n";
			for(auto cond : _conds)
				if( cond )
					ret += cond->toString(true);
		}
		return ret;
	}
    virtual string getName() const{
        return "end";
    }
	
};

#endif
