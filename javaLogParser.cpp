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
#include "javaStackTrace.h"
#include "javaLogEntry.h"
#include "javaLogParser.h"

// Namespacing: 
using namespace boost;
using namespace boost::program_options;
using namespace std;

bool javaLogParser::getDebug () {
    return javaLogParser::debug;
}

bool javaLogParser::setDebug (bool dbg) {
    javaLogParser::debug = dbg;
    return javaLogParser::debug;
}

bool javaLogParser::getDump() {
    return javaLogParser::dump;
}

bool javaLogParser::setDump (bool dmp) {
    javaLogParser::dump = dmp;
    return javaLogParser::dump;
}

bool javaLogParser::getAggregate () {
    return javaLogParser::aggregate;
}

bool javaLogParser::setAggregate (bool agg) {
    javaLogParser::aggregate = agg;
    return javaLogParser::aggregate;
}

bool javaLogParser::getSerialize () {
    return javaLogParser::serialize;
}

bool javaLogParser::setSerialize (bool ser) {
    javaLogParser::serialize = ser;
    return javaLogParser::serialize;
}

bool javaLogParser::getStats() {
    return javaLogParser::stats;
}

bool javaLogParser::setStats (bool ser) {
    javaLogParser::stats = ser;
    return javaLogParser::stats;
}

string javaLogParser::getFilters () {
    return javaLogParser::filters;
}

string javaLogParser::setFilters (string fltrs) {
    javaLogParser::filters = fltrs;
    return javaLogParser::filters;
}

javaLogParser::javaLogParser (string fileName) {
    this->pStart = time(nullptr);       // Metrics for Efficiency; 
    lineCount=allCount=debugCount=errorCount=fatalCount=fineCount=finerCount=finestCount=infoCount=offCount=severeCount=stackTraceCount=traceCount=unknownCount=warningCount=0;
    if(javaLogParser::getDebug ()) { cout << "DEBUG: debug: " << javaLogParser::debug << "  param debug: " << debug << endl; }
    if(javaLogParser::getDebug ()) { cout << "DEBUG: javaLogParser initialized (filename, filters, aggregate, debug)" << "( " << fileName << ", " << filters << ", " << aggregate << "," << debug << ")" << endl; }
    this->fileName = fileName; 
    this->aggregate = aggregate; 

    if (!filters.empty()){
        this->filters = filters;
    }
    
    this->initFileNames ();
    this->processFile();
}

javaLogParser::javaLogParser (const javaLogParser& source) {
    if(this->pStart > source.pStart) { this->pStart = source.pStart; }
    if(this->pEnd < source.pEnd) { this->pEnd = source.pEnd; } 
    this->fileName				=  source.fileName; 
    this->lineCount				= source.lineCount;
    this->allCount				= source.allCount;
    this->debugCount			= source.debugCount;
    this->errorCount			= source.errorCount;
    this->exceptionCount		= source.exceptionCount;
    this->fatalCount			= source.fatalCount;
    this->fineCount				= source.fineCount;
    this->finerCount			= source.finerCount;
    this->finestCount		    = source.finestCount;
    this->infoCount				= source.infoCount;
    this->offCount				= source.offCount;
    this->severeCount			= source.severeCount;
    this->stackTraceCount		= source.stackTraceCount;
    this->traceCount			= source.traceCount;
    this->unknownCount			= source.unknownCount;
    this->warningCount			= source.warningCount; 
    // Add Maps Items: 
    this->messageEntries		= source.messageEntries;
    this->allEntries			= source.allEntries;
    this->debugEntries			= source.debugEntries;
    this->errorEntries			= source.errorEntries;
    this->exceptionEntries		= source.exceptionEntries;
    this->fatalEntries			= source.fatalEntries;
    this->fineEntries			= source.fineEntries;
    this->finerEntries			= source.finerEntries;
    this->finestEntries			= source.finestEntries;
    this->infoEntries			= source.infoEntries;
    this->offEntries			= source.offEntries;
    this->severeEntries			= source.severeEntries;
    this->stackTraceEntries		= source.stackTraceEntries;
    this->stackTraceLineEntries = source.stackTraceLineEntries;
    this->traceEntries			= source.traceEntries;
    this->unknownEntries		= source.unknownEntries;
    this->warningEntries		= source.warningEntries;
    this->initFileNames ();
}

