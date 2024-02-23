#ifndef __PARSER_H__
#define __PARSER_H__

#include "common.h"
#include "variable.h"
#include "state_descriptor.h"
#include "state.h"
#include "condition.h"
#include "operation.h"
#include "action.h"
#include "domain.h"
#include "instance.h"
#include "instruction.h"
#include "abstractgraph.h"


class Parser{
public:
	Parser()= default;
	
	~Parser()= default;

    static map<int,vector<int> > all_objects;

    static map<int, vector<int>> node_size;

    static map<int, vector<int>> node_shape;

    static map<int, vector<int>> node_color;

    static map<int, vector<int>> even_node;

    static map<int, vector<int>> odd_node;

    static map<int, vector<int>> node;

    static map<AbstractGraph*, vector<int>> output_node;



	static string errorMessage( const string &expected_word = "", const string &readed_word = "" ) {
		return "Expected ''" + expected_word + "'' but ''" + readed_word + "'' read.";
	}

	static bool isDigit( const string &s ) {
		return (s.find_first_not_of("0123456789") == string::npos );
	}

    static vector< string > split( const string &s, char delimiter = ',' ){
        vector< string > splitted_s;
        size_t l = 0, n = 0;
        while( (n = s.find(delimiter, l)) != string::npos ){
            splitted_s.push_back( s.substr( l, n-l ) );
            l = n+1;
        }
        if( !s.empty() )
            splitted_s.push_back( s.substr(l) );
        return splitted_s;
    }

    static Variable* readGroundedVar( StateDescriptor *sd, const string &name, Instance *ins ){
        string var_name = name, var_type;
        int var_id;

        bool var_constant = isDigit( name );
        vector< int > param_ids;
        size_t pos;
        if( var_constant ) {
            var_type = "constant";
            var_id = atoi( name.c_str() );
        }
        else if( (pos = name.find('(')) != string::npos ) {
            var_type = "predicate";
            var_name = name.substr( 0, pos );
            var_id = sd->getTypeID( var_name );
            var_name += "(";
            auto param_names = split( name.substr( pos+1, (name.length() - 1 ) - (pos+1) ) );
            for (unsigned i = 0; i < param_names.size(); i++) {
                auto obj_address = ins->getObjectAddress(param_names[i]);
                param_ids.push_back(-(obj_address + 1)); // const objects have ids < 0;
                //var_name += param_name;
                var_name += (i ? "," : "") + to_string(obj_address);
            }
            var_name += ")";
        }
        else {
            var_type = "pointer";
            var_id = sd->getTypeID( name );
        }

        return (new Variable(var_name,var_type,var_id,param_ids));
	}

	static Variable* groundingVar( StateDescriptor *sd, const string &name, const map< string, int > &par_name_to_idx,
                       const vector< string > &grounded_params ){
		string var_name = name, var_type;
        int var_id;

        bool var_constant = isDigit( name );
        vector< int > param_ids;
        size_t pos;
		if( var_constant ) {
            var_type = "constant";
            var_id = atoi( name.c_str() );
        }
		else if( (pos = name.find('(')) != string::npos ) {
		    var_type = "predicate";
		    var_name = name.substr( 0, pos );
		    var_id = sd->getTypeID( var_name );
		    var_name += "(";
            auto param_names = split( name.substr( pos+1, (name.length() - 1 ) - (pos+1) ) );
            for( unsigned i = 0; i < param_names.size(); i++){
                //cout << name << " => " << var_name << ": " << param_names[i] << endl;
                auto it = par_name_to_idx.find(param_names[i]);
                assert(it != par_name_to_idx.end());
                auto idx = it->second;
                assert(idx < (int)grounded_params.size());
                auto param_id = sd->getTypeID(grounded_params[idx]); // get pointer ID
                param_ids.push_back(param_id);
                var_name += (i?",":"") + sd->getPointerName( param_id );
            }
            var_name += ")";
		}
		else {
		    var_type = "pointer";
		    var_id = sd->getTypeID( name );
		}

		return (new Variable( var_name, var_type, var_id, param_ids  ));
	}

