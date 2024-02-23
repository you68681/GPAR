#ifndef __LANDMARK_DISCOVERY_H__
#define __LANDMARK_DISCOVERY_H__

#include "instance.h"
#include "domain.h"
#include "relaxed_actions.h"
#include "relaxed_graph.h"
#include "landmark.h"

class LandmarkGraph{
public:
    LandmarkGraph(Domain *d, Instance *i ){
        // Save domain and instance references
        _dom = d;
        _ins = i;

        // Create the relaxed graph
        _rg = new RelaxedGraph( d, i );

        // Initialize the variables of the class that depend on the number of facts
        int num_of_facts = _rg->getNumOfFacts();
        //_is_landmark.resize( num_of_facts, false );
        _is_init.resize( num_of_facts, false );
        _is_goal.resize( num_of_facts, false );
        //_greedy_necessary_edges.resize( num_of_facts );

        // Precompute initial state
        auto sd = _dom->getStateDescriptor();
        unique_ptr<RelaxedState> rs = make_unique<RelaxedState>( sd );
        auto init_reg = i->getInitialState()->getTypedRegisters();
        for( int pred_idx = 0; pred_idx < (int)init_reg.size(); pred_idx++ ){
            for( const auto& m : init_reg[ pred_idx ] ){
                rs->addFact( pred_idx, m.first, m.second );
            }
        }
        for( int fact_id = 0; fact_id < num_of_facts; fact_id++ ){
            auto fact = _rg->getFact( fact_id );
            if( rs->hasFact(fact.first.first, fact.first.second, fact.second ) ){
                _is_init[ fact_id ] = true;
                _init.push_back( fact_id );
                _init_set_fact_ids.insert( fact_id );
            }
        }

        // Precompute goal condition
        for( auto c : i->getGoalCondition() ){
            string pred_name = c->getLeftName( sd  );
            if( pred_name.length() == 0u ) continue; // It must be a predicate
            int pred_idx = sd->getPredicateIDX( pred_name );
            const vector< int > objs = c->getLeftObjects();
            int value = c->getRHS(sd, i->getInitialState() );
            auto fact_id = _rg->getFactID( pred_idx, objs, value );
            _goal.push_back( fact_id );
            _is_goal[ fact_id ] = true;
        }

        // Discover landmarks and orders if the task is not unsolvable
        assert( not _rg->isUnsolvable() );
        _available_id = 0;
        identify_landmarks();

        // make copies of each ordering

        // After identifying landmarks, get the list of initial landmark ids
        for( auto l : _landmarks ){
            if( l->getNumOfFacts() > 1 ) continue;
            int fact_id = *((l->getFactIDs()).begin());
            int land_id = l->getID();
            if( _is_init[ fact_id ] )
                _init_landmark_ids.push_back( land_id );
        }

        // Create constant access landmarks
        _const_access_landmarks.resize( _available_id, nullptr );
        for( auto l : _landmarks ){
            int lm_id = l->getID();
            _const_access_landmarks[ lm_id ] = l;
        }

        // For efficiency reasons add the inverse of greedy/natural necessary landmarks orderings
        for( auto l : _landmarks ){
            auto it = _greedy_necessary_ordering.find( l->getID() );
            if( it != _greedy_necessary_ordering.end() ) {
                for (auto m : it->second){
                    _inverse_gn_ordering[ m ].insert( it->first );
                }
            }
            if( NATURAL_ORDERING ) {
                it = _natural_ordering.find(l->getID());
                if (it != _natural_ordering.end()) {
                    for (auto m: it->second) {
                        _inverse_nat_ordering[m].insert(it->first);
                    }
                }
            }
        }

        if( POINTER_LANDMARKS ) {
            postprocessingPointerLandmarks();
        }
    }

    ~LandmarkGraph(){
        if(!_landmarks.empty() ){
            for( auto l : _landmarks )
                delete l;
        }
        delete _rg;
    }

    bool isGoal( unique_ptr<RelaxedState> &rs ){
        for( auto fact_id : _goal ){
            auto fact = _rg->getFact( fact_id );
            if( not rs->hasFact( fact.first.first, fact.first.second, fact.second ) )
                return false;
        }
        return true;
    }