javaLogParser::javaLogParser () {
    this->pStart            = time(nullptr);       // Metrics for Efficiency; 
    this->lineCount         = 0;
    this->allCount          = 0;
    this->debugCount        = 0;
    this->errorCount        = 0;
    this->exceptionCount    = 0;
    this->fatalCount        = 0;
    this->fineCount         = 0;
    this->finerCount        = 0;
    this->finestCount       = 0;
    this->infoCount         = 0;
    this->offCount          = 0;
    this->severeCount       = 0;
    this->stackTraceCount   = 0;
    this->traceCount        = 0;
    this->unknownCount      = 0;
    this->warningCount      = 0; 
    this->initFileNames ();

}

javaLogParser::~javaLogParser () {
    this->fh.close ();
}

javaLogParser& javaLogParser::operator +=(javaLogParser const &source) {
    // Set filename to `aggregate + _{stat,ule,ulste} + .log`;
    this->lineCount         += source.lineCount;
    this->allCount          += source.allCount;
    debugCount              += source.debugCount;
    this->errorCount        += source.errorCount;
    this->exceptionCount    += source.exceptionCount;
    this->fatalCount        += source.fatalCount;
    this->fineCount         += source.fineCount;
    this->finerCount        += source.finerCount;
    this->finestCount       += source.finestCount;
    this->infoCount         += source.infoCount;
    this->offCount          += source.offCount;
    this->severeCount       += source.severeCount;
    this->stackTraceCount   += source.stackTraceCount;
    this->traceCount        += source.traceCount;
    this->unknownCount      += source.unknownCount;
    this->warningCount      += source.warningCount; 
    if(this->pStart > source.pStart) { this->pStart = source.pStart; }
    if(this->pEnd < source.pEnd) { this->pEnd = source.pEnd; } 

    // Add Maps Items: 
    for (auto x : source.stackTraceLineEntries) {
        this->stackTraceLineEntries[x.first] += x.second;  
    }
    for (auto x : source.messageEntries) {
        this->messageEntries[x.first] += x.second;  
    }

    // Add javaLogEntry vector: TODO: Need to create operator+ overload in javaLogEntry; 
    for (auto x : source.logEntries) {
        this->logEntries.push_back (x);
    }
    
    this->messageEntries.insert(source.messageEntries.begin(), source.messageEntries.end());
    this->allEntries.insert(source.allEntries.begin(), source.allEntries.end());
    this->debugEntries.insert(source.debugEntries.begin(), source.debugEntries.end());
    this->errorEntries.insert(source.errorEntries.begin(), source.errorEntries.end());
    this->exceptionEntries.insert(source.exceptionEntries.begin(), source.exceptionEntries.end());
    this->fatalEntries.insert(source.fatalEntries.begin(), source.fatalEntries.end());
    this->fineEntries.insert(source.fineEntries.begin(), source.fineEntries.end());
    this->finerEntries.insert(source.finerEntries.begin(), source.finerEntries.end());
    this->finestEntries.insert(source.finestEntries.begin(), source.finestEntries.end());
    this->infoEntries.insert(source.infoEntries.begin(), source.infoEntries.end());
    this->offEntries.insert(source.offEntries.begin(), source.offEntries.end());
    this->severeEntries.insert(source.severeEntries.begin(), source.severeEntries.end());
    this->stackTraceEntries.insert(source.stackTraceEntries.begin(), source.stackTraceEntries.end());
    this->stackTraceLineEntries.insert(source.stackTraceLineEntries.begin(), source.stackTraceLineEntries.end());
    this->traceEntries.insert(source.traceEntries.begin(), source.traceEntries.end());
    this->unknownEntries.insert(source.unknownEntries.begin(), source.unknownEntries.end());
    this->warningEntries.insert(source.warningEntries.begin(), source.warningEntries.end());

    return *this;
}

bool javaLogParser::operator ==(javaLogParser const &target) {
    // Compare pertinent values; 
    //      logLevel ; message
    //      date, time, and id can be disregarded for line comparisons; 
    bool equal = false;
    (this->lineCount == target.lineCount) ? equal = true : equal = false;
    return equal; 
}