    static Variable* groundingVar_object( StateDescriptor *sd, const string &name, const map< string, int > &par_name_to_idx,
                                   const vector< string > &grounded_params ){
        string var_name = name, var_type;
        int var_id;

        bool var_constant = isDigit( name );
        vector< int > param_ids;
        size_t pos;
        if( var_constant ) {
            var_type = "constant";
            var_id = atoi( name.c_str() );
        }
        else if( (pos = name.find('(')) != string::npos ) {
            var_type = "predicate";
            var_name = name.substr( 0, pos );
            var_id = sd->getTypeID( var_name );
            var_name += "(";
            auto param_names = split( name.substr( pos+1, (name.length() - 1 ) - (pos+1) ) );
            for( unsigned i = 0; i < param_names.size(); i++){
                //cout << name << " => " << var_name << ": " << param_names[i] << endl;
                auto it = par_name_to_idx.find(param_names[i]);
                assert(it != par_name_to_idx.end());
                auto idx = it->second;
                assert(idx < (int)grounded_params.size());
                auto param_id = sd->getTypeID(grounded_params[idx]); // get pointer ID
                param_ids.push_back(param_id);
                if (param_names[i]=="no"){
                    var_name += (i?",":"") + sd->getPointerName( param_id );
                }
                else{
                    if(sd->checkpointer(param_id)){
                        var_name += (i?",":"") + sd->getPointerName( param_id );
                    }
                    else{
                        var_name+=(i?",":"") + sd->getObjectName( param_id );
                    }
                }
            }
            var_name += ")";
        }
        else {
            var_type = "pointer";
            var_id = sd->getTypeID( name );
        }

        return (new Variable( var_name, var_type, var_id, param_ids  ));
    }


	static Condition* readCondition( StateDescriptor *sd, const string &name, Variable *lhs, Variable *rhs ){
		if( ">=" == name ) return new GreaterEqual( sd, lhs, rhs );
		else if( "<=" == name ) return new LesserEqual( sd, lhs, rhs );
		else if( ">" == name ) return new Greater( sd, lhs, rhs );
		else if( "<" == name ) return new Lesser( sd, lhs, rhs );
		else if( "=" == name ) return new Equals( sd, lhs, rhs );
		else if( "+" == name ) return new Add( sd, lhs, rhs );
//        else if( "+" == name ) return new AddNew( sd, lhs, rhs );
		else if( "-" == name ) return new Subtract( sd, lhs, rhs );
		else if( "!=" == name ) return new Different( sd, lhs, rhs );
//        else if( "@=" == name ) return new FunctionsEquals( sd, lhs, rhs );
		
		return nullptr;
	}



    static Operation* readOperation( StateDescriptor *sd, const string &name, Variable *lhs, Variable *rhs ){
		if( "+=" == name ) return new AddAssign( sd, lhs, rhs );
		else if( "-=" == name ) return new SubtractAssign( sd, lhs, rhs );
		else if( "=" == name ) return new Assign( sd, lhs, rhs );
		else if( "-" == name ) return new Compare( sd, lhs, rhs );
        else if( "-" == name ) return new Compare( sd, lhs, rhs );
        else if( "@=" == name ) return new FunctionsAssign( sd,lhs );
		
		return nullptr;
	}


    static bool parseAction_notGround( ifstream &ifs, Domain *dom ){
        StateDescriptor *sd = dom->getStateDescriptor();
        string full_name, act_name, act_type, word;
        ifs >> full_name;  // action name and parameters must be without spaces

        // 1. Parse action local parameters (name, type and index)
        // e.g. drop(b:ball,r:room,g:grippernew)
        vector< string > param_type_list;
        map< string, string > act_param_type;
        map< string, int > act_param_idx;
        size_t p = full_name.find( '(' ) ;
        if( p == string::npos ) {
            cerr << "No parenthesis found in the action name: " << full_name << endl;
            return false;
        }
        act_name = full_name.substr(0, p );
        string str_params = full_name.substr( p+1 , (full_name.length()-1)-(p+1) );
        auto raw_params = split( str_params );
        for( unsigned i = 0; i < raw_params.size(); i++ ){
            auto param_info = split( raw_params[i], ':' );
            act_param_type[ param_info[0] ] = param_info[1];
            act_param_idx[ param_info[0] ] = (int)i;
            param_type_list.push_back( param_info[1] );
        }


        // 2. Parse action type, e.g. "math" or "memory"
        ifs >> word;
        if( word != "TYPE:" ){
            cerr << errorMessage( "TYPE:", word );
            return false;
        }
        ifs >> act_type; // action type must be either "math" or "memory"
        if( act_type != "math" and act_type != "memory" ){
            cerr << "Action type must be either math or memory, and not " << act_type << endl;
            return false;
        }

        // 3. Parse preconditions, e.g. ( at-robby(r) = 1 )
        vector< vector< string > > conditions;
        ifs >> word;
        if( word != "PRECONDITIONS:" ){
            cerr << errorMessage( "PRECONDITIONS:", word );
            return false;
        }
        // Parsing new condition expressions
        ifs >> word;
        while( !ifs.eof() && ( word == "(" ) ){
            //string var_lhs, cond_op, var_rhs;
            vector< string > cond( 3, "" );
            for( unsigned i = 0; i < 3; i++ )
                ifs >> cond[i];

            conditions.push_back( cond );

            ifs >> word;
            if( word != ")" ){
                cerr << errorMessage( ")", word );
                return false;
            }

            ifs >> word;
        }

        // 4. Parse effects
        vector< vector< string > > effects;
        if( word != "EFFECTS:" ){
            cerr << errorMessage( "EFFECTS:", word );
            return false;
        }

        // Parsing new effect expressions
        ifs >> word;
        while( !ifs.eof() && word == "(" ){
            if( word != "(" ){
                cerr << errorMessage( "(", word );
                return false;
            }

            //string var_lhs, op, var_rhs;
            vector< string > eff( 3, "" );
            for( unsigned i = 0; i < 3; i++ )
                ifs >> eff[i];

            effects.push_back( eff );

            ifs >> word;
            if( word != ")" ){
                cerr << errorMessage( "(", word );
                return false;
            }
            ifs >> word;
        }


        // GROUNDING ACTIONS
        // Compute grounded params and add grounded horizontal_action to the domain
        bool external= false;
        if (act_name.find("@")!=std::string::npos){
            act_name = act_name.substr(1, act_name.size() );
            external=true;
        }
        dom->addActionSchema( new Action( act_name, act_type ) );

        Action *act = dom->getActionSchema( act_name );
        act->setActName(act_name);
        act->setPlanningAction(true);
        act->set_external_function(external);
        act->setParamTypeList(param_type_list);
        act->setActionPreconditionsStructure(conditions);
        act->setActionEffectStructure(effects);
        act->setActionParaType(act_param_type);
        act->setActionParaIndex(act_param_idx);

        return true;
    }


