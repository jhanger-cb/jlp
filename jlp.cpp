// STL Inclusions: 
#include <fstream>
#include <iterator>
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <regex>
#include <ctime>
#include <chrono>
#include <exception>

// BOOST Library Functionality as Needed:
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>

// Custom Inclusions: 
#include "jlp.h"        // Class Defs;
#include "clip.h"       // Command Line Parsing;  // Replacing with boost; 

// Namespacing: 
using namespace boost;
using namespace boost::program_options;
using namespace std;
//namespace po = boost::program_options; 

int main(int argc, char * argv[])
{
    string filename;
    /******************** 
     *  BEGIN CLI Parsing: 
     *      Command Line Options & Parsing - Using BOOST Librariers 
    *********************/
    options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "Produce this help message.")
    ("output-file,o", value< vector<string> >(),
     "Specifies output file.")
    ("input-file,i", value< vector<string> >(),
     "Specifies input file.");

    positional_options_description p;
    p.add("input-file", -1);

    variables_map vm;
    try {
        store(command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        notify(vm);
    } catch (std::exception &e) {
        cout << endl << e.what() << endl;
        cout << desc << endl;
    }

    // Display help text when requested
    if (vm.count("help")) {
        cout << "Usage: " << argv[0] << " [OPTIONS] <filename>" << endl;
        cout << desc << endl;
        return 0;
    }

    // Display the state of the arguments supplied
    if (vm.count("output-file")) {
        vector<string> outputFilename =
            vm["output-file"].as< vector<string> >();
        cout << "–output-file specified with value = " << outputFilename[0] << endl;
    }
    if (vm.count("input-file")) {
        vector<string> inputFilename =
            vm["input-file"].as< vector<string> >();
        filename = inputFilename[0];
    }
    /******************** 
     * END CLI Parsing: 
    *********************/

    /************
     * Let the Parsing Begin!
    *************/
    if (!filename.empty()) {
        javaLogParser jlp = javaLogParser(filename);
        //jlp.dump();
    }
    else {
      cout << "Error Loading File" << endl; 
    }
}
