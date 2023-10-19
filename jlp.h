// jlp.h
//
// This file declares the class for javaLogParser
// This class is responsible for:
//  1. Reading in a Java Log File
//  2. Parsing each Log Entry
//  3. Aggregating the logging events
//      1. Type of Log Entry: {Single-Line | Multi-Line}
//      2. Log Level of the Entry
//      3. Attached Stack Trace if included; 
//  4. As a base for a smarter and more intuitive insight into a JVM's logged entries; 

#include "jle.h"
using namespace std; 

class javaLogParser {
private:
    // Global Level Variables not specific to any particular method: 
    string fileName;

    enum logType { ALL = 1 << 1, CONFIG = 1 << 2, DEBUG = 1 << 3, ERROR = 1 << 4, FATAL = 1 << 5, FINE = 1 << 6, FINER = 1 << 7, FINEST = 1 << 8, INFO = 1 << 9, OFF = 1 << 10, SEVERE = 1 << 11, TRACE = 1 << 12, UNKNOWN = 1 << 13, WARN = 1 << 14 }; 

    // Metrics: 
    // Count each type of logType encountered; 
    int lineCount		= 0;
	int allCount		= 0;
	int debugCount		= 0;
	int errorCount		= 0;
	int fatalCount		= 0;
	int fineCount		= 0;
	int finerCount		= 0;
	int finestCount		= 0;
	int infoCount		= 0;
	int offCount		= 0;
	int severeCount		= 0;
	int stackTraceCount	= 0;
	int traceCount		= 0;
	int unknownCount	= 0;
	int warningCount	= 0; 

    // Benchmark Metrics to measure timings: 
    time_t pStart = time(nullptr);
    time_t pEnd; 

    unordered_map<string, int> messageEntries, allEntries, debugEntries, errorEntries, fatalEntries, fineEntries, finerEntries, finestEntries, infoEntries, offEntries, severeEntries, stackTraceEntries, traceEntries, unknownEntries, warningEntries; 

    // Line Item Specific Variables;
    string line;
    ifstream fh;
    istringstream ss;

    vector<javaLogEntry> logEntries;
    vector<string>::const_iterator iter; // Unused thusfar, may remove as a global iterator seems pointless ? "dweeb" : "that's one hypothesis"

    // Log File Fields
    string date; 
    string timestamp; 
    string id; 
    string logLevel;
    string message; 

    // Logging Output Vars
    string base_dir = "logs/";
    string ule_log = "";
    string uste_log = "";
    string stats_log = "";
    string all_log = "";
    string debug_log = "";
    string error_log = "";
    string fatal_log = "";
    string fine_log = "";
    string finer_log = "";
    string finest_log = "";
    string info_log = "";
    string off_log = "";
    string severe_log = "";
    string trace_log = "";
    string warning_log = "";
    string unknown_log = "";
    string fst_log = "";

//Global Options paramount to this class:
    static bool debug; 
    static bool aggregate;
    static bool serialize;
    static bool dump;
    static bool stats;
    static string filters;

public:
    static bool getDebug () {
        return javaLogParser::debug;
    }

    static bool setDebug (bool dbg) {
        javaLogParser::debug = dbg;
        return javaLogParser::debug;
    }

    static bool getDump() {
        return javaLogParser::dump;
    }

    static bool setDump (bool dmp) {
        javaLogParser::dump = dmp;
        return javaLogParser::dump;
    }

    static bool getAggregate () {
        return javaLogParser::aggregate;
    }

    static bool setAggregate (bool agg) {
        javaLogParser::aggregate = agg;
        return javaLogParser::aggregate;
    }

    static bool getSerialize () {
        return javaLogParser::serialize;
    }

    static bool setSerialize (bool ser) {
        javaLogParser::serialize = ser;
        return javaLogParser::serialize;
    }

    static bool getStats() {
        return javaLogParser::stats;
    }

    static bool setStats (bool ser) {
        javaLogParser::stats = ser;
        return javaLogParser::stats;
    }

    static string getFilters () {
        return javaLogParser::filters;
    }

    static string setFilters (string fltrs) {
        javaLogParser::filters = fltrs;
        return javaLogParser::filters;
    }