    bool isDeadEnd( unique_ptr<RelaxedState> &rs ){
        int num_facts;
        do{
            num_facts = rs->getNumFacts();
            for( auto ra : _rg->getRelaxedActions() ){
                if( ra->isApplicable(rs) )
                    ra->apply(rs);
            }
        }while( num_facts < rs->getNumFacts() );

        return (not isGoal(rs));
    }

    bool isGoalFact( int fact_id ) const{
        return _is_goal[ fact_id ];
    }

    RelaxedGraph* getRelaxedGraph() const{
        return _rg;
    }

    pair<pair<int,vector<int> >, int > getFact( int fact_id ) const{
        return _rg->getFact( fact_id );
    }

    Landmark* computeLandmark( set<int> &fact_ids ){
        if( not COPY_LANDMARKS ) {
            auto f2l_it = _facts_to_landmarks.find(fact_ids);
            if (f2l_it != _facts_to_landmarks.end()) {
                return f2l_it->second;
            }
        }
        auto *possible_new_landmark = new Landmark( _available_id++);
        possible_new_landmark->setFacts( fact_ids );
        return possible_new_landmark;
    }

    static void remove_ordering( int landmark_id, map< int, set<int > > &ordering ){
        auto it_nat = ordering.find( landmark_id );
        if( it_nat != ordering.end() )
            ordering.erase(it_nat);
        for( auto it : ordering ){
            auto it_nat2 = (it.second).find( landmark_id );
            if( it_nat2 != (it.second).end() )
                (it.second).erase( it_nat2 );
        }
    }

    // Combine conjunctive facts to disjunctive ones
    void combine_facts( const vector< set< int > > &conjunctive_facts, int pos,
                        vector< int > &current_comb, set< set<int> > &res ){
        if( pos >= (int)conjunctive_facts.size() ){
            set< int > s_comb( current_comb.begin(), current_comb.end() );
            res.insert( s_comb );
            return;
        }
        assert( current_comb.size() == conjunctive_facts.size() );
        for( auto f_id : conjunctive_facts[ pos ] ){
            current_comb[ pos ] = f_id;
            combine_facts( conjunctive_facts, pos+1, current_comb, res );
        }
    }

    // Add "pre" landmark, and ordering pre =>_X post where X is the ordering type
    void add_landmark_and_ordering( Landmark *pre, const string &ordering_type, Landmark *post ){
        auto pre_fact_ids = pre->getFactIDs();

        // 1. Facts are preferred over disjunctions (pre landmark is a fact)
        if( pre_fact_ids.size() == 1u ){
            int fact_id = *(pre_fact_ids.begin());
            // Remove landmarks that contain pre in the disjunction
            for(auto it = _landmarks.begin(); it != _landmarks.end(); ){
                if( (*it)->getNumOfFacts() > 1 and (*it)->hasFact( fact_id ) ){
                    int landmark_id = (*it)->getID();
                    // Remove obsolete orderings
                    if( NATURAL_ORDERING ) {
                        remove_ordering(landmark_id, _natural_ordering);
                    }
                    remove_ordering( landmark_id, _greedy_necessary_ordering );
                    auto f2l_it = _facts_to_landmarks.find( (*it)->getFactIDs() );
                    assert( f2l_it != _facts_to_landmarks.end() );
                    _facts_to_landmarks.erase( f2l_it );
                    // [ToDo] shouldn't new orderings between pre and landmark_id be considered?
                    // Remove obsolete landmark
                    delete (*it);
                    _landmarks.erase(it );
                }
                else it++;
            }
        }

        // 2. Abort on overlap with existing landmark
        bool new_landmark = true;
        for( auto landmark : _landmarks ){
            auto fact_ids = landmark->getFactIDs();
            // 2.a If it is the same landmark as "pre" continue
            if( fact_ids == pre_fact_ids ) {
                new_landmark = false;
                continue;
            }
            // 2.b abort if intersection with the current landmark is found
            for( auto ipre = pre_fact_ids.begin(), il = fact_ids.begin();
                ipre != pre_fact_ids.end() and il != fact_ids.end(); ){
                if( *ipre == *il ) {// abort (intersection found)
                    delete pre;
                    return;
                }
                if( *ipre < *il ) ipre++;
                else il++;
            }
        }

        // 3. Add the new landmark to the graph
        if( COPY_LANDMARKS or new_landmark ){
            _facts_to_landmarks[ pre->getFactIDs() ] = pre;
            _landmarks.emplace(pre );
            _queue.push( pre );
        }

        // 4. Add the new ordering to the graph
        if( ordering_type == "nat" ){ // Natural ordering
            _natural_ordering[ pre->getID() ].insert( post->getID() );
        }
        else if( ordering_type == "gn" ){ // Greedy necessary ordering
            _greedy_necessary_ordering[ pre->getID() ].insert( post->getID() );
        }
    }

