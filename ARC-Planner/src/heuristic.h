#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#include "common.h"
#include "instruction.h"
#include "program.h"
#include "program_state.h"
#include "generalized_planning_problem.h"
#include "landmark_graph.h"

class Heuristic{
public:
	explicit Heuristic( const string &name = "heuristic" ) : _name( name ){
	}
	
	virtual ~Heuristic()= default;
	
	virtual string getName() const{
		return _name;
	}
	
	// Program-based or Performance-based heuristic
	virtual int compute( Program *p, GeneralizedPlanningProblem *gpp  ){
		return 0;
	}
private:
	string _name;
};




class Hpluscheck : public Heuristic{
public:
    Hpluscheck() : Heuristic( "hcn" ){}

    virtual ~Hpluscheck(){}
    // varient of h_cn introduced in the Novelty and Lifted Helpful Actions in Generalized Planning
    virtual int compute( Program *p, GeneralizedPlanningProblem *gpp = NULL ){
        int res=0;
        int problem_size=1000;
        int W = 1; // default value
        int check_number=W*p->getNumOfcheck();
        res += problem_size-check_number;
        return res;

    }
};





class Hpluscount : public Heuristic{
public:
    Hpluscount() : Heuristic( "hp" ){}

    virtual ~Hpluscount(){}

    // h_p introduced in the paper Generalized Planning for the Abstraction and Reasoning Corpus
    virtual int compute( Program *p, GeneralizedPlanningProblem *gpp = NULL){
        return p->getNumOfgoal();
    }
};


class Hplusline : public Heuristic{
public:
    Hplusline() : Heuristic( "hln" ){}

    virtual ~Hplusline(){}

    // h_ln introduced in the Novelty and Lifted Helpful Actions in Generalized Planning
    virtual int compute( Program *p, GeneralizedPlanningProblem *gpp = NULL ){
        int res = 0;
        vector< Instruction* > instructions = p->getInstructions();
        for( int i = 0; i < int( instructions.size() ); i++ ){
            if( instructions[ i ] != NULL){
                PlanningAction *a = dynamic_cast< PlanningAction* >( instructions[ i ] );
                if (a and a->getName().find("test")==std::string::npos and a->getName().find("cmp")==std::string::npos and a->getName().find("check")==std::string::npos){
                    res++;
                }
            }
        }
        return res;
    }

};




#endif
