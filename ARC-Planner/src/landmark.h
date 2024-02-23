#ifndef __LANDMARK_H__
#define __LANDMARK_H__

#include "relaxed_state.h"
#include "relaxed_graph.h"

class Landmark{
public:
    explicit Landmark( int id ) : _landmark_id(id){ }

    explicit Landmark( Landmark* l ){
        _landmark_id = -1;
        _fact_ids = l->getFactIDs();
        _var_type_assignments = l->getVarAssignments();
    }

    ~Landmark()= default;

    Landmark* copy(){
        return new Landmark(this);
    }

    void setPossibleAchieversSize( int pa_size ){
        _var_type_assignments.resize( pa_size );
    }

    void addVarAssignment( int possible_achiever_idx, int var_id, int value ){
        assert( possible_achiever_idx < (int)_var_type_assignments.size() );
        _var_type_assignments[ possible_achiever_idx ].push_back( make_pair( var_id, value ) );
    }

    bool isPointerLandmark() const{
        return ( !_var_type_assignments.empty() );
    }

    void addFact( int fact_id ){
        _fact_ids.insert( fact_id );
    }

    void setFacts( set<int> &fact_ids ){
        _fact_ids = fact_ids;
    }

    bool hasFact( int fact_id ) const{
        return (_fact_ids.find(fact_id) != _fact_ids.end());
    }

    int getNumOfFacts() const{
        return (int)_fact_ids.size();
    }

    int getID() const{ return _landmark_id; }

    set< int > getFactIDs() const{
        return _fact_ids;
    }

    vector< vector<pair<int,int> > > getVarAssignments() const{
        return _var_type_assignments;
    }

    // Disjunctive facts checking
    bool holds( RelaxedGraph *rg, unique_ptr<RelaxedState> &rs ) const{
        for( auto f_id : _fact_ids ){
            auto fact = rg->getFact( f_id );
            if( rs->hasFact( fact.first.first, fact.first.second, fact.second ) )
                return true;
        }
        return false;
    }

    bool holds( RelaxedGraph *rg, State *s ) const{
        auto reg = s->getTypedRegisters();
        for( auto f_id : _fact_ids ){
            auto fact = rg->getFact( f_id ); // fact = < < pred_idx, objs >, value >
            auto it = reg[ fact.first.first ].find( fact.first.second );
            if( ( it != reg[ fact.first.first ].end() and (it->second) == fact.second ) or
                (it == reg[ fact.first.first ].end() and fact.second == 0) )
                return true;
        }
        return false;
    }

    bool holds( StateDescriptor *sd, State *s ) const{
        assert( isPointerLandmark() );
        for( const auto& pa : _var_type_assignments ) {
            bool conjunction_holds = true;
            for (auto vta: pa) {
                auto ptr_ids = sd->getTypedPointerIDs( vta.first );
                bool var_assignment_found = false;
                for( auto ptr_id : ptr_ids ){
                    int ptr_val = s->getPointer( sd,ptr_id );
                    if( ptr_val  == vta.second ){
                        var_assignment_found = true;
                        break;
                    }
                }
                if( not var_assignment_found ){
                    conjunction_holds = false;
                    break;
                }
            }
            if( conjunction_holds ) return true;
        }
        return false;
    }

    /*bool operator< (const Landmark *l) const {
        return _landmark_id < l->getID();
    }*/
    /*inline bool operator()( const Landmark *a, const Landmark *b ) const{
        return a->getID() < b->getID();
    }*/

    string toString( StateDescriptor *sd, RelaxedGraph *rg ) const{
        string output = "[LANDMARK]: id = " + to_string( _landmark_id ) + "; ";
        if( isPointerLandmark() ){
            for( auto pa : _var_type_assignments ){
                if( pa != _var_type_assignments[0] ) output += " OR ";
                output += "{";
                for( auto vta : pa ) {
                    if( vta != (*pa.begin()) ) output += " AND ";
                    auto ptr_ids = sd->getTypedPointerIDs(vta.first);
                    output += "{";
                    for( auto ptr_id : ptr_ids ){
                        auto ptr_name = sd->getPointerName( ptr_id );
                        output += (ptr_id != ptr_ids[0]?"|":"") + ptr_name;
                    }
                    output += "} = " + to_string( vta.second );
                }
                output += "}";
            }
        }
        else{
            output += "L = {";
            for( auto it = _fact_ids.begin(); it != _fact_ids.end(); it++ ){
                output += (it!=_fact_ids.begin()?" | ":"") + rg->factID2String(sd, *it );
            }
            output += "}";
        }
        output += "\n";
        return output;
    }

private:
    // Disjunctive facts
    set< int > _fact_ids;
    // Landmark id
    int _landmark_id;
    vector< vector<pair<int,int> > > _var_type_assignments; // {var1 = val & var2 = val ...} | {var1 = val & val2 = val} | ...
};

#endif //__LANDMARK_H__