    void identify_landmarks(){
        // 0. [lines 12-13] landmark graph starts with all goals, no orderings
        for( auto goal_fact : _goal ){
            auto *l = new Landmark(_available_id++);
            l->addFact( goal_fact );
            _landmarks.insert(l );
            _queue.push( l );
            _goal_landmark_ids.push_back( l->getID() );
            _facts_to_landmarks[ l->getFactIDs() ] = l;
if ( INFO ) cout << "[INFO] New goal landmark: " << l->toString(_dom->getStateDescriptor(),_rg) << endl;
        }
        // further_orderings (not included yet)

        while( !_queue.empty() ){
            Landmark *current_landmark = _queue.front();
            _queue.pop();
            auto fact_ids = current_landmark->getFactIDs();
if( INFO ) cout << "[INFO] Current landmark: " << current_landmark->toString( _dom->getStateDescriptor(), _rg) ;

            // 1. [line 17] If the fact f_id is in the initial state, then the landmark is true in init
            bool is_init = false;
            for( auto f_id : fact_ids ){
                if(_init_set_fact_ids.find(f_id ) != _init_set_fact_ids.end() ){
                    is_init = true; break;
                }
            }
            if( is_init ) continue;
if( INFO ) cout << "[INFO] current landmarks are not in the initial state " << endl;
            // 2. [line 18] Build the restricted relax planning graph (RRPG)
            // Split relaxed horizontal_action between the ones that add the current landmark (possible achievers)
            // and the ones that do not (available horizontal_action)
            auto relaxed_actions = _rg->getRelaxedActions();
            vector< RelaxedAction* > available_actions;
            vector< RelaxedAction* > possible_achievers;
            for( auto ra : relaxed_actions ){
                for( auto f_id : fact_ids ) {
                    auto fact = _rg->getFact( f_id );
                    if (ra->addsFact( fact.first.first, fact.first.second, fact.second ) ) {
                        possible_achievers.push_back( ra );
                    }
                    else available_actions.push_back( ra );
                }
            }
if( INFO ) cout << "[INFO] relaxed horizontal_action computed " << endl;
            // Initialize relaxed state to initial state
            //auto *rs = new RelaxedState(_dom->getStateDescriptor() );
            unique_ptr<RelaxedState> rs = make_unique<RelaxedState>(_dom->getStateDescriptor());
            for( auto f_id : _init_set_fact_ids ) {
                auto fact = _rg->getFact( f_id );
                rs->addFact( fact.first.first, fact.first.second, fact.second );
            }
            // Apply all available horizontal_action while applicable and find the first possible achievers
            vector< RelaxedAction* > first_possible_achievers;
            int num_of_facts = rs->getNumFacts();
            bool first_achievers_found = false;
            do{
                num_of_facts = rs->getNumFacts();
                if( not first_achievers_found ) {
                    // Check if the possible achievers can be applied in the current relaxed state
                    for (auto ra: possible_achievers) {
                        if (ra->isApplicable(rs)) {
                            first_possible_achievers.push_back(ra);
                        }
                    }
                    first_achievers_found = (!first_possible_achievers.empty());
                }
                // (Commented) Break when first achievers are found
                // if( first_possible_achievers.size() > 0u ) break;
                // Otherwise continue applying available horizontal_action until fix point is reached
                vector< RelaxedAction* > applicable_rel_act;
                for( auto ra : available_actions ){
                    if( ra->isApplicable( rs ) ){
                        //ra->apply( rs );
                        applicable_rel_act.push_back( ra );
                    }
                }
                for( auto ra : applicable_rel_act ) ra->apply( rs );
            }while( num_of_facts < rs->getNumFacts() ); // fixpoint

            // 3. [lines 19-21] Shared preconditions (from first achievers) for current landmark
            // for each one add a greedy necessary ordering pre => current
            assert( !first_possible_achievers.empty() );

            _first_possible_achievers[ current_landmark->getID() ] = first_possible_achievers;

            set< int > shared_preconditions;
            vector< set< int > > prec_fact_ids( first_possible_achievers.size() );
            auto pre_shared = first_possible_achievers[ 0 ]->getPreconditionFacts();
            for( int pred_idx = 0; pred_idx < (int)pre_shared.size(); pred_idx++ ){
                for( const auto& m : pre_shared[ pred_idx ] ){
                    auto fact_id = _rg->getFactID( pred_idx, m.first, m.second );
                    shared_preconditions.insert( fact_id );
                    prec_fact_ids[ 0 ].insert( fact_id );
                }
            }
            for( int pos = 1; pos < (int)first_possible_achievers.size(); pos++ ){
                pre_shared = first_possible_achievers[ pos ]->getPreconditionFacts();
                set< int > current_preconditions;
                for( int pred_idx = 0; pred_idx < (int)pre_shared.size(); pred_idx++ ){
                    for( const auto& m : pre_shared[ pred_idx ] ){
                        auto fact_id = _rg->getFactID(pred_idx, m.first, m.second );
                        prec_fact_ids[ pos ].insert( fact_id );
                        if( shared_preconditions.find( fact_id ) != shared_preconditions.end() )
                            current_preconditions.insert( fact_id );
                    }
                }
                // Update the set of shared preconditions
                swap( shared_preconditions, current_preconditions );
            }
            // Add landmark and ordering
            for( auto sp : shared_preconditions ){
                // Retrieve the landmark if already exists, otherwise create a new one
                set< int > aux_fact_id = {sp};
                Landmark *shared_prec_landmark = computeLandmark( aux_fact_id );
if( INFO ) cout << "[INFO] Shared precondition landmark: " << shared_prec_landmark->toString( _dom->getStateDescriptor(), _rg );
                add_landmark_and_ordering( shared_prec_landmark, "gn", current_landmark );
if( INFO ) cout << "[INFO] landmark and greedy necessary ordering added" << endl;
            }

            // 4. [lines 22-25] Disjunctive fact to cover all shared preconditions
            // (optional) combine only one fact of each achiever of the same predicate symbol
            // (optional) in the LAMA paper, disjunctions up to size 4
            // Filter the uncovered facts
            vector< set<int> > pre_disj;
            for( const auto& pfact_ids : prec_fact_ids ){
                set< int > uncovered_facts;
                for( auto f_id : pfact_ids ){
                    if( shared_preconditions.find( f_id ) == shared_preconditions.end() ){
                        uncovered_facts.insert( f_id );
                    }
                }
                if( !uncovered_facts.empty() )
                    pre_disj.push_back( uncovered_facts );
            }

            if( !pre_disj.empty() ) {
                vector<int> current_comb(pre_disj.size());
                set<set<int> > disjunctive_facts;
                combine_facts(pre_disj, 0, current_comb, disjunctive_facts);

                for (auto disjunction : disjunctive_facts) {
                    bool entailed_by_initial_state = false;
                    for (auto f_id : disjunction) {
                        if (_is_init[f_id]) {
                            entailed_by_initial_state = true;
                            break;
                        }
                    }
                    if (entailed_by_initial_state) continue;

                    // If the disjunction is not entailed by the initial state
                    // add it as a disjunctive landmark
                    Landmark *disjunctive_landmark = computeLandmark( disjunction );
                    add_landmark_and_ordering(disjunctive_landmark, "gn", current_landmark);
                }
            }

            // 5. [lines 26-29] (optional) extract landmarks from DTG

            // 6. [line 30] (optional) add potential natural orderings from current landmark to unachieved ones
            // add a natural ordering from current landmark => l\in L s.t. l is not entailed by rs
            vector< Landmark* > potential_nat_orderings;
            int current_id = current_landmark->getID();
            for( auto l : _landmarks ){
                int l_id = l->getID();
                if( l_id == current_id ) continue;
                auto it = _greedy_necessary_ordering[ current_id ].find( l_id );
                if( it != _greedy_necessary_ordering[ current_id ].end() ) continue;

                if( not l->holds(_rg, rs) ){
                    potential_nat_orderings.push_back( l );
                }
            }
            // Make one iteration_action_less in the relaxed state for all relaxed horizontal_action
            vector< RelaxedAction* > ra_applicable;
            //for( auto ra : relaxed_actions ){
            for( auto ra : first_possible_achievers ){
                if( ra->isApplicable( rs ) )
                    ra_applicable.push_back( ra );
            }
            for( auto ra : ra_applicable ) {
                ra->apply(rs);
            }

            // Check all potential nat ordering, if they do not hold then add them as a nat ordering
            for( auto l : potential_nat_orderings ){
                if( not l->holds(_rg, rs ) ) {
                    add_landmark_and_ordering(current_landmark, "nat", l);
                }
            }
        }

    }