void javaLogParser::addCounterMetrics (string logLevel) {
    // Filter date/time/id from lines for better uniq counting; 
    string ln = this->logLevel + " " + this->message; //this->logEntries[idx].getMessage ();
    switch (hashit(logLevel)) {
        case ALL:
            this->allCount++;
            this->allEntries[ln]++;
            break;
        case DEBUG:
            debugCount++;
            debugEntries[ln]++;
            break;
        case ERROR:
            this->errorCount++;
            this->errorEntries[ln]++;
            break;
        case FATAL:
            this->fatalCount++;
            this->fatalEntries[ln]++;
            break;
        case FINE:
            this->fineCount++;
            this->fineEntries[ln]++;
            break;
        case FINER:
            this->finerCount++;
            this->finerEntries[ln]++;
            break;
        case FINEST:
            this->finestCount++;
            this->finestEntries[ln]++;
            break;
        case INFO:
            this->infoCount++;
            this->infoEntries[ln]++;
            break;
        case OFF:
            this->offCount++;
            this->offEntries[ln]++;
            break;
        case SEVERE:
            this->severeCount++;
            this->severeEntries[ln]++;
            break;
        case TRACE:
            this->traceCount++;
            this->traceEntries[ln]++;
            break;
        case WARN:
            this->warningCount++;
            this->warningEntries[ln]++;
            break;
        default:
            this->unknownCount++;
            this->unknownEntries[ln]++;
            cout << "Failure to log Metrics for UNKNOWN Log Level Type: " << logLevel << ln << endl;
    }
}

void javaLogParser::addStackItem (string line) {
    istringstream ss(line);
    string traceItem; 
    ss >> this->firstWord;
    if (this->firstWord != "at" || this->firstWord != "Caused:"){
        string tmp; 
        getline(ss,tmp);
        traceItem = this->firstWord + tmp;
    } else {
        getline(ss,traceItem);
    }
    stackTraceLineEntries[traceItem]++;
}

void javaLogParser::dumpElements () {
    for ( long unsigned int i=0; i< this->logEntries.size(); i++) {
        logEntries[i].dumpElements ();
    }

    // Order the unordered_maps; 
    multimap<int, string> stackTraceLineEntriesOrdered; 
    multimap<int, string> messageEntriesOrdered; 
    multimap<int, string> severeEntriesOrdered; 
    for (auto x : stackTraceLineEntries) {
        stackTraceLineEntriesOrdered.insert(pair<int, string>(x.second, x.first));
    }
    for (auto x : messageEntries) {
        messageEntriesOrdered.insert(pair<int, string>(x.second, x.first));
    }
    for (auto x : severeEntries) {
        severeEntriesOrdered.insert(pair<int, string>(x.second, x.first));
    }

    // Create a map reverse iterator
    multimap<int, string>::reverse_iterator it;

    // Iterate them reversely: 
    
    cout << this->header(string("Unique Stack Trace Entries")); 
    for (it = stackTraceLineEntriesOrdered.rbegin(); it != stackTraceLineEntriesOrdered.rend(); it++) {
        cout << it->first << " " << it->second << endl;
    }
    cout << this->header(string("Unique Log Entry Messages"));
    for (it = messageEntriesOrdered.rbegin(); it != messageEntriesOrdered.rend(); it++) {
        cout << it->first << " " << it->second << endl;
    }
    cout << this->header(string("Unique SEVERE Log Entries"));
    for (it = severeEntriesOrdered.rbegin (); it != severeEntriesOrdered.rend (); it++) {
        cout << it->first << " " << it->second << endl;
    }
}

string javaLogParser::escaped(const string& input) {
    string output;
    output.reserve(input.size());
    for (const char c: input) {
        switch (c) {
            case '\a':  output += "\\a";        break;
            case '\b':  output += "\\b";        break;
            case '\f':  output += "\\f";        break;
            case '\n':  output += "\\n";        break;
            case '\r':  output += "\\r";        break;
            case '\t':  output += "\\t";        break;
            case '\v':  output += "\\v";        break;
            case '/':  output += "_";        break;
            default:    output += c;            break;
        }
    }

    return output;
}

