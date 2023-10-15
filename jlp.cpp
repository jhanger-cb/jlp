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
#include <thread> 

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
    string filtersRaw = "";

    static bool dump = false;
    static bool aggregate = false;
    static bool serialize = false;
    static bool stats = false;
    /******************** 
     *  BEGIN CLI Parsing: 
     *      Command Line Options & Parsing - Using BOOST Librariers 
    *********************/
    options_description desc("Allowed options");
    desc.add_options()
    //("output-file,o", value< vector<string> >(), "Specifies output file.")
    ("input-file,i", value< vector<string> >(), "Specifies input file.")
    ("log-level,l", value< vector<string> >(), "Specify Log Level Filter: eg. [... WARNING|SEVERE|INFO|FINE ...]")
    ("help,h", "Produce this help message.")
    ("aggregate,a", "Specifies if stats and logs should be aggregated (true) or individual (false);")
    ("dump,d", "Print out the raw data.")
    ("serialize,s", "Save the aggregated data.")
    ("print-stats,-p", "Print Statistics Summary.");

    positional_options_description p;
    p.add("input-file", -1);

    // Alternative way for multiple options in parameters;
    // Skip storing the options in a map;  
    //parsed_options parsed_options = command_line_parser(argc, argv).options(desc).run();
    parsed_options parsed_options = command_line_parser(argc,argv).options(desc).positional(p).run();
    vector <vector <string>> lists;

    variables_map vm;
    store(parsed_options, vm);
    store(command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    notify(vm);

    for (const option& o : parsed_options.options) {
        // For Multi-Value Parameters: 
        if (o.string_key == "input-file"){
            lists.push_back(o.value);
        }

        // Display help text when requested
        if (o.string_key == "help") {
            cout << "Usage: " << argv[0] << " [OPTIONS] <filename>" << endl;
            cout << desc << endl;
            return 0;
        }

        if (o.string_key == "log-level") {
            filtersRaw = o.string_key;
            vector<string> filters; 
            //boost::split(filters, filters_raw, boost::is_any_of(","), boost::algorithm::token_compress_on);
            stringstream ss(filtersRaw);
            string s;
            while (getline(ss, s, ',')) {
                filters.push_back(s);
            }
            // TODO: 
            // Add logical operators to compare to enumeration variable of log levels; 
            // Set internal flags for printing out ONLY those log entries + stack traces
            // That are part of the filtered and selected Log Levels; 
            cout << filters.size () << " Filters applied:" << endl;
            for ( long unsigned int i=0; i< filters.size(); i++) {
                cout << "\tFilter " << i+1 << ":\t" << filters[i] << endl;
            }
        }
        if (o.string_key == "dump") {
            dump = true; 
        }
        if (o.string_key == "aggregate") {
            aggregate = true; 
        }
        if (o.string_key == "serialize") {
            serialize = true; 
        }
        if (o.string_key == "print-stats") {
            stats = true; 
        }
        /*if ((lists[lists.size()].size() < 1)) {
            cout << "ERROR: No File Specified for Parsing!" << endl;
            cout << desc << endl;
        }*/
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
    //      Solution: do both; Aggregate summary <javaLogParser1> + <javaLogParser2> => stats sums both;
    //          - serialize aggregates both to a summary file; 
    javaLogParser aggregateParser(aggregate);
    for (size_t i = 0; i < lists.size(); ++i) {
        for (size_t j = 0; j < lists[i].size(); ++j) {
            filename = lists[i][j];
            //cout << "i: " << i << "\t j: " << j << "\tFilename: " << i+1 << ": " << i << ": " << filename << " Aggregation is: " << aggregate << endl;
            javaLogParser jlp = javaLogParser(filename, filtersRaw, aggregate);
            if (aggregate) {
                aggregateParser += jlp;
            }
            if (dump && !aggregate) { jlp.dump (); }
            if (serialize && !aggregate){ jlp.serialize(); }
            if (stats && !aggregate) { jlp.printStats(); }
        }
    }
    if (aggregate) {
        if (dump) { aggregateParser.dump (); }
        if (serialize){ aggregateParser.serialize(); }
        if (stats) { aggregateParser.printStats(); }
    }
}