    vector< int > getInitialLandmarksIDs() const{
        return _init_landmark_ids;
    }

    vector< int > getGoalLandmarksIDs() const{
        return _goal_landmark_ids;
    }

    int getNumLandmarks() const{
        return (int)_landmarks.size();
    }

    Landmark* getLandmark( int landmark_id ) const{
        assert( landmark_id < (int)_const_access_landmarks.size() );
        assert( _const_access_landmarks[ landmark_id ] != nullptr );
        return _const_access_landmarks[ landmark_id ];
    }

    set< Landmark* > getLandmarks() const{
        return _landmarks;
    }

    set<int> getGreedyNecessaryLandmarkOrdering( int landmark_id ) const{
        auto it = _greedy_necessary_ordering.find( landmark_id );
        if( it == _greedy_necessary_ordering.end() )
            return {};
        return it->second;
    }

    set<int> getInverseGreedyNecessaryLandmarkOrdering( int landmark_id ) const{
        auto it = _inverse_gn_ordering.find( landmark_id );
        if( it == _inverse_gn_ordering.end() )
            return {};
        return it->second;
    }

    set<int> getInverseNaturalLandmarkOrdering( int landmark_id ) const{
        auto it = _inverse_nat_ordering.find( landmark_id );
        if( it == _inverse_nat_ordering.end() )
            return {};
        return it->second;
    }