vector<string> javaLogParser::generateStats () {
    vector<string> stats; 
    int elements = this->lineCount;
    stats.push_back ("\tLines:\t\t\t\t" + to_string(elements) + "\n");
    stats.push_back ("\tUniq Stack Trace Items:\t\t" + to_string(stackTraceLineEntries.size ()) + "\n");
    stats.push_back ("\t\tStack Trace Lines:\t" + to_string(stackTraceCount) + "\n"); 
    stats.push_back ("\tUniq Log Entry Items:\t\t" + to_string(messageEntries.size ()) + "\n");
    // Suppress 0 value Entries to remove noisy output; 
    if(allCount != 0){ stats.push_back ("\t\tALL Entries:\t\t" + to_string(allCount) + "\n"); }
    if(debugCount != 0){ stats.push_back ("\t\tDEBUG Entries:\t\t" + to_string(debugCount) + "\n"); }
    if(errorCount != 0){ stats.push_back ("\t\tERROR Entries:\t\t" + to_string(errorCount) + "\n"); }
    if(exceptionCount != 0){ stats.push_back ("\t\tException Entries:\t\t" + to_string(exceptionCount) + "\n"); }
    if(fatalCount != 0){ stats.push_back ("\t\tFATAL Entries:\t\t" + to_string(fatalCount) + "\n"); }
    if(fineCount != 0){ stats.push_back ("\t\tFINE Entries:\t\t" + to_string(fineCount) + "\n"); }
    if(finerCount != 0){ stats.push_back ("\t\tFINER Entries:\t\t" + to_string(finerCount) + "\n"); }
    if(finestCount != 0){ stats.push_back ("\t\tFINEST Entries:\t\t" + to_string(finestCount) + "\n"); }
    if(infoCount != 0){ stats.push_back ("\t\tINFO Entries:\t\t" + to_string(infoCount) + "\n"); }
    if(offCount != 0){ stats.push_back ("\t\tOFF Entries:\t\t" + to_string(offCount) + "\n"); }
    if(severeCount != 0){ stats.push_back ("\t\tSEVERE Entries:\t\t" + to_string(severeCount) + "\n"); }
    if(traceCount != 0){ stats.push_back ("\t\tTRACE Entries:\t\t" + to_string(traceCount) + "\n"); }
    if(unknownCount != 0){ stats.push_back ("\t\tUNKNOWN Entries:\t" + to_string(unknownCount) + "\n"); }
    if(warningCount != 0){ stats.push_back ("\t\tWARN Entries:\t\t" + to_string(warningCount) + "\n"); }
    stats.push_back ("\tStart Ts:\t\t" + to_string(pStart) + "\n");
    this->pEnd = time(nullptr);
    int duration = pEnd - pStart; 
    stats.push_back ("\tEnd Time:\t\t" + to_string(pEnd) + "\n");
    double lps; 
    (duration == 0) ? lps = elements : lps = elements / duration; 
    stats.push_back ("\tDuration:\t\t" + to_string(duration) + " s\n");
    stats.push_back ("\tProcessed:\t\t" + to_string(lps/1000) + "K lines per second" + "\n");

    return stats;
}

javaLogEntry* javaLogParser::getCurrentLogEntry() {
    size_t sz = this->logEntries.size() - 1;
    return &this->logEntries[sz];
}

vector<javaLogEntry> javaLogParser::getElements() const {
    return this->logEntries; 
}; 


javaLogParser::logLevelType javaLogParser::hashit (string const& inString) {
    if (inString == "ALL") return ALL;
    else if (inString == "DEBUG") return DEBUG; 
    else if (inString == "ERROR" || inString == "ERR") return ERROR; 
    else if (inString == "FATAL") return FATAL; 
    else if (inString == "FINE") return FINE; 
    else if (inString == "FINER") return FINER; 
    else if (inString == "FINEST") return FINEST; 
    else if (inString == "INFO") return INFO; 
    else if (inString == "OFF") return OFF; 
    else if (inString == "SEVERE") return SEVERE; 
    else if (inString == "TRACE") return TRACE; 
    else if (inString == "WARNING" || inString == "WARN") return WARN; 
    else {
        //cout << "UNKNOWN Log Level: " << inString << endl;
        return UNKNOWN; 
    }
}

