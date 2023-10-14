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
#include <unordered_map> 

// Non-STL standard libraries:
#include <sys/stat.h>


// BOOST Library Functionality as Needed:
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split

// Custom Inclusions: 
#include "jlp.h"        // Class Defs;
#include "clip.h"       // Command Line Parsing;  // Replacing with boost; 

// Namespacing: 
using namespace boost;
using namespace boost::program_options;
using namespace std;

int main(int argc, char * argv[])
{
    char const *log_folder = "./logs/";
    mkdir(log_folder, 0755); 

    string filename;
    string delimiter = ",";
    bool dump = false;
    /******************** 
     *  BEGIN CLI Parsing: 
     *      Command Line Options & Parsing - Using BOOST Librariers 
    *********************/
    options_description desc("Allowed options");
    desc.add_options()
    ("help,h", "Produce this help message.")
    ("output-file,o", value< vector<string> >(), "Specifies output file.")
    ("input-file,i", value< vector<string> >(), "Specifies input file.")
    ("log-level,l", value< vector<string> >(), "Specify Log Level Filter: eg. [... WARNING|SEVERE|INFO|FINE ...]")
    ("dump,d", "Print out the raw data.");

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
    if (vm.count("log-level")) {
        vector<string> filters_raw = vm["log-level"].as< vector<string> >();
        vector<string> filters; 
        //boost::split(filters, filters_raw, boost::is_any_of(","), boost::algorithm::token_compress_on);
        for ( long unsigned int i=0; i< filters_raw.size(); i++) {
            stringstream ss(filters_raw[i]);
            string s;
            while (getline(ss, s, ',')) {
                filters.push_back(s);
            }
            // TODO: 
            // Add logical operators to compare to enumeration variable of log levels; 
            // Set internal flags for printing out ONLY those log entries + stack traces
            // That are part of the filtered and selected Log Levels; 
        }
        cout << filters.size () << " Filters applied:" << endl;
        for ( long unsigned int i=0; i< filters.size(); i++) {
            cout << "\tFilter " << i+1 << ":\t" << filters[i] << endl;
        }
    }
    if (vm.count("dump")) {
        dump = true; 
    }
    if (!vm.count("input-file")) {
        cout << "ERROR: No File Specified for Parsing!" << endl;
        cout << desc << endl;
    }

    /******************** 
     * END CLI Parsing: 
    *********************/

    /**********************
     * Let the Log Parsing Begin!
    ***********************/
    // Currently only single file, lines, lol; 
    // TODO:  Add Iteration of multiple-files; 
    //      To aggregate or not to aggregate? -._.-*`*-._.-> To Ponder <-._.-*`*-._.-
    //      So far, should retain 1:1 <javaLogParser>:<file> mapping; 
    if (!filename.empty()) {
        javaLogParser jlp = javaLogParser(filename);
        if(dump){ jlp.dump(); }
    }
    else {
      cout << "Error Loading File" << endl; 
    }
}