    void postprocessingPointerLandmarks(){
        //auto sd = _dom->getStateDescriptor();
        vector< Landmark* > aux_landmarks;
        set< pair<int,int> > aux_gn_orderings;

        for( auto l : _landmarks ){
            int land_id = l->getID();
//cout << "[INFO] Checking landmark:" << l->toString(_dom->getStateDescriptor(),_rg);
            auto fact_ids = l->getFactIDs();
            if( fact_ids.size() == 1u and _is_init[ *fact_ids.begin() ] ) {
//cout << "[INFO] is INIT!" << endl;
                continue;
            }
            auto *new_landmark = new Landmark(_available_id++);
            auto fpa = _first_possible_achievers[ land_id ];
            int current_size = 1;
            bool all_zeros = false;
            for( int fpa_idx = 0; fpa_idx < (int)fpa.size(); fpa_idx++  ){
//cout << fpa[ fpa_idx ]->toString(_dom->getStateDescriptor()) << endl;
                auto param_types = fpa[fpa_idx]->getParamTypes();
                auto obj_ids = fpa[fpa_idx]->getGroundedParams();
                if( obj_ids.empty() ) continue;
                int num_zeros = 0;
                new_landmark->setPossibleAchieversSize( current_size++ );
                for( int param = 0; param < (int)param_types.size(); param++ ) {
                    new_landmark->addVarAssignment( fpa_idx, param_types[param], obj_ids[param] );
                    if( obj_ids[ param ] == 0 ) num_zeros++;
                }
                if( num_zeros == (int)obj_ids.size() ) all_zeros = true;
            }
            if( current_size == 1 ){ delete new_landmark; continue; } // possible achievers have no parameters
            // Save new landmark and orderings
            aux_landmarks.push_back( new_landmark );
            int new_landmark_id = new_landmark->getID();
            aux_gn_orderings.insert(make_pair( new_landmark_id, land_id ) );
            for( auto ign : _inverse_gn_ordering[ land_id ] ){
                aux_gn_orderings.insert(make_pair(ign, new_landmark_id));
            }
            if( all_zeros ) _init_landmark_ids.push_back( new_landmark_id );
        }
        // Add the new landmarks
        _const_access_landmarks.resize( _available_id, nullptr );
        for( auto landmark : aux_landmarks ){
            //_facts_to_landmarks[] ?
            _landmarks.insert( landmark );
            _const_access_landmarks[ landmark->getID() ]  = landmark;
        }
        // Add the new greedy necessary orderings
        for( auto gn_ord : aux_gn_orderings ){
            _greedy_necessary_ordering[ gn_ord.first ].insert( gn_ord.second );
            _inverse_gn_ordering[ gn_ord.second ].insert( gn_ord.first );
        }
        // Test first LOCK instance constraint
        //_greedy_necessary_ordering[ 4 ].insert( 27 );
        //_inverse_gn_ordering[ 27 ].insert( 4 );
    }