    javaLogParser (string fileName) {
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

    javaLogParser (const javaLogParser& source) {
        if(this->pStart > source.pStart) { this->pStart = source.pStart; }
        if(this->pEnd < source.pEnd) { this->pEnd = source.pEnd; } 
        this->fileName =  source.fileName; 
        this->lineCount = source.lineCount;
        this->allCount = source.allCount;
        this->debugCount = source.debugCount;
        this->errorCount = source.errorCount;
        this->fatalCount = source.fatalCount;
        this->fineCount = source.fineCount;
        this->finerCount = source.finerCount;
        this->finestCount = source.finestCount;
        this->infoCount = source.infoCount;
        this->offCount = source.offCount;
        this->severeCount = source.severeCount;
        this->stackTraceCount = source.stackTraceCount;
        this->traceCount = source.traceCount;
        this->unknownCount = source.unknownCount;
        this->warningCount = source.warningCount; 
        // Add Maps Items: 
        this->messageEntries = source.messageEntries;
        this->allEntries = source.allEntries;
        this->debugEntries = source.debugEntries;
        this->errorEntries = source.errorEntries;
        this->fatalEntries = source.fatalEntries;
        this->fineEntries = source.fineEntries;
        this->finerEntries = source.finerEntries;
        this->finestEntries = source.finestEntries;
        this->infoEntries = source.infoEntries;
        this->offEntries = source.offEntries;
        this->severeEntries = source.severeEntries;
        this->stackTraceEntries = source.stackTraceEntries;
        this->traceEntries = source.traceEntries;
        this->unknownEntries = source.unknownEntries;
        this->warningEntries = source.warningEntries;
        this->initFileNames ();

    }

    javaLogParser () {
        this->pStart = time(nullptr);       // Metrics for Efficiency; 
        this->lineCount = 0;
        this->allCount = 0;
        this->debugCount = 0;
        this->errorCount = 0;
        this->fatalCount = 0;
        this->fineCount = 0;
        this->finerCount = 0;
        this->finestCount = 0;
        this->infoCount = 0;
        this->offCount = 0;
        this->severeCount = 0;
        this->stackTraceCount = 0;
        this->traceCount = 0;
        this->unknownCount = 0;
        this->warningCount = 0; 
        this->initFileNames ();

    }
 
    ~javaLogParser () {
        this->fh.close ();
    }

    javaLogParser& operator +=(javaLogParser const &source) {
        // Set filename to `aggregate + _{stat,ule,ulste} + .log`;
        this->lineCount += source.lineCount;
        this->allCount += source.allCount;
        debugCount += source.debugCount;
        this->errorCount += source.errorCount;
        this->fatalCount += source.fatalCount;
        this->fineCount += source.fineCount;
        this->finerCount += source.finerCount;
        this->finestCount += source.finestCount;
        this->infoCount += source.infoCount;
        this->offCount += source.offCount;
        this->severeCount += source.severeCount;
        this->stackTraceCount += source.stackTraceCount;
        this->traceCount += source.traceCount;
        this->unknownCount += source.unknownCount;
        this->warningCount += source.warningCount; 
        if(this->pStart > source.pStart) { this->pStart = source.pStart; }
        if(this->pEnd < source.pEnd) { this->pEnd = source.pEnd; } 

        // Add Maps Items: 
        for (auto x : source.stackTraceEntries) {
            this->stackTraceEntries[x.first] += x.second;  
        }
        for (auto x : source.messageEntries) {
            this->messageEntries[x.first] += x.second;  
        }

        // Add javaLogEntry vector: TODO: Need to create operator+ overload in javaLogEntry; 
        for (auto x : source.logEntries) {
            this->logEntries.push_back (x);
        }
        this->messageEntries = source.messageEntries;
        this->allEntries = source.allEntries;
        this->debugEntries = source.debugEntries;
        this->errorEntries = source.errorEntries;
        this->fatalEntries = source.fatalEntries;
        this->fineEntries = source.fineEntries;
        this->finerEntries = source.finerEntries;
        this->finestEntries = source.finestEntries;
        this->infoEntries = source.infoEntries;
        this->offEntries = source.offEntries;
        this->severeEntries = source.severeEntries;
        this->stackTraceEntries = source.stackTraceEntries;
        this->traceEntries = source.traceEntries;
        this->unknownEntries = source.unknownEntries;
        this->warningEntries = source.warningEntries;

        return *this;
    }

    bool operator ==(javaLogParser const &target) {
        // Compare pertinent values; 
        //      logLevel ; message
        //      date, time, and id can be disregarded for line comparisons; 
        return true; 
    }

    void addCounterMetrics (string logLevel) {
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

    void addStackTraceMetrics (string line) {


    }

    void addStackItem (string line) {
        istringstream ss(line);
        string firstWord, traceItem; 
        ss >> firstWord;
        if (firstWord != "at"){
            string tmp; 
            getline(ss,tmp);
            traceItem = firstWord + tmp;
        } else {
            getline(ss,traceItem);
        }
        stackTraceEntries[traceItem]++;
    }

    void dumpElements () {
        for ( long unsigned int i=0; i< this->logEntries.size(); i++) {
          logEntries[i].dumpElements ();
        }

        // Order the unordered_maps; 
        multimap<int, string> stackTraceEntriesOrdered; 
        multimap<int, string> messageEntriesOrdered; 
        multimap<int, string> severeEntriesOrdered; 
        for (auto x : stackTraceEntries) {
            stackTraceEntriesOrdered.insert(pair<int, string>(x.second, x.first));
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
        for (it = stackTraceEntriesOrdered.rbegin(); it != stackTraceEntriesOrdered.rend(); it++) {
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


    vector<string> generateStats () {
        vector<string> stats; 
        int elements = this->lineCount;
        stats.push_back ("\tLines:\t\t\t\t" + to_string(elements) + "\n");
        stats.push_back ("\tStack Trace Lines:\t\t" + to_string(stackTraceCount) + "\n"); 
        stats.push_back ("\tUniq Stack Trace Items:\t\t" + to_string(stackTraceEntries.size ()) + "\n");
        stats.push_back ("\tUniq Log Entry Items:\t\t" + to_string(messageEntries.size ()) + "\n");
        // Suppress 0 value Entries to remove noisy output; 
        if(allCount != 0){ stats.push_back ("\t\tALL Entries:\t\t" + to_string(allCount) + "\n"); }
        if(debugCount != 0){ stats.push_back ("\t\tDEBUG Entries:\t\t" + to_string(debugCount) + "\n"); }
        if(errorCount != 0){ stats.push_back ("\t\tERROR Entries:\t\t" + to_string(errorCount) + "\n"); }
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

    vector<javaLogEntry> getElements() const {
        return this->logEntries; 
    }; 


    logType hashit (string const& inString) {
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
    
    string header (string title, int style = 2) {
        switch (style) {
            case 1:
                return string("\n\n======================================== " + title + " ========================================\n\n");
            case 2:
            default:
                return string("\n\n-*`*-._.-*`*-._.-*`*-._.-*`*-._.-*`*-._.-> " + title + " <-*`*-._.-*`*-._.-*`*-._.-*`*-._.-*`*-._.-\n\n");
        }
    }

    void initFileNames () {
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
            prefix = this->fileName;
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

    bool isStackTrace (string firstWord) {
        regex re("^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]");
        return !regex_match (firstWord, re);
    }

    multimap<int, string> orderMap (unordered_map<string, int>& sourceMap) {
        multimap<int, string> orderedMap;
        for (auto x : sourceMap) {
            orderedMap.insert(pair<int, string>(x.second, x.first));
        }

        return orderedMap; 
    }

    void printStats () {
        // TODO: reference vector<string> generateStats (); iterate it; 
        vector<string> stats = generateStats ();
        vector<string>::iterator sit;
        for (sit = stats.begin(); sit != stats.end(); ++sit) {
            cout << *sit;
        }
    }

    void processFile() {
        try {
            this->fh = ifstream(this->fileName);
        } catch (const exception &e) {
            cerr << e.what () << endl;
            string header = string("Error in File: " + this->fileName); 
            cerr << this->header(header) << endl; 
        }

        while (getline(this->fh, this->line)){
            string firstWord;
            this->ss = istringstream(this->line);
            //cout << "firstWord:\t" << firstWord << endl; 
            this->ss >> firstWord; 
            
            if (!this->isStackTrace(firstWord)) {
                this->date = firstWord;
                regex re("^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]");
                javaLogEntry logEntry = this->processLine(line);
                this->logEntries.push_back(logEntry); 
                this->ss >> this->timestamp >> this->id >> this->logLevel; 
                getline(this->ss, this->message);  // Get remaining line entries as one object; 
                this->messageEntries[this->message]++;
                addCounterMetrics (this->logLevel);
            }
            else {
                //  Recursively add/pop line to previous javaLogEntry into stack trace vector; 
                this->processLine(line, true);
                addStackTraceMetrics(line);
            }
        }
        if(javaLogParser::getDebug ()) { cout << "DEBUG: Processed File" << endl; }
    }

    javaLogEntry processLine(string line, bool stackTrace = false) {
        if(!stackTrace) {
            //cout << "Initial Log Entry - Not a Stack Trace:" << line << endl;
            javaLogEntry logEntry (line);
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
                return logEntry;
            } else {
                javaLogEntry logEntry (this->logEntries[sz -1]);  // -1 is important, causes core dump cuase itr is past last entry of vector; 
                logEntry.pushST(line);

                return logEntry;
            }
        }
    }

    void serializeData() {
        // Dump the metrics and data to file(s); 
        //  <log_name>_ule.log      -> Uniq Log Entries (Ordered most occurring to least)
        //  <log_name> uste.log     -> Uniq Stack Trace Entries (Ordered most occurring to least)
        //  <log_name>_stats.log    -> stats () / metrics (); 
        
        // Create reverse multimap iterator
        multimap<int, string>::reverse_iterator it;
        // Create string iterator; 
        vector<string>::iterator sit;

        // These will always output on serialize; 
        ofstream uleLogFile(this->base_dir + this->ule_log); 
        multimap<int, string> messageEntriesOrdered;
        for (auto x : messageEntries) {
            messageEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }

		ofstream usteLogFile(this->base_dir + this->uste_log); 
        multimap<int, string> stackTraceEntriesOrdered;
        for (auto x : stackTraceEntries) {
            stackTraceEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }

		ofstream statsLogFile(this->base_dir + this->stats_log);

        // Iterate them reversely: 
        
        usteLogFile << this->header(" Unique Stack Trace Line Items ");
        for (it = stackTraceEntriesOrdered.rbegin(); it != stackTraceEntriesOrdered.rend(); it++) {
            usteLogFile << it->first << " " << it->second << endl;
        }

        uleLogFile << this->header(" Unique Log Entry Messages ");
        for (it = messageEntriesOrdered.rbegin(); it != messageEntriesOrdered.rend(); it++) {
            uleLogFile << it->first << " " << it->second << endl;
        }

        // Generate stats and iterate the stats vector; 
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

};
