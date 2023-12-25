// STL Inclusions: 
#include <fstream>
#include <iterator>
#include <sstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <regex>
#include <ctime>
#include <chrono>
#include <exception>
#include <unordered_map> 
#include <thread> 
#include <set> 
#include <algorithm>

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

// Custom Inclusions (Are these needed to be included here or is in their respective class.h file appropriate? lets find out); 
#include "config.h"
#include "javaStackTrace.h"
#include "javaLogEntry.h"
#include "javaLogParser.h"        // Class Defs;


// Namespacing: 
using namespace boost;
using namespace boost::program_options;
using namespace std;

// Flags passed to be updated later to be seen by all objects in the program:
bool javaLogParser::debug;
bool javaLogParser::aggregate;
bool javaLogParser::serialize;
bool javaLogParser::dump; 
bool javaLogParser::stats;
string javaLogParser::filters;
regex javaLogParser::re = regex("^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]");

void threadStartup(vector<javaLogParser>* jlp, string fn) { 
    jlp->push_back(javaLogParser(fn));
    if(javaLogParser::getDebug()){ cout << "Started Thread for Filename: " << fn << endl; }
}


int main(int argc, char * argv[])
{
    time_t tsStart = time(nullptr); 
    char const *log_folder = "./logs/";
    mkdir(log_folder, 0755); 

    string filename;
    string delimiter = ",";
    string filtersRaw = "";


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
    ("print-stats,-p", "Print Statistics Summary.")
    ("debug,-b", "Print DEBUG log info")
    ("version,-v", "Print Version info");

    positional_options_description p;
    p.add("input-file", -1);

    // Alternative way for multiple options in parameters;
    // Skip storing the options in a map;  
    //  parsed_options parsed_options = command_line_parser(argc, argv).options(desc).run();
    parsed_options parsed_options = command_line_parser(argc,argv).options(desc).positional(p).run();
    vector <vector <string>> fileNames;

    variables_map vm;
    store(parsed_options, vm);
    store(command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    notify(vm);

    for (const option& o : parsed_options.options) {
        // Display help text when requested
        if (o.string_key == "help") {
            cout << "Usage: " << argv[0] << " [OPTIONS] <filename>" << endl;
            cout << desc << endl;
            return 0;
        }

        // For Multi-Value Parameters: 
        if (o.string_key == "input-file"){
            fileNames.push_back(o.value);
        }

        if (o.string_key == "log-level") {
            filtersRaw = o.string_key;
            javaLogParser::setFilters (o.string_key);
            vector<string> filterList; 
            //boost::split(filters, filters_raw, boost::is_any_of(","), boost::algorithm::token_compress_on);
            stringstream ss(filtersRaw);
            string s;
            while (getline(ss, s, ',')) {
                filterList.push_back(s);
            }
            // TODO: 
            // Add logical operators to compare to enumeration variable of log levels; 
            // Set internal flags for printing out ONLY those log entries + stack traces
            // That are part of the filtered and selected Log Levels; 
            cout << filterList.size () << " Filters applied:" << endl;
            for ( long unsigned int i=0; i< filterList.size(); i++) {
                cout << "\tFilter " << i+1 << ":\t" << filterList[i] << endl;
            }
        }
        if (o.string_key == "dump") {
            javaLogParser::setDump(true); 
        }
        if (o.string_key == "aggregate") {
            javaLogParser::setAggregate(true); 
        }
        if (o.string_key == "serialize") {
            javaLogParser::setSerialize(true); 
        }
        if (o.string_key == "print-stats") {
            javaLogParser::setStats(true); 
        }
        if (o.string_key == "debug") {
            javaLogParser::setDebug (true); 
        }
        if (o.string_key == "version") {
            cout << PROJECT_NAME << " " << PROJECT_VER << endl;
            return 0;
        }
    }
    
    if (argc <= 1 || fileNames.size() < 1) {
        cout << "Usage: " << argv[0] << " [OPTIONS] <filename>" << endl;
        cout << desc << endl;

        return 0; 
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
    vector<javaLogParser> vlogParsers;
    vector<thread> jlpThreads;
    vector<javaLogParser>* parsersPtr = &vlogParsers;

    if (javaLogParser::getDebug ()) { cout << "DEBUG: init log parsers vector" << endl; }

    // Iterate through Filenames; 
    for (auto x : fileNames) {
        for (auto y: x) {
            filename = y;

            if (javaLogParser::getDebug ()) { cout << "DEBUG:\n\tx: " << x.size () << "\n\tj: " << y.size() << "\n\tfilename: " << filename << "\n\tAggregation is: " << javaLogParser::getAggregate () << "\n\tDebug is " << javaLogParser::getDebug () << endl; }
            //vlogParsers.push_back (javaLogParser(filename));
            jlpThreads.push_back(thread(threadStartup, /*&vlogParsers*/parsersPtr, filename));
            if (javaLogParser::getDebug ()) { cout << "DEBUG: jlp created; pushing on stack;" << endl; }
            //vlogParsers.push_back (jlp);
            if (javaLogParser::getDebug ()) { cout << "DEBUG: jlp pushed" << endl; }
        }
    }

    for (thread &t : jlpThreads) {
        if (t.joinable()){
            t.join();
        }
    }

    if(javaLogParser::getDebug ()) { cout << "DEBUG: vlogParsers populated: " << vlogParsers.size() << endl; }
    // Override Aggregates Setting if more than one file presented to -i or positionally on cli; 
    if(fileNames.size() > 1) { 
        javaLogParser::setAggregate(true); 
        if (javaLogParser::getDebug ()) { cout << "DEBUG: Overrode Aggregate Option: new value TRUE" << endl; }
        
    } else {
        javaLogParser::setAggregate(false); // No reason to have aggregation option set when there is only 1 file; 
    }
    //javaLogParser aggregateParser(/*vlogParsers[0]*/); // segfaults when no parameters passed, means failing to print help/usage and exit when no parameters passed !; 
    javaLogParser aggregateParser;
    // segfaults when no parameters passed, means failing to print help/usage and exit when no parameters passed !; 
    if (javaLogParser::getDebug ()) { cout << "DEBUG: Entering loop: \n\tvlogParsers.size(): \t" << vlogParsers.size() << endl; }
    int y=0;
    int sz = vlogParsers.size ();
    for (auto x : vlogParsers) {
        if(javaLogParser::getDebug ()) { cout << "DEBUG: vlogParsers loop iteration: " << y << endl; }
        if (javaLogParser::getAggregate ()) {    
            if (javaLogParser::getDebug ()) { cout << "DEBUG: Aggregate selected, adding objects: vlog: " << vlogParsers.size () << endl; }
            if (sz == 1) { 
                javaLogParser aggregateParser(x); 
            } else { 
                aggregateParser += x; 
            }
        }
        else {
            if (javaLogParser::getDebug ()) { 
                cout << "DEBUG: Aggregate not selected, iterating through options to print, serialize, dump, etc" << endl; 
                cout << "\t\tFilters: " << javaLogParser::getFilters () << endl;
                cout << "\t\tDebug: " << javaLogParser::getDebug () << endl;
                cout << "\t\tDump:  " << javaLogParser::getDump () << endl;
                cout << "\t\tSerialize: " << javaLogParser::getSerialize () << endl;
                cout << "\t\tStats: " << javaLogParser::getStats () << endl;
            }

            if (javaLogParser::getDump ()) { x.dumpElements (); } 
            if (javaLogParser::getSerialize ()) { x.serializeData (); }
            if (javaLogParser::getStats ()) { x.printStats (); }
        }
        y++;
    }

    if (javaLogParser::getAggregate ()) {
            if (javaLogParser::getDebug ()) { cout << "Dumping Aggregated javaLogParser Data" << endl; }
            if (javaLogParser::getDump ()) { aggregateParser.dumpElements (); }
            if (javaLogParser::getSerialize ()) { aggregateParser.serializeData (); }
            if (javaLogParser::getStats ()) { aggregateParser.printStats (); }
    }
    time_t tsEnd = time(nullptr);
    cout << "bmTime: " << tsEnd - tsStart << "s" << endl;
}