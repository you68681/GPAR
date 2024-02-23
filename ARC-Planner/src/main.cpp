#include <dirent.h>
#include "common.h"
#include "domain.h"
#include "instance.h"
#include "parser.h"
#include "generalized_planning_problem.h"
#include "program.h"
#include "node.h"
#include "generalized_domain.h"
#include "heuristic.h"
#include "engine.h"

int main(  int argc, const char* argv[] ){
	if( argc < 5 ){
		cout << "[ERROR] Incorrect input." << endl;
		cout << "[INFO] Execute: ./main.bin <PGP> <program lines> <novelty> <pointers> <problem training folder> <problem testing folder> <list of heuristic names>" << endl;
		cout << "[INFO] Example: ./main.bin PGP 5 1 no1:node,co1:color  ARC-benchmark/new_color_task/aabf363d/re4_training/ ARC-benchmark/new_color_task/aabf363d/re4_testing/  hcount hcheck" << endl;
		return -1;
	}

	int precision = 3;
	cout.setf( ios::fixed );
	cout.precision( precision );

	time_t start, parse_time;
	time( &start );

    string search_mode = string(argv[1]); // BFS or PGP
    if( search_mode != "PGP" ){
        cout << "[ERROR] Search mode is wrong, either use BFS or PGP.\n";
        cout << "[INFO] Execute: ./main.bin <PGP> <program lines> <novelty> <pointers> <problem training folder> <problem testing folder> <list of heuristic names>" << endl;
        cout << "[INFO] Example: ./main.bin PGP 5 1 no1:node,co1:color  ARC-benchmark/new_color_task/aabf363d/re4_training/ ARC-benchmark/new_color_task/aabf363d/re4_testing/  hcount hcheck" << endl;
        return -10;
    }
    bool is_progressive = (search_mode=="PGP"); // BFS => progressive = false; PGP => progressive = true;

	int program_lines = stoi( argv[2] );

    int width_line = stoi( argv[3] );

    string pointer_indicator = string ( argv[4] );

	string problem_folder = string( argv[5] );

    string test_folder = string( argv[6] );

    // process the input pointer
    vector<std::string> pointers;
    stringstream ss(pointer_indicator);
    while(ss.good())
    {
        std::string substr;
        getline(ss, substr, ',');
        pointers.push_back(substr);
    }

    string pointer_file;

    // match the input pointer to the default pointer file
    for (auto pointer: pointers){
        size_t pointer_pos = pointer.find_first_of( ':' );
        string pointer_ind = pointer.substr( 0, pointer_pos);
        if (pointer_file.empty()){
            pointer_file+=pointer_ind;
        }
        else{
            pointer_file+="_"+pointer_ind;
        }

    }

    vector< string > heuristic_names( argc - 7 );
    string heuristics;
    for( int i = 7; i < argc; i++ ) {
        heuristic_names[ i-7 ] = string(argv[i]);
        heuristics += "_" + heuristic_names[i-7];
    }
    if( argc == 7 ){ // default heuristics
        heuristic_names.emplace_back("hp" );
        heuristic_names.emplace_back("hln" );
        heuristics = "_hp_hln";
    }
	
	ifstream ifs_domain( problem_folder + "domain.txt" );
	if( !ifs_domain ){
		cout << "[ERROR] domain.txt does not exist." << endl;
		return -2;
	}
	ifs_domain.close();
	
	auto *parser = new Parser();
	auto *dom = new Domain();
    dom->setWidth(width_line);
		
	if( !parser->parseDomain( dom, pointers, problem_folder + "domain.txt" ) ){
		cout << "[ERROR] while parsing domain.txt" << endl;
		delete dom;
		delete parser;
		return -3;
	}

    size_t lastSlashPos = problem_folder.rfind('/');
    size_t secondLastSlashPos = problem_folder.rfind('/', lastSlashPos - 1);
    size_t thirdLastSlashPos = problem_folder.rfind('/', secondLastSlashPos - 1);

    std::string domain_name = problem_folder.substr(thirdLastSlashPos + 1, secondLastSlashPos - thirdLastSlashPos - 1);

    std::string abstraction_name = problem_folder.substr(secondLastSlashPos + 1, lastSlashPos - secondLastSlashPos - 1);


    string outfile = "./experiments/" + domain_name+"_"+ abstraction_name +"_"+pointer_file+ "_" + to_string(program_lines) +"_"+to_string(width_line) + heuristics;


	time( &parse_time );
    if(PRINT)
	cout << "[INFO] Parsed domain. [" << difftime( parse_time, start ) << "]" << endl;
	
	// Generating the GP problem
	auto *gpp = new GeneralizedPlanningProblem();
	gpp->addDomain( dom );
    gpp->setProgressive( is_progressive );
	
	int i = 1;

    while( true ){
        auto *ins = new Instance();
        string input_instance = problem_folder + to_string( i ) + ".txt";
        i++;

        ifstream ifs_instance( input_instance );
        if( !ifs_instance ){
            delete ins;
            break;
        }
        ifs_instance.close();


        if( parser->parseInstance( dom, ins, input_instance, i-2 ) )
            gpp->addInstance( ins );
        else{
            cout << "[ERROR] while parsing " << input_instance << endl;
            delete ins;
            delete gpp;
            delete parser;
            return -4;
        }
    }

    i = 1;
    while( true ){
        string input_instance = test_folder + to_string( i ) + ".txt";
        i++;

        ifstream ifs_instance( input_instance );
        if( !ifs_instance ){
            break;
        }
        ifs_instance.close();


        parser->parseTestInstance( dom,  input_instance) ;
    }


	// After reading all instances, update the bounds for each predicate in every instance.
	gpp->updatePredicateBounds();



	time_t gpp_time;
	time( &gpp_time );
    if (PRINT)
	cout << "[INFO] Generalized Planning Problem created. [" << difftime( gpp_time, parse_time ) << "]" << endl;

    // predicate constraint and argument constraint
    parser->findRemovedTest(dom);

    // read the default pointer operations from pointer operation file.
    auto default_actions=parser->preprocessDefaultAction(pointer_file);

    // real program line in the output planning program (+1 for the end instruction programmed at the last time)

    program_lines=program_lines+default_actions.size()+1;

	auto *gd = new GeneralizedDomain( dom, program_lines,gpp->constraints_acquisition_global() );


	time_t gd_time;
	time( &gd_time );


	auto *engine = new BFS( program_lines, gd, gpp );
    engine->setTime(start);

    if(DESIGNED_ACTIONS){
        // parser the string pointer operations to instructions (string-> instruction) used in the looping section.
        auto possible_actions=parser->parseDefaultAction(dom,default_actions);

            for (int l = 0; l < possible_actions.size(); l++) {
                auto *act = dynamic_cast<PlanningAction *>( possible_actions[l] );
                if (act) {
                    Instruction *ins = gd->searchInstruction(l, possible_actions[l]->getName());
                    if (ins == NULL) {
                        cout << "[ERROR] Instruction " << possible_actions[l]->getName() << " not found." << endl;
                    }
                    possible_actions[l] = ins;
                }
            }

        gd->setPossibleActions(possible_actions);
        gpp->setPossibleActions(possible_actions);
    }


	time_t engine_time;
	time( &engine_time );

    for( const auto& h_name : heuristic_names ){
        if( h_name == "hp" ) engine->addHeuristic( new Hpluscount() );
        else if( h_name == "hln" ) engine->addHeuristic( new Hplusline() );
        else if( h_name == "hcn" ) engine->addHeuristic( new Hpluscheck() );
        else{
            delete engine;
            delete parser;
            cout << "[ERROR] heuristic " << h_name << " is unknown." << endl;
            return -5;
        }
        heuristics += "_" + h_name;
    }

    gpp->getDomain()->getStateDescriptor()->setNewTypeID();

	time_t init_search_time;
	time( &init_search_time );
	cout << "[INFO] Searching... [" << difftime(init_search_time,engine_time) << "]" << endl;

	Node *res = engine->solve(is_progressive);

	time_t end_time;
	time( &end_time );
    ostringstream total_time, search_time;
    total_time.setf(ios::fixed); search_time.setf(ios::fixed);
    total_time.precision( precision ); search_time.precision( precision);
    total_time << difftime(end_time, start); search_time << difftime(end_time, init_search_time );

	string output;
    for (auto actions: gd->getRelaxedActions()){
        for (auto action : actions){
            output+=action+" ";
        }
        output+="\n";
    }
    cout<<"start output"<<endl;
	
	if( res != nullptr ){
		Program *p = res->getProgram();

		ofstream ofs_prog( outfile + ".prog" );
		ofs_prog << p->toString( false );
		ofs_prog.close();

		
		output += "[INFO] SOLUTION FOUND!!!\n";
        output += "[INFO] Expanded nodes: " + to_string( engine->getExpanded() ) + "\n";
        output += "[INFO] Evaluated nodes: " + to_string( engine->getEvaluated() ) + " (deadends: " + to_string(engine->getDeadends()) + ")\n";

        output += "[INFO] Evaluated states: " + to_string( engine->getEvaluatedStates() ) + "\n";
		output += p->toString( false )  + "\n";
		output += "[INFO] Total time: " + total_time.str() + "\n";
        output += "[INFO] Search time: " + search_time.str() + "\n";
	
		cout << output;	
		cout << "[INFO] Program file: " << outfile << ".prog" << endl;	
	}	
	else{
		output += "[INFO] SOLUTION NOT FOUND :(\n";
		output += "[INFO] Expanded nodes: " + to_string( engine->getExpanded() ) + "\n";
		output += "[INFO] Evaluated nodes: " + to_string( engine->getEvaluated() ) + " (deadends: " + to_string(engine->getDeadends()) + ")\n";
        output += "[INFO] Evaluated states: " + to_string( engine->getEvaluatedStates() ) + "\n";
        output += "[INFO] Total time: " + total_time.str() + "\n";
        output += "[INFO] Search time: " + search_time.str() + "\n";
		cout << output;
	}
    cout<<"end output"<<endl;
	ofstream ofs( outfile + ".out" );
	ofs << output;
	ofs.close();
	
	cout << "[INFO] Experiments file: " << outfile << ".out" << endl;	
	
	delete engine;
	delete parser;
}