	static bool parseAction( ifstream &ifs, Domain *dom ){
	    StateDescriptor *sd = dom->getStateDescriptor();
        string full_name, act_name, act_type, word;
        ifs >> full_name;  // action name and parameters must be without spaces

        // 1. Parse action local parameters (name, type and index)
        // e.g. drop(b:ball,r:room,g:grippernew)
        vector< string > param_type_list;
        map< string, string > act_param_type;
        map< string, int > act_param_idx;
        size_t p = full_name.find( '(' ) ;
        if( p == string::npos ) {
            cerr << "No parenthesis found in the action name: " << full_name << endl;
            return false;
        }
        act_name = full_name.substr(0, p );
        string str_params = full_name.substr( p+1 , (full_name.length()-1)-(p+1) );
        auto raw_params = split( str_params );
        for( unsigned i = 0; i < raw_params.size(); i++ ){
            auto param_info = split( raw_params[i], ':' );
            act_param_type[ param_info[0] ] = param_info[1];
            act_param_idx[ param_info[0] ] = (int)i;
            param_type_list.push_back( param_info[1] );
        }


        // 2. Parse action type, e.g. "math" or "memory"
        ifs >> word;
        if( word != "TYPE:" ){
            cerr << errorMessage( "TYPE:", word );
            return false;
        }
        ifs >> act_type; // action type must be either "math" or "memory"
        if( act_type != "math" and act_type != "memory" ){
            cerr << "Action type must be either math or memory, and not " << act_type << endl;
            return false;
        }

        // 3. Parse preconditions, e.g. ( at-robby(r) = 1 )
        vector< vector< string > > conditions;
        ifs >> word;
        if( word != "PRECONDITIONS:" ){
            cerr << errorMessage( "PRECONDITIONS:", word );
            return false;
        }
        // Parsing new condition expressions
        ifs >> word;
        while( !ifs.eof() && ( word == "(" ) ){
            //string var_lhs, cond_op, var_rhs;
            vector< string > cond( 3, "" );
            for( unsigned i = 0; i < 3; i++ )
                ifs >> cond[i];

            conditions.push_back( cond );

            ifs >> word;
            if( word != ")" ){
                cerr << errorMessage( ")", word );
                return false;
            }

            ifs >> word;
        }

        // 4. Parse effects
        vector< vector< string > > effects;
        if( word != "EFFECTS:" ){
            cerr << errorMessage( "EFFECTS:", word );
            return false;
        }

        // Parsing new effect expressions
        ifs >> word;
        while( !ifs.eof() && word == "(" ){
            if( word != "(" ){
                cerr << errorMessage( "(", word );
                return false;
            }

            //string var_lhs, op, var_rhs;
            vector< string > eff( 3, "" );
            for( unsigned i = 0; i < 3; i++ )
                ifs >> eff[i];

            effects.push_back( eff );

            ifs >> word;
            if( word != ")" ){
                cerr << errorMessage( "(", word );
                return false;
            }
            ifs >> word;
        }

        // GROUNDING ACTIONS
        // Compute grounded params and add grounded horizontal_action to the domain
        vector< string > grounded_params;
        sd->computeGroundedParams(param_type_list,grounded_params);

        for( const auto& gp : grounded_params ){
            vector< string > pointer_names = split( gp );
            string grounded_name = act_name + "(" + gp + ")";

            // Only create grounded horizontal_action
            dom->addAction( new Action( grounded_name, act_type ) );
            Action *act = dom->getAction( grounded_name );
            act->setActName(act_name);
            act->setPlanningAction(true);
            vector<string> effectWord;
            vector<string> condWord;

            for( auto c : conditions ){
                Variable *lhs = groundingVar( sd, c[0], act_param_idx, pointer_names );
                Variable *rhs = groundingVar( sd, c[2], act_param_idx, pointer_names );
                Condition *cond = readCondition( sd, c[1], lhs, rhs );
                act->addCondition( cond );
                if (rhs->getName()=="1"){
                    string name=lhs->getName();
                    condWord.push_back(name.substr(0,name.find("(")));
                }
                if (rhs->getName()=="0"){
                    string name=lhs->getName();
                    condWord.push_back("not-"+name.substr(0,name.find("(")));
                }
            }
            act->setActionPreWord(condWord);

            for( auto eff : effects ){
                Variable *lhs = groundingVar( sd, eff[0], act_param_idx, pointer_names );
                Variable *rhs = groundingVar( sd, eff[2], act_param_idx, pointer_names );
                Operation *oper = readOperation( sd, eff[1], lhs, rhs );
                act->addOperation( oper );
                if (rhs->getName()=="1"){
                    string name=lhs->getName();
                    effectWord.push_back(name.substr(0,name.find("(")));
                }
                if (rhs->getName()=="0"){
                    string name=lhs->getName();
                    effectWord.push_back("not-"+name.substr(0,name.find("(")));
                }
            }
            act->setActionEffectWord(effectWord);
        }

        return true;
	}
	
