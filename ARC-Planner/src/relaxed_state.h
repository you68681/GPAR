#ifndef __RELAXED_STATE_H__
#define __RELAXED_STATE_H__

#include "state_descriptor.h"

class RelaxedState{
public:
    explicit RelaxedState( StateDescriptor *sd ){
        _sd = sd;
        _facts.resize( sd->getPredicateTypes().size() );
        _instance_id = 0;
    }

    ~RelaxedState() = default;

    int getMaxVal( int pred_idx ){
        assert( (int)_facts.size() > pred_idx );
        int max_val = 0;
        for( const auto& m : _facts[ pred_idx ] ) {
            auto max_elem = max_element( m.second.begin(), m.second.end() );
            max_val = max( max_val, (*max_elem) );
        }
        return max_val;
    }

    void addFact( int pred_idx, const vector< int > &objs, int value ){
        _facts[ pred_idx ][ objs ].insert( value );
    }

    bool hasFact( int pred_idx, const vector< int > &objs, int value ){
        auto it = _facts[ pred_idx ].find( objs );
        // grounding for predicate at pred_idx not found
        if( it == _facts[ pred_idx ].end() ) {
            addFact( pred_idx, objs, 0 ); // Add a 0/false value
            return ( value == 0 );
        }
        return ( (it->second).find( value ) != (it->second).end() );
    }

    vector< map< const vector< int >, set<int> > > getFacts() const {
        return _facts;
    }

    void setInstanceID( int instance_id ){
        _instance_id = instance_id;
    }

    int getInstanceID() const{
        return _instance_id;
    }

    int getNumFacts() const{
        int total = 0;
        for(const auto & fact : _facts){
            for( const auto& m : fact ){
                total += int( m.second.size() );
            }
        }
        return total;
    }

    string toString() const{
        string output = "[RELAXED-STATE]:";
        auto pred_names = _sd->getPredicateTypes();
        for( unsigned pred_idx = 0; pred_idx < _facts.size(); pred_idx++ ){
            string pred_name = pred_names[ pred_idx ];
            for( const auto& m : _facts[ pred_idx ] ){
                //auto objects = m.first;
                auto values = m.second;
                output += "\n" + pred_name + "(";
                for( unsigned o_idx = 0; o_idx < m.first.size(); o_idx++ ){
                    output += (o_idx?",":"") + to_string(m.first[ o_idx ]); // add object
                }
                output += ")={";
                for( auto it = m.second.begin(); it != m.second.end(); it++ ){
                    output += (it!=m.second.begin()?",":"") + to_string((*it));
                }
                output += "}";
            }
        }
        return output;
    }

private:
    StateDescriptor *_sd;
    vector< map< const vector< int >, set<int> > > _facts; // PredType ( size = |Obj1| x ... x |ObjM|;  or size=1 for 0-ary)
    int _instance_id;
};

#endif //__RELAXED_STATE_H__
