#ifndef __RELAXED_GRAPH_H__
#define __RELAXED_GRAPH_H__

#include "relaxed_actions.h"

class RelaxedGraph{
public:
    explicit RelaxedGraph( Domain *dom, Instance *ins ){
        _dom = dom;
        _ins = ins;

        // Init all pred domains to MAX_VAL
        auto sd = _dom->getStateDescriptor();
        auto pred_names = sd->getPredicateTypes();
        for( auto pn : pred_names ){
            int pred_id = sd->getTypeID( pn );
            sd->setStateVarBound(pred_id, _ins->getInstanceID(), MAX_VAL );
        }

        // Precompute facts and relaxed horizontal_action
        precompute_facts();
        precompute_relaxed_actions();

        // Initialize variables
        int num_of_facts = getNumOfFacts();
        _parent_facts.resize( num_of_facts );
        _fact_layer.resize( num_of_facts, INF );

        // Build the relaxed graph
        _unsolvable = false;
        build_relaxed_graph();
    }

    ~RelaxedGraph(){
        // Delete relaxed horizontal_action
        for(auto & relaxed_action : _relaxed_actions){
            delete relaxed_action;
        }
    }

    vector< int > getFactsIDs(vector< map< vector< int >, int > > &facts ){
        vector< int > f_ids;
        for( unsigned i = 0; i < facts.size(); i++ ){
            for( const auto& m : facts[ i ] ){
                auto fact = make_pair(make_pair(i, m.first), m.second);
                auto it = _fact_2_id.find(fact);
                assert(it != _fact_2_id.end());
                f_ids.push_back(it->second);
            }
        }
        return f_ids;
    }

    void updateRoots( vector< map< vector< int >, int > > &facts ){
        auto f_ids = getFactsIDs(facts);
        for( auto f : f_ids ){
            _parent_facts[ f ].push_back( vector<int>(1,f) );
            _fact_layer[ f ] = 0;
        }
    }

    void updateParents( RelaxedAction *a, int layer ){
        auto parent_facts = a->getPreconditionFacts();
        auto parent_ids = getFactsIDs(parent_facts);
        auto succ_facts = a->getEffectFacts();
        auto succ_ids = getFactsIDs(succ_facts);
        for( auto s : succ_ids ){
            // Update the parents for each action that adds the landmark
            // only in the current layer
            if( _fact_layer[ s ] >= layer ) {
                _fact_layer[ s ] = layer;
                _parent_facts[s].push_back(parent_ids);
            }
        }
    }

    int getFactID( int pred_idx, const vector<int> &objs, int value ){
        auto it = _fact_2_id.find(make_pair(make_pair(pred_idx,objs), value));
        assert( it != _fact_2_id.end() );
        return it->second;
    }

    string factID2String( StateDescriptor *sd, int fact_id ) const {
        auto fact = _id_2_fact[ fact_id ];
        auto pred_names = sd->getPredicateTypes(); // this can be improved, directly accessing Predicate IDX
        string output = pred_names[ fact.first.first ] + "(";
        for( unsigned o_idx = 0; o_idx < fact.first.second.size(); o_idx++ )
            output += (o_idx?",":"") + to_string( fact.first.second[ o_idx ] );
        output += ") = " + to_string( fact.second );

        return output;
    }

    int getFactLayer( int pred_idx, const vector<int> &objs, int value ) const{
        auto fact = make_pair(make_pair( pred_idx, objs ), value );
        auto it_fact_id = _fact_2_id.find( fact );
        assert( it_fact_id != _fact_2_id.end() );
        return _fact_layer[ it_fact_id->second ];
    }

    void groundParams( StateDescriptor *sd, const vector<string> &var_types, vector<vector< int > > &obj_ids,
                       vector< int > &current, unsigned idx = 0  ){
        if( var_types.empty() ) return;
        if( idx >= var_types.size() ){ obj_ids.push_back( current ); return; }
        auto bound = sd->getBound( var_types[ idx ], _ins->getInstanceID() );
        for( int b = 0; b < bound; b++){
            current[ idx ] = b;
            groundParams( sd, var_types, obj_ids, current, idx+1 );
        }
    }

    void precompute_facts(){
        auto sd = _dom->getStateDescriptor();
        auto pred_names = sd->getPredicateTypes();
        int fact_id_counter = 0;

        for( unsigned pred_idx = 0; pred_idx < pred_names.size(); pred_idx++ ){
            auto pred_name = pred_names[ pred_idx ];
            auto pred_var_names = sd->getPredicateVarNames( pred_name );
            //auto pred_var_ids = sd->getPredicateVarTypeIDs( pred_name );
            vector< vector< int > > obj_ids;
            if( pred_var_names.size() == 0u ) {
                obj_ids.push_back( vector<int>());
            }
            else{
                vector< int > current_grounding( pred_var_names.size() );
                groundParams( sd, pred_var_names, obj_ids, current_grounding );
            }
            for( const auto objs : obj_ids ){
                auto bound = sd->getBound( pred_names[ pred_idx ], _ins->getInstanceID() );
                for( int b = 0; b < bound; b++ ) {
                    auto fact = make_pair(make_pair(pred_idx,objs),b);
                    _id_2_fact.push_back( fact );
                    _fact_2_id[ fact ] = fact_id_counter++;
                }
            }

        }
    }