string javaLogParser::header (string title, int style) {
    switch (style) {
        case 1:
            return string("\n\n======================================== " + title + " ========================================\n\n");
        case 2:
        default:
            return string("\n\n-*`*-._.-*`*-._.-*`*-._.-*`*-._.-*`*-._.-> " + title + " <-*`*-._.-*`*-._.-*`*-._.-*`*-._.-*`*-._.-\n\n");
    }
}

void javaLogParser::initFileNames () {
    // Logging output filename propagation:
    // fileName has already been parsed for unsupported chracters, TODO: perhaps moving the filename regexing to here may be better suited in future; 

    if(javaLogParser::getDebug ()) { cout << "DEBUG: Calling initFileNames ()" << endl; } 

    bool dbgPrev = this->getDebug ();
    if(this->getDebug()) { this->setDebug(false); }
    if(this->getDebug ()) { cout<< "DEBUG: Debug flag is: " << javaLogParser::debug << endl; }
    string prefix; 
    if (this->aggregate) {
        prefix = "aggregate"; 
    } else {
        prefix = this->escaped(this->fileName);
    }
    if(javaLogParser::getDebug ()) { cout << "DEBUG: aggregate tested: " << prefix << "\t agg: " << this->aggregate << endl; }
    this->base_dir      = "logs/";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: base_dir" << endl; }
    this->ule_log       = prefix + "_ule.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: ule_log" << endl; }
    this->uste_log      = prefix + "_uste.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: uste_log" << endl; }
    this->stats_log     = prefix + "_stats.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: stats_log" << endl; }
    this->all_log       = prefix + "_all.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: all_log" << endl; }
    debug_log     = prefix + "_debug.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: debug_log" << endl; }
    this->error_log     = prefix + "_error.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: debug_log" << endl; }
    this->exception_log = prefix + "_exception.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: err_log" << endl; }
    this->fatal_log     = prefix + "_fatal.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: fatal_log" << endl; }
    this->fine_log      = prefix + "_fine.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: fine_log" << endl; }
    this->finer_log     = prefix + "_finer.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: finer_log" << endl; }
    this->finest_log    = prefix + "_finest.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: finest_log" << endl; }
    this->info_log      = prefix + "_info.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: info_log" << endl; } 
    this->off_log       = prefix + "_off.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: off_log" << endl; } 
    this->severe_log    = prefix + "_severe.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: severe_log" << endl; }
    this->trace_log     = prefix + "_trace.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: trace_log" << endl; }
    this->warning_log   = prefix + "_warning.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: warning_log" << endl; }
    this->unknown_log   = prefix + "_unknown.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: unknown_log" << endl; }
    this->fst_log       = prefix + "_fst.log";
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Initialized Log File Names: fst_log" << endl; }

    if(dbgPrev) { this->setDebug (dbgPrev); }
}

bool javaLogParser::isStackTrace () {
    return !regex_match (this->firstWord, javaLogParser::reDate);
}

bool javaLogParser::hasException () {
    return regex_search (this->line, javaLogParser::reException); 
}

multimap<int, string> javaLogParser::orderMap (unordered_map<string, int>& sourceMap) {
    multimap<int, string> orderedMap;
    for (auto x : sourceMap) {
        orderedMap.insert(pair<int, string>(x.second, x.first));
    }

    return orderedMap; 
}

void javaLogParser::printStats () {
    // TODO: reference vector<string> generateStats (); iterate it; 
    vector<string> stats = generateStats ();
    vector<string>::iterator sit;
    for (sit = stats.begin(); sit != stats.end(); ++sit) {
        cout << *sit;
    }
}

