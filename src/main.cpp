#include "headers/clause.h"
#include "headers/solver.h"
#include "headers/util.h"

using namespace std;

int main(int argc, char* argv[]){
    // command line inputs
    string input_filename;
    int time_limit;

    ifstream input_file;
    string line;
    Solver sol;
    vector<int> c;
    string fmt;
    int num_bvar;
    int num_clause;
    time_t timer;

    int result = -1; // default as unknown
    bool verbose = false;

    // handle input
    // $ solver filename
    if(argc == 2){
        input_filename = argv[1];
        time_limit = INT_MAX; // very long run time
    } 
    // $ solver filename --verbose
    else if(argc == 3 && !strcmp(argv[2],"--verbose")){
        input_filename = argv[1];
        verbose = true;
        time_limit = INT_MAX; // very long run time
    }
    // $ solver filename --time i
    else if(argc == 4 && !strcmp(argv[2],"--time")){
        input_filename = argv[1];
        time_limit = stoi(argv[3]);
    }
    // $ solver filename --time i --verbose
    // $ sovler filename --verbose --time i
    else if(argc == 5){
        input_filename = argv[1];
        if(!strcmp(argv[2],"--time")){
            time_limit = stoi(argv[3]);
            verbose = true;
        }
        if(!strcmp(argv[2],"--verbose")){
            time_limit = stoi(argv[4]);
            verbose = true;
        }
    }
    else {
        cout << "Usage: " << argv[0] << " <input_filename> [--time <time_limit(sec)>] [--verbose]" << endl; 
        return 0;
    }

    input_file.open(input_filename);
    
    if(!input_file.is_open()){
        cout << "ERROR: Failed to open the file" << endl;
        return 0;
    }
    
    while(getline(input_file, line)){
        if(line[0] == 'c'){
            continue;
        } else if(line[0] == 'p'){
            parseHeader(line, ' ', fmt, num_bvar, num_clause);
            sol.setFormat(fmt, num_bvar, num_clause);
        } else{
            c = split(line, ' ');
            c.pop_back(); // get rid of the last 0;
            sol.insertClause(c);
        }
    }

    input_file.close();

    //sol.printClausesInfo();

    time(&timer);
    result = sol.CDCL(timer, time_limit);

    if(verbose){
        // sat
        if(result == 1){
            sol.printAssignment();
        }
        // unsat
        else if(result == 0){
            sol.printLastLearntClause();
        }
        // unknown
        else{
            // current stack of decision literals
            sol.printDecision();
        }
    }
    
    
    return 0;
}