    void precompute_relaxed_actions(){
        set< string > computed_actions;
        StateDescriptor *sd = _dom->getStateDescriptor();
        for( auto a : _dom->getActions() ){
            auto full_name = a->getName();
            size_t it = full_name.find( "(" );
            assert( it != string::npos );
            string act_name = full_name.substr( 0, it );

            full_name = full_name.substr( it + 1, full_name.length() - (it+2) );
            // Do not process the action, if it has been previously grounded
            if( computed_actions.find( act_name ) != computed_actions.end() )
                continue;
            computed_actions.insert( act_name );
            vector< string > params;
            map< string, int > pointer_to_param_idx;
            vector<int> param_types;
            int p_idx = 0;
            while( (it = full_name.find(",")) != string::npos ){
                // horizontal_action are grounded with pointers in _d, thus param is a pointer
                auto param = full_name.substr( 0, it );
                assert( sd->isPointer(param) );
                full_name = full_name.substr(it+1);
                int var_id = sd->getPointerVarType( param );
                params.push_back( sd->getVarName( var_id ) );
                pointer_to_param_idx[ param ] = p_idx++;
                param_types.push_back( var_id );
            }
            if( full_name.length() > 0u ) {
                int var_id = sd->getPointerVarType( full_name );
                params.push_back( sd->getVarName( var_id ) );
                pointer_to_param_idx[ full_name ] = p_idx;
                param_types.push_back( var_id );
            }

            vector<vector< int > > obj_ids;
            vector< int > current_grounding( params.size() );
            groundParams(sd, params, obj_ids,  current_grounding);
            if( params.size() == 0u ) obj_ids.push_back( vector<int>() );
            for( auto objs : obj_ids ){
                string grounded_act_name = act_name;
                grounded_act_name += "(";
                for( unsigned o_id = 0; o_id < objs.size(); o_id++ )
                    grounded_act_name += (o_id?",":"") + to_string( objs[o_id] );
                grounded_act_name += ")";

                // Create new grounded delete-relaxed action
                RelaxedAction *ra = new RelaxedAction( sd, grounded_act_name );
                ra->setParamType( param_types );
                ra->setGroundedParams( objs );

                // Add preconditions
                auto conds = a->getPreconditions();
                for( auto c : conds ){
                    string pred_name = c->getLeftName( sd  );
                    if( pred_name.length() == 0u ) continue; // It must be a predicate
                    int pred_idx = sd->getPredicateIDX( pred_name );
                    vector< string > pointer_names = c->getLeftPointerNames( sd );
                    vector< int > object_ids( pointer_names.size() );
                    for( p_idx = 0; p_idx < (int)pointer_names.size(); p_idx++ ){
                        auto it_ptr2par = pointer_to_param_idx.find(pointer_names[ p_idx ] );
                        assert(it_ptr2par != pointer_to_param_idx.end() );
                        object_ids[ p_idx ] = objs[ it_ptr2par->second ];
                    }
                    int value = c->getRHS(sd, _ins->getInitialState() );
                    ra->addPrecondition( pred_idx, object_ids, value );
                }

                // Add effects
                auto effs = a->getEffects();
                for( auto eff : effs ){
                    string pred_name = eff->getLeftName( sd  );
                    if( pred_name.length() == 0u ) continue; // It must be a predicate
                    int pred_idx = sd->getPredicateIDX( pred_name );
                    vector< string > pointer_names = eff->getLeftPointerNames( sd );
                    vector< int > object_ids( pointer_names.size() );
                    for( p_idx = 0; p_idx < (int)pointer_names.size(); p_idx++ ){
                        auto it_ptr2par = pointer_to_param_idx.find(pointer_names[ p_idx ] );
                        assert(it_ptr2par != pointer_to_param_idx.end() );
                        object_ids[ p_idx ] = objs[ it_ptr2par->second ];
                    }
                    int value = eff->getRHS(sd, _ins->getInitialState() );
                    ra->addEffect( pred_idx, object_ids, value );
                }

                // Add the relaxed action to the class
                _relaxed_actions.push_back( ra );
            }
        }
    }

