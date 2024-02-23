#ifndef __GENERALIZED_PLANNING_PROBLEM_H__
#define __GENERALIZED_PLANNING_PROBLEM_H__

#include "common.h"
#include "domain.h"
#include "instance.h"
#include "relaxed_graph.h"
#include "instruction.h"

class GeneralizedPlanningProblem{
public:
	GeneralizedPlanningProblem(){
        _domain = nullptr;
        _is_progressive = false;
	}
	
	~GeneralizedPlanningProblem(){
		delete _domain;
		for( auto instance : _instances )
			delete instance;

         _active.clear();
        _Relaxes.clear();

//        for (auto possibleactions:_possibleactions){
//            for (auto pa: possibleactions.second){
//                delete pa;
//            }
//        }
//         _possibleactions.clear();

	}
	
	void addDomain( Domain *dom ){
		_domain = dom;
	}
	
	Domain* getDomain(){
		return _domain;
	}
	
	void addInstance( Instance *ins ){
		_instances.push_back( ins );
		_active.push_back( false );
	}	
	
	Instance* getInstance( int id ){
		return _instances[ id ];
	}
	
	vector< Instance* > getInstances(){
		return _instances;
	}

   vector<Instruction*> getPossibleActions(){
        return _possibleactions;
    }
    void setPossibleActions(vector<Instruction*> actions){
        _possibleactions=actions;
    }
	
	int getNumInstances(){
		return int( _instances.size() );
	}

	void activateInstance( int id ){
	    assert( 0<= id and id < (int)_active.size() );
        //for( int i = 0; i < (int)_active.size(); i++ ) _active[i] = false;
	    _active[ id ] = true;
	}
    set<string> getRelaxes (){
        return _Relaxes;
    }
    void  setRelaxes (set<string> actions){
        _Relaxes= actions;
    }
	bool isInstanceActive( int id ){
	    assert( id < (int)_active.size() );
	    return _active[ id ];
	}

	void updatePredicateBounds(){
	    // They are updated when building the relaxed graph
	    for( auto ins : _instances ){
	        auto *rg = new RelaxedGraph( _domain, ins );
	        delete rg;
	    }
	}

    void setProgressive(bool is_progressive){
        _is_progressive = is_progressive;
    }


    bool getProgressive() const{
        return _is_progressive;
    }

    int getNumActiveInstances() const{
        auto res = count(_active.begin(), _active.end(), true);
        if( res == 0 ) return (int)_active.size();
        return res;
    }
	
	string toString(){
		string ret = "[GENERALIZED PLANNING PROBLEM]:\n";
		
		if( _domain ) 
			ret += _domain->toString();
		
		for(auto & instance : _instances)
			ret += "\n\n" + instance->toString();
		
		return ret;
	}

    vector<string> constraints_acquisition_global(){

        //  implemented during PDDL generate
        return {};
    }



	
private:
	Domain* _domain;
	vector< Instance* > _instances;
	vector< bool > _active;
    bool _is_progressive;
    set<string> _Relaxes;
    vector<Instruction*> _possibleactions;
};

#endif

