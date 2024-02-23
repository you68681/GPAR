#ifndef __RELAXED_ACTIONS_H__
#define __RELAXED_ACTIONS_H__

#include "state_descriptor.h"
#include "relaxed_state.h"
#include "action.h"

// A Relaxed Action is an Action grounded with the objects of the instance
// that only add facts to a Relaxed State
class RelaxedAction : public  Action {
public:
    explicit RelaxedAction( StateDescriptor *sd, const string &name = "",
                            const string &atype = "memory") : Action(name, atype){
        int num_of_predicates = (int)sd->getPredicateTypes().size();
        _prec_facts.resize( num_of_predicates );
        _eff_facts.resize( num_of_predicates );
    }

    ~RelaxedAction() override = default;

    void setParamType( vector<int> &param_types ){
        _param_types = param_types;
    }

    vector<int> getParamTypes() const{
        return _param_types;
    }

    void setGroundedParams( vector<int> &grounded_params ){
        _grounded_params = grounded_params;
    }

    vector<int> getGroundedParams() const{
        return _grounded_params;
    }

    void addPrecondition(int pred_idx, const vector<int> &objs, int value ){
        _prec_facts[ pred_idx ][ objs ] = value;
    }

    vector< map< vector< int >, int > > getPreconditionFacts() const{
        return _prec_facts;
    }

    void addEffect(int pred_idx, const vector<int> &objs, int value){
        _eff_facts[ pred_idx ][ objs ] = value;
    }

    vector< map< vector< int >, int > > getEffectFacts() const{
        return _eff_facts;
    }

    bool isApplicable( unique_ptr<RelaxedState> &s ){
        for( int pred_idx = 0; pred_idx < (int)_prec_facts.size(); pred_idx++ ){
            for( const auto& m : _prec_facts[ pred_idx ] ){
                if( not s->hasFact( pred_idx, m.first, m.second) )
                    return false;
            }
        }
        return true;
    }

    // Check if the action is adding a specific fact
    bool addsFact( int pred_idx, const vector<int> &objs, int value ){
        assert( (unsigned)pred_idx < _eff_facts.size() );
        auto it = _eff_facts[ pred_idx ].find( objs );
        if( it == _eff_facts[ pred_idx ].end() ) return false;
        return (it->second) == value;
    }

    void apply( unique_ptr<RelaxedState> &s ){
        for( int pred_idx = 0; pred_idx < (int)_eff_facts.size(); pred_idx++ ){
            for( const auto& m : _eff_facts[ pred_idx ] ){
                s->addFact( pred_idx, m.first, m.second );
            }
        }
    }

    string toString( StateDescriptor *sd ) const{
        string output = "[RELAXED-ACTION]: " + getName() + "\nPRECONDITIONS:";
        auto pred_names = sd->getPredicateTypes();
        for( unsigned pred_id = 0; pred_id < _prec_facts.size(); pred_id++ ){
            if( _prec_facts[ pred_id ].empty() ) continue;
            for( const auto& m : _prec_facts[ pred_id ] ){
                output += "\n" + pred_names[ pred_id ] + "(";
                for( unsigned o = 0; o < m.first.size(); o++ )
                    output += (o?",":"") + to_string( m.first[ o ] );
                output += ") = " + to_string( m.second );
            }
        }
        output += "\nEFFECTS:";
        for( unsigned pred_id = 0; pred_id < _eff_facts.size(); pred_id++ ){
            if( _eff_facts[ pred_id ].empty() ) continue;
            for( const auto& m : _eff_facts[ pred_id ] ){
                output += "\n" + pred_names[ pred_id ] + "(";
                for( unsigned o = 0; o < m.first.size(); o++ )
                    output += (o?",":"") + to_string( m.first[ o ] );
                output += ") = " + to_string( m.second );
            }
        }
        output += "\n";
        return output;
    }

private:
    vector<int> _grounded_params;
    vector<int> _param_types;
    vector< map< vector< int >, int > > _prec_facts;
    vector< map< vector< int >, int > > _eff_facts;

};
#endif //__ACTIONS_H__