    void build_relaxed_graph() {
        auto sd = _dom->getStateDescriptor();
        auto init = _ins->getInitialState()->getTypedRegisters();
        auto pred_names = sd->getPredicateTypes();
        auto num_of_preds = (int)pred_names.size();

        // 1. get facts from the initial state of the instance
        unique_ptr<RelaxedState> rs = make_unique<RelaxedState>( sd );

        for( int pred_idx = 0; pred_idx < num_of_preds; pred_idx++ ){
            for( auto it : init[ pred_idx ] ){
                rs->addFact( pred_idx, it.first, it.second );
            }
        }

        // set up roots
        vector< map< vector<int>, int > > roots( init.size() );
        for( const auto& fact : _id_2_fact ){
            if( rs->hasFact( fact.first.first, fact.first.second, fact.second ) ){
                roots[ fact.first.first ][ fact.first.second ] = fact.second;
            }
        }

        updateRoots( roots );

        // add facts and their parents to the graph
        int num_of_facts;
        int num_of_iter = 1;
        do {
            num_of_facts = rs->getNumFacts();

            // 2. apply all applicable relaxed horizontal_action that add some new fact
            vector< RelaxedAction* > applicable_act;
            for (auto a : _relaxed_actions) {
                if (a->isApplicable(rs)) {
                    auto add_facts = a->getEffectFacts();
                    bool add_action = false;
                    for( int pred_idx = 0; pred_idx < num_of_preds; pred_idx++ ){
                        for( auto m : add_facts[ pred_idx ] ){
                            if( not rs->hasFact( pred_idx, m.first, m.second ) ){
                                add_action = true;
                                break;
                            }
                        }
                        if( add_action ) break;
                    }
                    if( add_action ) applicable_act.push_back( a );
                }
            }
            for( auto a : applicable_act ) {
                a->apply( rs );
                updateParents( a, num_of_iter ); // update the relaxed graph
            }

            num_of_iter++;

            // 3. stop when the relaxed state hasn't change between two consecutive iterations
        }while( rs->getNumFacts() > num_of_facts );

        _unsolvable = ( not isGoal(rs) );

        // 4. Update the predicate bounds to the maximum observed
        for( int pred_idx = 0; pred_idx < num_of_preds; pred_idx++ ){
            int max_val = rs->getMaxVal( pred_idx );
            int pred_id = sd->getTypeID( pred_names[ pred_idx ] );
            sd->setStateVarBound(pred_id, _ins->getInstanceID(), max_val+1 );
        }
    }

    bool isGoal( unique_ptr<RelaxedState> &rs ){
        auto sd = _dom->getStateDescriptor();
        auto goal = _ins->getGoalCondition();
        for( auto g : goal ){
            string pred_name = g->getLeftName( sd  );
            if( pred_name.length() == 0u ) continue; // It must be a predicate
            int pred_idx = sd->getPredicateIDX( pred_name );
            const vector< int > objs = g->getLeftObjects();
            int value = g->getRHS(sd, _ins->getInitialState() );
            if( not rs->hasFact(pred_idx, objs, value ) )
                return false;
        }
        return true;
    }

    bool isUnsolvable() const{
        return _unsolvable;
    }

    int getNumOfFacts() const{
        return (int)_id_2_fact.size();
    }

    vector< RelaxedAction* > getRelaxedActions() const{
        return _relaxed_actions;
    }

    vector<vector<int> > getParents( int fact_id ) const{
        assert( fact_id < (int)_parent_facts.size() );
        return _parent_facts[ fact_id ];
    }

    pair< pair< int, vector<int> >, int > getFact( int fact_id ) const{
        assert( fact_id < (int)_id_2_fact.size() );
        return _id_2_fact[ fact_id ];
    }

    string toString(StateDescriptor *sd) const{
        string output = "[RELAXED-GRAPH]:";
        for( int fact_id = 0; fact_id < (int)_parent_facts.size(); fact_id++ ){
            if( !_parent_facts[ fact_id ].empty() ) output += "\n";
            for( unsigned p_idx = 0; p_idx < _parent_facts[ fact_id ].size(); p_idx++ ){
                output += (p_idx ? " | {":"{");
                for( unsigned pf_idx = 0; pf_idx < _parent_facts[ fact_id ][ p_idx ].size(); pf_idx++ ) {
                    output += (pf_idx ? "," : "") + factID2String(sd, _parent_facts[fact_id][p_idx][pf_idx]);
                }
                output += "}";
            }
            if( !_parent_facts[ fact_id ].empty() ) output += ": " +  factID2String( sd, fact_id );
        }
        return output;
    }

private:
    Domain *_dom;
    Instance *_ins;
    vector< RelaxedAction* > _relaxed_actions;
    map< pair< pair< int, vector<int> >, int >, int > _fact_2_id;
    vector< pair< pair< int, vector<int> >, int > > _id_2_fact;
    vector< vector< vector< int > > > _parent_facts; // disjunctive parents
    vector< int > _fact_layer;
    bool _unsolvable;
};

#endif
