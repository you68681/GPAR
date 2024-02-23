#define VALIDATOR
//#define DEBUG
#include "common.h"
#include "domain.h"
#include "instance.h"
#include "parser.h"
#include "generalized_planning_problem.h"
#include "program_state.h"
#include "instruction.h"
#include "program.h"
#include "generalized_domain.h"


int main(  int argc, const char* argv[] ){
	if( argc < 3 ){
		cout << "[ERROR] Incorrect input." << endl;
		cout << "[INFO] Execute: ./validator.bin <program_file> <problem_folder/> (INF|NOINF)?" << endl;
		cout << "[INFO] Example: ./validator.bin triangular-sum.prog domains/validation/triangular-sum/" << endl;
		cout << "[INFO] The domain file must be domain.txt with all instances numbered from 3.txt to [n].txt" << endl;
		return -1;
	}

    int precision = 3;
    cout.setf( ios::fixed );
    cout.precision( precision );

    time_t start, parse_time;
    time( &start );

	bool infinite_detection = false;
	
	if( argc == 5 && string(argv[ 4 ]) == "INF" ){
		infinite_detection = false;
		cout << "[INFO] Validating with INFINITE detection." << endl;
	}
	else cout << "[INFO] Validating without INFINITE detection." << endl;


	string program_file = string( argv[ 1 ] );
	ifstream ifs_program( program_file );
	
	if( !ifs_program ){
		cout << "[ERROR] the program " << program_file << " does not exist." << endl;
		return -1;
	}
	
	vector< string > program_instructions;
	string instruction;
	while( getline( ifs_program, instruction ) ){
		if( instruction.size() == 0u ) continue;
		instruction = instruction.substr( instruction.find_first_of( ". " ) + 2 );
		program_instructions.push_back( instruction );
	}
		
	int program_lines = int( program_instructions.size() );	
	
	string problem_folder = string( argv[ 2 ] );
	ifstream ifs_domain( problem_folder + "domain.txt" );
	if( !ifs_domain ){
		cout << "[ERROR] domain.txt does not exist." << endl;
		return -2;
	}
	ifs_domain.close();

    string pointer_indicator = string ( argv[3] );

    // process the input pointer
    vector<std::string> pointers;
    stringstream ss(pointer_indicator);
    while(ss.good())
    {
        std::string substr;
        getline(ss, substr, ',');
        pointers.push_back(substr);
    }


    Parser *parser = new Parser();
	Domain *dom = new Domain();


//    if( !parser->parseDomain( dom, problem_folder + "domain.txt" ) ){
    if(  !parser->parseDomain( dom, pointers, problem_folder + "domain.txt" )){
		cout << "[ERROR] while parsing domain.txt" << endl;
		delete dom;
		delete parser;
		return -3;
	}

    time( &parse_time );
    cout << "[INFO] Parsed domain. [" << difftime( parse_time, start ) << "]" << endl;
	
	// Generating the GP problem
	GeneralizedPlanningProblem *gpp = new GeneralizedPlanningProblem();
	gpp->addDomain( dom );
	
	int i = 1;	
	while( true ){
		Instance *ins = new Instance();
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
			delete ins;
			cout << "[ERROR] while parsing " << input_instance << endl;
			return -4;
		}
	}

    time_t gpp_time;
    time( &gpp_time );
    cout << "[INFO] Generalized Planning Problem created. [" << difftime( gpp_time, parse_time ) << "]" << endl;
	
	GeneralizedDomain *gd = new GeneralizedDomain( dom, program_lines,gpp->constraints_acquisition_global() );

    time_t gd_time;
    time( &gd_time );
    cout << "[INFO] Generalized Domain created. [" << difftime( gd_time, gpp_time ) << "]" << endl;
			
	Program *p = new Program( program_lines );
	for( int l = 0; l < program_lines; l++ ){
		Instruction *ins = gd->searchInstruction( l, program_instructions[ l ] );
		if( ins == NULL ){
			cout << "[ERROR] Instruction " << program_instructions[ l ] << " not found." << endl;
			return -5;
		}
		p->setInstruction( l, ins, false);
	}

	time_t prog_time;
	time( &prog_time );
	cout << "[INFO] Program loaded. [" << difftime( prog_time, gd_time ) << "]" << endl;
	cout << "[INFO] Read program: " << endl;
	cout << p->toString( false ) << endl;

    string output;

	vector< ProgramState* > vps = p->run( gpp, infinite_detection, false );

	if( int( vps.size() ) > 0 ){
		for( int l = 0; l < int( vps.size() ); l++ ){
			#ifdef DEBUG
			cout << "INSTANCE #" << (l+1) << endl;
			//cout << vps[ l ]->toString(dom->getStateDescriptor()) << endl;
			#endif
			if( vps[ l ] )
				delete vps[ l ];
		}
        output += "[INFO] GOAL ACHIEVED!\n";
	}
	else{
		output += "[INFO] INVALID GENERAL PLAN :(\n";
	}

	time_t end_time;
	time( &end_time );
	output += "[INFO] Total time: " + to_string(difftime(end_time, start )) + "\n";

    cout << output << endl;

    size_t it_prog_name = program_file.find_last_of('/')+1;
    string outfile_name = program_file.substr(it_prog_name, program_file.size() - it_prog_name - 5) + ".out";
    string outfile = "../experiments/validation/" + outfile_name;
    ofstream ofs( outfile );
    ofs << output;
    ofs.close();

	if( p ) delete p;
	if( gd ) delete gd;
	if( gpp ) delete gpp;
	if( parser ) delete parser;
}