void javaLogParser::processFile() {
    this->fh = ifstream(this->fileName); // streams don't use exceptions by default; 
    if (!this->fh) {
        string header = string("Error in File: " + this->fileName); 
        cout << "DEBUG: " << this->header(header) << endl; 
    }

    while (getline(this->fh, this->line)) {
        this->ss = istringstream(this->line);
        //cout << "firstWord:\t" << firstWord << endl; 
        this->ss >> this->firstWord; 
        size_t sz = 0;
        
        if (!this->isStackTrace()) {
            this->date = this->firstWord;
            javaLogEntry logEntry = this->processLine();
            this->logEntries.push_back(logEntry); 
            this->ss >> this->timestamp >> this->id >> this->logLevel; 
            getline(this->ss, this->message);  // Get remaining line entries as one object; 
            this->messageEntries[this->message]++;
            addCounterMetrics (this->logLevel);
            // Add Exception Entries 
            if (this->hasException()) {
               this->exceptionCount++;
               this->exceptionEntries[this->line]++; 
            }

            // Check if previous javaLogEntry contained a stack trace, if true ? push : continue;
            sz = this->logEntries.size () - 1; // index to last element;
            //if (javaLogParser::getDebug ()) { cout << "DEBUG: Stack Trace before unordered map: cst: " << this->logEntries[sz - 1].containsStackTrace << endl; }

            if (sz > 0) {
                if (this->logEntries[sz - 1].containsStackTrace == 1) {
                    if (!logEntries[sz - 1].getLines ()->empty ()) {
                        this->stackTraceEntries[*logEntries[sz - 1].jstPtr]++; 
                    }
                }
            }
        }
        else {
            //  Recursively add/pop line to previous javaLogEntry into stack trace vector; 
            if (this->hasException()) {
                this->exceptionCount++;
                this->exceptionEntries[this->line]++; 
            }
            this->processLine();
        }
    }
    if(javaLogParser::getDebug ()) { cout << "DEBUG: Processed File" << endl; }
}

javaLogEntry javaLogParser::processLine() {
    if(!this->isStackTrace ()) {
        //cout << "Initial Log Entry - Not a Stack Trace:" << line << endl;
        javaLogEntry logEntry (this->line);
        this->lineCount++;
        return logEntry;
    } 
    else {
        this->lineCount++;
        this->stackTraceCount++;
        this->addStackItem(line); // add stack trace line to unique stack trace line items unordered_map; 

        int sz = this->logEntries.size ();
        if(sz == 0) {
            javaLogEntry logEntry (line);
            logEntry.containsStackTrace = true;
            return logEntry;
        } else {
            javaLogEntry* jle = &this->logEntries[sz -1];
            jle->containsStackTrace = true; 
            //javaLogEntry logEntry (this->logEntries[sz - 1]);  // -1 is important, causes core dump cuase itr is past last entry of vector; 
            jle->push_back(line);

            return *jle;
        }
    }
}