	// In charge of parsing and creating a domain
	static bool parseDomain( Domain *dom, vector<string> pointers, const string &file_name = "" ){
		ifstream ifs( file_name.c_str() );

		string word;

		// DOMAIN NAME
		ifs >> word;
		
		// Transform the name to lowercase
        std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ return std::tolower(c); });
		dom->setName( word );
		
		// STATE DESCRIPTOR
		if( ifs.eof() ){
			cerr << errorMessage( "STATE", "END OF FILE" ) << endl;
			return false;
		}
		
		ifs >> word;
		
		if( word != "STATE" ){
			cerr << errorMessage( "STATE", word ) << endl;
			return false;
		}
		
		if( ifs.eof() ) {
			cerr << errorMessage( "DESCRIPTOR:", "END OF FILE" ) << endl;
			return false;
		}
		
		ifs >> word;
		
		if( word != "DESCRIPTOR:" ){
			cerr << errorMessage( "DESCRIPTOR:", word ) << endl;
			return false;
		}
		
		dom->setStateDescriptor( new StateDescriptor() );
		
		StateDescriptor *sd = dom->getStateDescriptor();
		
		if( ifs.eof() ){
			cerr << errorMessage( "DESCRIPTOR:", "END OF FILE" ) << endl;
			return false;
		}
		
		ifs >> word;

		// Parsing STATE DESCRIPTOR
		while( !ifs.eof() && word != "ACTION:" ){
			size_t splitter = word.find(':');
			if( splitter == string::npos ) {
                cerr << "Name or type not defined in: " << word << endl;
                return false;
            }
			string v_name = word.substr(0, splitter );
			string v_type = word.substr( splitter+1 );
			if( v_type == "var_type" ){ // adding a new var type
                sd->addVar( v_name );
			}
			else if( v_type == "pred_type" ){ // adding a new predicate type
			    size_t pred_splitter = v_name.find('(' );
			    if( pred_splitter == string::npos or v_name[ (int)v_name.length() - 1 ] != ')'){
			        cerr << "Predicate: " << v_name << " is ill-defined" << endl;
			        return false;
			    }
			    vector< string > param_names;
			    string param_vars = v_name.substr( pred_splitter+1, v_name.length()-1-(pred_splitter+1) );
			    size_t next_param = 0, last_param = 0;
			    while( (next_param = param_vars.find(',',last_param)) != string::npos ){
			        param_names.push_back( param_vars.substr(last_param,next_param-last_param) );
                    last_param = next_param+1;
			    }
			    if( param_vars.size() > size_t(0) )
			        param_names.push_back( param_vars.substr( last_param ) );
			    sd->addPredicate( v_name.substr(0, pred_splitter ), param_names );
			}
			else{ // otherwise, it must be a pointer
                sd->addPointer( v_type, v_name );
			}

			ifs >> word;
		}
        for (auto pointer:pointers){
            size_t splitter = pointer.find(':');
            string v_name = pointer.substr(0, splitter );
            string v_type = pointer.substr( splitter+1 );
            sd->addPointer( v_type, v_name );

        }

		

       // Parsing Action
        while( !ifs.eof() && word == "ACTION:" ){
            if( !parseAction_notGround( ifs, dom ) ) {
                cerr << "Error while parsing the action" << endl;
                return false;
            }
        }
		
		ifs.close();
		
		return true;
	}
	
	static bool parseInstance( Domain *dom, Instance *ins, const string &file_name = "", int instance_id = 0 ){
	    ins->setInstanceID( instance_id );
        StateDescriptor *sd = dom->getStateDescriptor();

		ifstream ifs( file_name.c_str() );
		
		// Instance name
		string word;
		ifs >> word;
		ins->setName( word );

		// 1. Parse objects
        ins->setInitialState( new State( sd ) );
        State *init = ins->getInitialState();
        init->setGraph(new AbstractGraph());
        AbstractGraph *graph=init->getGraph();



        ifs >> word;
        if( word != "OBJECTS:" ){
            cerr << errorMessage( "OBJECTS:", word ) << endl;
            return false;
        }
        ifs >> word;
		while( !ifs.eof() && word != "INIT:" ){
            auto obj_info = split( word, ':' );


            sd->addObject(obj_info[0],obj_info[1]);
            ins->addObject(sd,graph,obj_info[0], obj_info[1] );
            ifs >> word;
            if (obj_info[1]=="size" or obj_info[1]=="color" ){
                Parser::all_objects[instance_id].push_back(sd->getTypeID(obj_info[0]));
            }


            if (obj_info[1]=="node"){
                Parser::node[instance_id].push_back(sd->getTypeID(obj_info[0]));
            }

            if (obj_info[1]=="output-node"){
                Parser::output_node[graph].push_back(sd->getTypeID(obj_info[0]));
            }


		}
        sd->link_object_pointer();

		// 2. Parse initial state
        if( word != "INIT:" ){
            cerr << errorMessage( "INIT:", word ) << endl;
            return false;
        }



        ifs >> word;
        vector<string> initWord;
        vector<string> goalWord;

        while( !ifs.eof() && word == "(" ){
            ifs >> word;
            string groundstate=word;// read grounded predicate
            size_t pos = word.find( '(' );

            if( pos != string::npos ) { // it is a predicate
                string pred_type = word.substr(0, pos);
                // Get object addresses
                if (pred_type=="contain-pixel"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    auto pixel_id=sd->getTypeID(obj_names[1]);
                    graph->getNode(node_id)->setPixel(pixel_id,graph->getPixel(pixel_id));

                }

                if (pred_type=="node-color"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    auto color_id=sd->getTypeID(obj_names[1]);
                    graph->getNode(node_id)->setColor(color_id);

                    Parser::node_color[instance_id].push_back(color_id);


                }

                if (pred_type=="background-color"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto color_id=sd->getTypeID(obj_names[0]);
                    graph->setBackgroundColor(color_id);

                }

                if (pred_type=="color-most"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto color_id=sd->getTypeID(obj_names[0]);
                    graph->setMostColor(color_id);

                }

                if (pred_type=="color-least"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto color_id=sd->getTypeID(obj_names[0]);
                    graph->setLeastColor(color_id);

                }


                if (pred_type=="node-size"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    auto size_id=sd->getTypeID(obj_names[1]);
                    graph->getNode(node_id)->setSize(size_id);

                    Parser::node_size[instance_id].push_back(size_id);

                }

                if (pred_type=="node-shape"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    auto shape_id=sd->getTypeID(obj_names[1]);
                    graph->getNode(node_id)->setShape(shape_id);

                    Parser::node_shape[instance_id].push_back(shape_id);

                }


                if (pred_type=="pixel-color"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto pixel_id=sd->getTypeID(obj_names[0]);
                    auto color_id=sd->getTypeID(obj_names[1]);
                    graph->getPixel(pixel_id)->setColor(color_id);

                }

                if (pred_type=="node-background-color"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    auto color_id=sd->getTypeID(obj_names[1]);
                    graph->getNode(node_id)->setBackgroundColor(color_id);
                }

                if (pred_type=="vec-neighbor"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    auto vec_neighbor=sd->getTypeID(obj_names[1]);
                    graph->getNode(node_id)->setVer(vec_neighbor);
                }

                if (pred_type=="hor-neighbor"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    auto hor_neighbor=sd->getTypeID(obj_names[1]);
                    graph->getNode(node_id)->setHor(hor_neighbor);
                }




                if (pred_type=="contain-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    auto contain_node=sd->getTypeID(obj_names[1]);
                    graph->getNode(node_id)->setContain(contain_node);
                }

                if (pred_type=="same-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    auto same_node=sd->getTypeID(obj_names[1]);
                    graph->getNode(node_id)->setSame(same_node);
                }

                if (pred_type=="same-color"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    auto same_node=sd->getTypeID(obj_names[1]);
                    graph->getNode(node_id)->setSameColor(same_node);
                }



                if (pred_type=="node-similar"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    auto similar_color=sd->getTypeID(obj_names[1]);
                    graph->getNode(node_id)->setSimilar(similar_color);
                }


                if (pred_type=="incomplete-contain-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    auto incomplete_contain_node=sd->getTypeID(obj_names[1]);
                    graph->getNode(node_id)->setIncompleteContain(incomplete_contain_node);
                }

                if (pred_type=="size-max"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setMaxSize();
                }

                if (pred_type=="size-min"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setMinSize();
                }


                if (pred_type=="color-max"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setMostColor();
                }

                if (pred_type=="color-min"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setMinColor();
                }

                if (pred_type=="odd-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setOdd();

                    Parser::odd_node[instance_id].push_back(node_id);

                }

                if (pred_type=="even-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setEven();

                    Parser::even_node[instance_id].push_back(node_id);


                }


                if (pred_type=="left-diagonal-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setLeftDiagonalNode();

                }
                if (pred_type=="right-diagonal-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setRightDiagonalNode();

                }
                if (pred_type=="up-border-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setUpBorderNode();

                }
                if (pred_type=="down-border-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setDownBorderNode();

                }
                if (pred_type=="left-border-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setLeftBorderNode();

                }

                if (pred_type=="right-border-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setRightBorderNode();

                }

                if (pred_type=="vertical-middle-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setVerticalMiddleNode();

                }

                if (pred_type=="horizontal-middle-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);
                    graph->getNode(node_id)->setHorizontalMiddleNode();

                }




                if (pred_type=="node-diagonal"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id1=sd->getTypeID(obj_names[0]);
                    auto node_id2=sd->getTypeID(obj_names[1]);
                    graph->getNode(node_id1)->setDiagonal(node_id2);

                }



                if (pred_type=="node-spatial-relation"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id1=sd->getTypeID(obj_names[0]);
                    auto node_id2=sd->getTypeID(obj_names[1]);
                    auto move_direction=sd->getTypeID(obj_names[2]);
                    if (obj_names[2]=="left"){
                        graph->getNode(node_id1)->setLeft(node_id2);
                    }
                    if (obj_names[2]=="right"){
                        graph->getNode(node_id1)->setRight(node_id2);
                    }
                    if (obj_names[2]=="up"){
                        graph->getNode(node_id1)->setUp(node_id2);
                    }
                    if (obj_names[2]=="down"){
                        graph->getNode(node_id1)->setDown(node_id2);
                    }
                    if (obj_names[2]=="left-up"){
                        graph->getNode(node_id1)->setLeftUp(node_id2);
                    }
                    if (obj_names[2]=="left-down"){
                        graph->getNode(node_id1)->setLeftDown(node_id2);
                    }
                    if (obj_names[2]=="right-up"){
                        graph->getNode(node_id1)->setRightUp(node_id2);
                    }
                    if (obj_names[2]=="right-down"){
                        graph->getNode(node_id1)->setRightDown(node_id2);
                    }

                }


                auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                vector<int> obj_idxs( obj_names.size() );


                for (int oname_idx = 0; oname_idx < (int)obj_names.size(); oname_idx++ ) {
                    obj_idxs[oname_idx] = ins->getObjectAddress(obj_names[oname_idx] );

                }


                // Currenty, the initial states are only assignments
                ifs >> word;
                assert(word == "=");

                int value;
                ifs >> value;
                if (value==1){
                    initWord.push_back(groundstate);
                }
                if (value==0){
                    initWord.push_back("not-"+groundstate);
                }
                init->addRegister(sd, pred_type, obj_idxs, value);

            }
            else{ // otherwise, it must be a pointer
                string pointer_name = word;
                assert( sd->isPointer( pointer_name ) );
                ifs >> word;
                assert( word == "=" );
                int value;
                ifs >> value;
                init->setPointer(sd, pointer_name, value );
            }

            ifs >> word;
            if( word != ")" ){
                cerr << errorMessage( "(", word );
                return false;
            }
            ifs >> word;
        }

		// 3. Parse goal condition
        if( word != "GOAL:" ){
            cerr << errorMessage( "GOAL:", word ) << endl;
            return false;
        }

        ifs >> word;

        while( !ifs.eof() && word == "(" ){
            string lhs_name, cond_op, rhs_name;

            ifs >> lhs_name >> cond_op >> rhs_name;
            goalWord.push_back(lhs_name);

            size_t pos = lhs_name.find( '(' );
            string pred_type = lhs_name.substr(0, pos);

            if (pred_type=="pixel-color"){
                auto obj_names = split(lhs_name.substr(pos + 1, lhs_name.length() - 1 - (pos + 1)));
                auto pixel_id=sd->getTypeID(obj_names[0]);
                auto color_id=sd->getTypeID(obj_names[1]);
                graph->addGoalImagePixel(pixel_id,color_id);
            }

            if (pred_type=="contain-pixel-output"){
                auto obj_names = split(lhs_name.substr(pos + 1, lhs_name.length() - 1 - (pos + 1)));
                auto node_id=sd->getTypeID(obj_names[0]);
                auto pixel_id=sd->getTypeID(obj_names[1]);
                graph->getOutputNode(node_id)->setPixel(pixel_id,graph->getPixel(pixel_id));

                ifs >> word;
                if( word != ")" ){
                    cerr << errorMessage( "(", word );
                    return false;
                }
                ifs >> word;

                continue;

            }


            Variable *lhs = readGroundedVar(sd, lhs_name, ins );
            Variable *rhs = readGroundedVar(sd, rhs_name, ins );
            Condition *cond = readCondition(sd, cond_op, lhs, rhs );

            ins->addCondition( cond );


            ifs >> word;
            if( word != ")" ){
                cerr << errorMessage( "(", word );
                return false;
            }
            ifs >> word;
        }

        vector<string>::iterator it;
        for(it=goalWord.begin();it!=goalWord.end();)
        {
            if ( std::find(initWord.begin(), initWord.end(), *it) != initWord.end() )
                it=goalWord.erase(it);
            else
                ++it;
        }

        ins->setgoalWord(goalWord);
        ins->setinitWord(initWord);

        ifs.close();

        return true;
	}



    static bool parseTestInstance( Domain *dom, const string &file_name = "", int instance_id = -1 ) {

        StateDescriptor *sd = dom->getStateDescriptor();

        ifstream ifs(file_name.c_str());

        // Instance name
        string word;
        ifs >> word;


        ifs >> word;
        if (word != "OBJECTS:") {
            cerr << errorMessage("OBJECTS:", word) << endl;
            return false;
        }
        ifs >> word;
        while (!ifs.eof() && word != "INIT:") {
            auto obj_info = split(word, ':');

            ifs >> word;
            if (obj_info[1] == "size" or obj_info[1] == "color") {
                Parser::all_objects[instance_id].push_back(sd->getTypeID(obj_info[0]));
            }


            if (obj_info[1] == "node") {
                Parser::node[instance_id].push_back(sd->getTypeID(obj_info[0]));
            }


        }



        ifs >> word;

        while( !ifs.eof() && word == "(" ){
            ifs >> word;
            string groundstate=word;// read grounded predicate
            size_t pos = word.find( '(' );

            if( pos != string::npos ) { // it is a predicate
                string pred_type = word.substr(0, pos);
                // Get object addresses

                if (pred_type == "node-color") {
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id = sd->getTypeID(obj_names[0]);
                    auto color_id = sd->getTypeID(obj_names[1]);

                    Parser::node_color[instance_id].push_back(color_id);


                }


                if (pred_type == "node-size") {
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id = sd->getTypeID(obj_names[0]);
                    auto size_id = sd->getTypeID(obj_names[1]);

                    Parser::node_size[instance_id].push_back(size_id);

                }

                if (pred_type == "node-shape") {
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id = sd->getTypeID(obj_names[0]);
                    auto shape_id = sd->getTypeID(obj_names[1]);

                    Parser::node_shape[instance_id].push_back(shape_id);

                }

                if (pred_type=="odd-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);

                    Parser::odd_node[instance_id].push_back(node_id);

                }

                if (pred_type=="even-node"){
                    auto obj_names = split(word.substr(pos + 1, word.length() - 1 - (pos + 1)));
                    auto node_id=sd->getTypeID(obj_names[0]);

                    Parser::even_node[instance_id].push_back(node_id);


                }


                ifs >> word;
                assert(word == "=");

                int value;
                ifs >> value;

            }
            else{ // otherwise, it must be a pointer
                ifs >> word;
                assert( word == "=" );
                int value;
                ifs >> value;
            }

            ifs >> word;
            if( word != ")" ){
                cerr << errorMessage( "(", word );
                return false;
            }
            ifs >> word;
        }
        ifs.close();

        return true;
    }

    void findRemovedTest(Domain *dom){

        vector<int> tmp;
        //its.first==-1 means all objects in test instance
        // it.first!=-1 means all objects in training instance
        for (auto its:Parser::all_objects){
            if (its.first==-1) continue;
            for (auto it :its.second){
                tmp.push_back(it);
            }
        }

        for (auto obj: tmp){
            if (std::find(Parser::all_objects[-1].begin(), Parser::all_objects[-1].end(),obj)==Parser::all_objects[-1].end()){
                dom->setPrune(obj);
            }
        }

        vector<int> tmp_size;
        vector<int> tmp_shape;
        vector<int> tmp_color;
        vector<int> tmp_even;
        vector<int> tmp_odd;
        vector<int> tmp_node;



        for (auto its:Parser::node_size){
            for (auto it :its.second){
                if (std::find(tmp_size.begin(), tmp_size.end(),it)==tmp_size.end())
                    tmp_size.push_back(it);
            }
        }



        for (auto its:Parser::node_color){
            for (auto it :its.second){
                if (std::find(tmp_color.begin(), tmp_color.end(),it)==tmp_color.end())
                    tmp_color.push_back(it);
            }
        }


        for (auto its:Parser::node_shape){
            for (auto it :its.second){
                if (std::find(tmp_shape.begin(), tmp_shape.end(),it)==tmp_shape.end())
                    tmp_shape.push_back(it);
            }
        }





        for (auto its:Parser::even_node){
            for (auto it :its.second){
                    tmp_even.push_back(it);
            }
        }

        for (auto its:Parser::odd_node){
            for (auto it :its.second){
                    tmp_odd.push_back(it);
            }
        }

        for (auto its:Parser::node){
            for (auto it :its.second){
                    tmp_node.push_back(it);
            }
        }



        if (tmp_size.size()==1)
            dom->setPruneType("node-size");

        if (tmp_shape.size()==1)
            dom->setPruneType("node-shape");

        if (tmp_color.size()==1)
            dom->setPruneType("node-color");

        if (tmp_even.size()==tmp_node.size())
            dom->setPruneType("even-node");

        if (tmp_odd.size()==tmp_node.size())
            dom->setPruneType("odd-node");


        dom->setPruneType("contain-pixel");
        dom->setPruneType("pixel-color");
        dom->setPruneType("node-background-color");
        dom->setPruneType("background-color");
        dom->setPruneType("contain-pixel-output");

        dom->setPruneType("node-spatial-relation");

    }


    static vector<string> preprocessDefaultAction(string pointer_file) {
        vector<string> action_plans;
        string program_file = "./default_actions/"+pointer_file;
        vector<string> program_instructions;
        string instruction;
        ifstream ifs_program;
        ifs_program.open(program_file.data());
        assert(ifs_program.is_open());

        while (getline(ifs_program, instruction)) {
            if (instruction.size() == 0u) continue;
            instruction = instruction.substr(instruction.find_first_of(". ") + 2);
            program_instructions.push_back(instruction);

        }
        return  program_instructions;
    }


    static vector<Instruction*> parseDefaultAction(Domain *dom,vector<string> program_instructions ){
        int program_lines = int(program_instructions.size());
        vector<Instruction *> actions;
        for (int l = 0; l < program_lines; l++) {
            auto position_destination = program_instructions[l].find("@=");
            auto clear_destination = program_instructions[l].find("=");
            auto position_pointer = program_instructions[l].find(" = ");
            auto position_destination_end = program_instructions[l].find(",");
            if (position_destination != string::npos) {
                auto possible_destination = stoi(program_instructions[l].substr(position_destination + 2,
                                                                                        position_destination_end -
                                                                                        (position_destination + 2)));
                auto pointer = program_instructions[l].substr(position_pointer - 2, 2);
                auto constant = stoi(program_instructions[l].substr(position_pointer + 3, 1));
                Goto *ins = new Goto(possible_destination);

                vector<Condition *> _conds;
                auto lhs = new Variable(pointer, "pointer", dom->getStateDescriptor()->getTypeID(pointer));
                auto rhs = new Variable(to_string(constant), "constant", constant);
                Condition *goto_cond = new Equals(dom->getStateDescriptor(), lhs, rhs);
                ins->addCond(goto_cond);
                ins->setVariable(possible_destination);
                actions.push_back(ins);
                continue;

            }

            if (position_destination == string::npos and clear_destination != string::npos) {
                auto destination = stoi(program_instructions[l].substr(clear_destination + 1,
                                                                               position_destination_end -
                                                                               (clear_destination + 1)));
                auto pointer = program_instructions[l].substr(position_pointer - 2, 2);
                auto constant = stoi(program_instructions[l].substr(position_pointer + 3, 1));
                Goto *ins = new Goto(destination);

                vector<Condition *> _conds;
                auto lhs = new Variable(pointer, "pointer", dom->getStateDescriptor()->getTypeID(pointer));
                auto rhs = new Variable(to_string(constant), "constant", constant);
                Condition *goto_cond = new Equals(dom->getStateDescriptor(), lhs, rhs);
                ins->addCond(goto_cond);
                actions.push_back(ins);
                continue;

            }


            Action *act = new Action(program_instructions[l]);
            Instruction *inc_ins = new PlanningAction(act);
            actions.push_back(inc_ins);

        }
        return actions;
    }
	
};


map<int,vector<int> > Parser::all_objects;

map<int, vector<int>> Parser::node_size;

map<int, vector<int>> Parser::node_shape;

map<int, vector<int>> Parser::node_color;

map<int, vector<int>> Parser::even_node;

map<int, vector<int>> Parser::odd_node;

map<int, vector<int>> Parser::node;

map<AbstractGraph*, vector<int>> Parser::output_node;
#endif