    string toString() const{
        string output = "[LANDMARKS][INSTANCE #" + to_string( _ins->getInstanceID() ) + "]:\n";
        auto sd = _dom->getStateDescriptor();
        //output += _rg->toString(sd) +"\n"; // Print the relaxed graph too
        for( auto l : _landmarks ){
            auto landmark_id = l->getID();
            auto fact_ids = l->getFactIDs();
            // Print landmark
            output += l->toString(sd,_rg);

            // Print greedy necessary orderings
            output += "GREEDY-NECESSARY: ";
            auto it_gn = _greedy_necessary_ordering.find( landmark_id );
            //assert( it_gn != _greedy_necessary_ordering.end() );
            output += to_string( landmark_id ) + " => {";
            if( it_gn != _greedy_necessary_ordering.end() ) {
                for (auto it = it_gn->second.begin(); it != it_gn->second.end(); it++) {
                    output += (it != it_gn->second.begin() ? "," : "") + to_string(*it);
                }
            }
            output += "}\n";
            if( NATURAL_ORDERING ) {
                output += "NATURAL: ";
                auto it_nat = _natural_ordering.find(landmark_id);
                //assert( it_gn != _greedy_necessary_ordering.end() );
                output += to_string(landmark_id) + " => {";
                if (it_nat != _natural_ordering.end()) {
                    for (auto it = it_nat->second.begin(); it != it_nat->second.end(); it++) {
                        output += (it != it_nat->second.begin() ? "," : "") + to_string(*it);
                    }
                }
                output += "}\n";
            }
        }
        return output;
    }

private:
    /// Original domain and instance
    Domain *_dom;
    Instance *_ins;
    /// Relaxed graph of the original domain and instance
    RelaxedGraph * _rg;
    /// Initial fact ids
    vector< int > _init;
    vector< bool > _is_init;
    vector< int > _init_landmark_ids;
    set< int > _init_set_fact_ids;
    /// Goal fact ids
    vector< int > _goal;
    vector< bool > _is_goal;
    vector< int > _goal_landmark_ids;
    /// Discovered landmarks
    map< set<int>, Landmark* > _facts_to_landmarks; // set of fact ids map to landmark ids
    set< Landmark* > _landmarks; // set of landmarks
    vector< Landmark* > _const_access_landmarks; // auxiliary variable to have constant access to landmarks
    map< int, set<int> > _natural_ordering; // natural orderings of two landmark ids
    map< int, set<int> > _inverse_nat_ordering; // post <-nat pre landmarks
    map< int, set<int> > _greedy_necessary_ordering; // greedy necessary orderings of two landmark ids
    map< int, set<int> > _inverse_gn_ordering; // post <-gn pre landmarks
    queue< Landmark* > _queue;
    int _available_id;
    map< int, vector<RelaxedAction*> > _first_possible_achievers;
};

#endif //__LANDMARKS_H__