void javaLogParser::serializeData() {
    // Dump the metrics and data to file(s); 
    //  <log_name>_ule.log      -> Uniq Log Entries (Ordered most occurring to least)
    //  <log_name> uste.log     -> Uniq Stack Trace Entries (Ordered most occurring to least)
    //  <log_name>_stats.log    -> stats () / metrics (); 
    
    if (javaLogParser::getDebug()) { cout << "Serializing Data" << endl; }
    // Create reverse multimap iterator
    multimap<int, string>::reverse_iterator it;
    // Create string iterator; 
    vector<string>::iterator sit;

    // These will always output on serialize; 

    /**********************
     * uleLogFile: Uniq Log Entries
    */
    ofstream uleLogFile(this->base_dir + this->ule_log); 
    multimap<int, string> messageEntriesOrdered;
    for (auto x : messageEntries) {
        messageEntriesOrdered.insert (pair<int, string>(x.second, x.first));
    }
    uleLogFile << this->header(" Unique Log Entry Messages ");
    for (it = messageEntriesOrdered.rbegin(); it != messageEntriesOrdered.rend(); it++) {
        uleLogFile << it->first << " " << it->second << endl;
    }

    /**********************
     * usteLogFile: Uniq Stack Trace Entries
    */
    ofstream usteLogFile(this->base_dir + this->uste_log); 
    if (javaLogParser::getDebug ()) { cout << this->header ("DEBUG: Uniq Stack Trace Log: " + this->base_dir + this->uste_log) << "\t Stack Traces: " << this->stackTraceEntries.size() << endl; }
    multimap<int, javaStackTrace> stackTraceEntriesOrdered;
    for (auto x : this->stackTraceEntries) {
        stackTraceEntriesOrdered.insert(pair<int, javaStackTrace>(x.second, x.first));
    }
    usteLogFile << this->header(" Unique Stack Trace Entries ");
    if (javaLogParser::getDebug ()) { cout << "DEBUG: stackTraceEntries: " << this->stackTraceEntries.size() << "\nDEBUG: stackTraceEntriesOrdered: " << stackTraceEntriesOrdered.size() << endl;}
    //for (auto x : stackTraceEntriesOrdered) {
    multimap<int, javaStackTrace>::reverse_iterator rit;
    for (rit = stackTraceEntriesOrdered.rbegin(); rit != stackTraceEntriesOrdered.rend(); rit++) {
        usteLogFile << rit->first << " " << rit->second << endl;
        if (javaLogParser::getDebug ()) { cout << "DEBUG: stackTraceEntriesOrdered: " << rit->first << rit->second << endl;}
    }

    // Generate stats and iterate the stats vector; 
    /**********************
     * statsLogFile: Log File Parsing Statistics
    */
    ofstream statsLogFile(this->base_dir + this->stats_log);
    statsLogFile << this->header(" Java Log Parsing Statsistics ");
    vector<string> stats = this->generateStats ();
    for (sit = stats.begin(); sit != stats.end(); ++sit) {
        statsLogFile << *sit;
    }

    uleLogFile.close ();
    usteLogFile.close ();
    statsLogFile.close ();  

    // These logs depend on if there's entries found, no sense cluttering up the logs folder with empty files; 
    //
    if (this->allCount > 0) {
        ofstream allLogFile(this->base_dir + this->all_log);
        multimap<int, string> allEntriesOrdered;
        for (auto x : allEntries) {
            allEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        allLogFile << this->header(" Unique ALL Log Level Entries ");
        for (it = allEntriesOrdered.rbegin(); it != allEntriesOrdered.rend(); it++) {
            allLogFile << it->first << " " << it->second << endl;
        }
        allLogFile.close ();
    }
    if (debugCount > 0) {
        ofstream debugLogFile(this->base_dir + debug_log); 
        multimap<int, string> debugEntriesOrdered;
        for (auto x : debugEntries) {
            debugEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        debugLogFile << this->header(" Unique DEBUG Log Level Entries ");
        for (it = debugEntriesOrdered.rbegin(); it != debugEntriesOrdered.rend(); it++) {
            debugLogFile << it->first << " " << it->second << endl;
        }
        debugLogFile.close ();
    }
    if (this->errorCount > 0) {
        ofstream errorLogFile(this->base_dir + this->error_log); 
        multimap<int, string> errorEntriesOrdered;
        for (auto x : errorEntries) {
            errorEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        errorLogFile << this->header(" Unique ERROR Log Level Entries ");
        for (it = errorEntriesOrdered.rbegin(); it != errorEntriesOrdered.rend(); it++) {
            errorLogFile << it->first << " " << it->second << endl;
        }
        errorLogFile.close ();
    }
    if (this->exceptionCount > 0) {
        ofstream exceptionLogFile(this->base_dir + this->exception_log); 
        multimap<int, string> exceptionEntriesOrdered;
        for (auto x : exceptionEntries) {
            exceptionEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        exceptionLogFile << this->header(" Unique exception Log Level Entries ");
        for (it = exceptionEntriesOrdered.rbegin(); it != exceptionEntriesOrdered.rend(); it++) {
            exceptionLogFile << it->first << " " << it->second << endl;
        }
        exceptionLogFile.close ();
    }
    if (this->fatalCount > 0) {
        ofstream fatalLogFile(this->base_dir + this->fatal_log); 
        multimap<int, string> fatalEntriesOrdered;
        for (auto x : fatalEntries) {
            fatalEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        fatalLogFile << this->header(" Unique Log Level Entries ");
        for (it = fatalEntriesOrdered.rbegin(); it != fatalEntriesOrdered.rend(); it++) {
            fatalLogFile << it->first << " " << it->second << endl;
        }
        fatalLogFile.close ();
    }
    if (this->fineCount > 0) {
        ofstream fineLogFile(this->base_dir + this->fine_log); 
        multimap<int, string> fineEntriesOrdered;
        for (auto x : fineEntries) {
            fineEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        fineLogFile << this->header(" Unique FINE Log Level Entries ");
        for (it = fineEntriesOrdered.rbegin(); it != fineEntriesOrdered.rend(); it++) {
            fineLogFile << it->first << " " << it->second << endl;
        }
        fineLogFile.close ();
    }
    if (this->finerCount > 0) {
        ofstream finerLogFile(this->base_dir + this->finer_log); 
        multimap<int, string> finerEntriesOrdered;
        for (auto x : finerEntries) {
            finerEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        finerLogFile << this->header(" Unique FINER Log Level Entries ");
        for (it = finerEntriesOrdered.rbegin(); it != finerEntriesOrdered.rend(); it++) {
            finerLogFile << it->first << " " << it->second << endl;
        }
        finerLogFile.close ();
    }
    if (this->finestCount > 0) {
        ofstream finestLogFile(this->base_dir + this->finest_log); 
        multimap<int, string> finestEntriesOrdered;
        for (auto x : finestEntries) {
            finestEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
            finestLogFile << this->header(" Unique FINEST Log Level Entries ");
        for (it = finestEntriesOrdered.rbegin(); it != finestEntriesOrdered.rend(); it++) {
            finestLogFile << it->first << " " << it->second << endl;
        } 
        finestLogFile.close ();       
    }
    if (this->infoCount > 0) {
        ofstream infoLogFile(this->base_dir + this->info_log); 
        multimap<int, string> infoEntriesOrdered;
        for (auto x : infoEntries) {
            infoEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        infoLogFile << this->header(" Unique INFO Log Level Entries ");
        for (it = infoEntriesOrdered.rbegin(); it != infoEntriesOrdered.rend(); it++) {
            infoLogFile << it->first << " " << it->second << endl;
        }
        infoLogFile.close ();
    }
    if (this->offCount > 0) {
        ofstream offLogFile(this->base_dir + this->off_log); 
        multimap<int, string> offEntriesOrdered;
        for (auto x : offEntries) {
            offEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        offLogFile << this->header(" Unique OFF Log Level Entries ");
        for (it = offEntriesOrdered.rbegin(); it != offEntriesOrdered.rend(); it++) {
            offLogFile << it->first << " " << it->second << endl;
        }
        offLogFile.close ();
    }
    if (this->severeCount > 0) {
        ofstream severeLogFile(this->base_dir + this->severe_log); 
        multimap<int, string> severeEntriesOrdered;
        for (auto x : severeEntries) {
            severeEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        severeLogFile << this->header(" Unique SEVERE Log Level Entries ");
        for (it = severeEntriesOrdered.rbegin(); it != severeEntriesOrdered.rend(); it++) {
            severeLogFile << it->first << " " << it->second << endl;
        }
        severeLogFile.close ();
    }
    if (this->traceCount > 0) {
        ofstream traceLogFile(this->base_dir + this->trace_log); 
        multimap<int, string> traceEntriesOrdered;
        for (auto x : traceEntries) {
            traceEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        traceLogFile << this->header(" Unique TRACE Log Level Entries ");
        for (it = traceEntriesOrdered.rbegin(); it != traceEntriesOrdered.rend(); it++) {
            traceLogFile << it->first << " " << it->second << endl;
        }
        traceLogFile.close ();
    }
    if (this->warningCount > 0) {
        ofstream warningLogFile(this->base_dir + this->warning_log); 
        multimap<int, string> warningEntriesOrdered;
        for (auto x : warningEntries) {
            warningEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        warningLogFile << this->header(" Unique WARNING Log Level Entries ");
        for (it = warningEntriesOrdered.rbegin(); it != warningEntriesOrdered.rend(); it++) {
            warningLogFile << it->first << " " << it->second << endl;
        }
        warningLogFile.close ();
    }
    if (this->unknownCount > 0) {
        ofstream unknownLogFile(this->base_dir + this->unknown_log); 
        multimap<int, string> unknownEntriesOrdered;
        for (auto x : unknownEntries) {
            unknownEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        unknownLogFile << this->header(" Unique UNKNOWN Log Level Entries ");
        for (it = unknownEntriesOrdered.rbegin(); it != unknownEntriesOrdered.rend(); it++) {
            unknownLogFile << it->first << " " << it->second << endl;
        }
        unknownLogFile.close ();
    } 
}
