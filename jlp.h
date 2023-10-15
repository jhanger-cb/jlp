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
    // Metrics Variables; 
    int lineCount, allCount, debugCount, errorCount, fatalCount, fineCount, finerCount, finestCount, infoCount, offCount, severeCount, stackTraceCount, traceCount, unknownCount, warnCount; 
    enum logType { 
        ALL = 1 << 1,
        CONFIG = 1 << 2,
        DEBUG = 1 << 3,
        ERROR = 1 << 4,
        FATAL = 1 << 5,
        FINE = 1 << 6,
        FINER = 1 << 7,
        FINEST = 1 << 8,
        INFO = 1 << 9,
        OFF = 1 << 10,
        SEVERE = 1 << 11,
        TRACE = 1 << 12,
        UNKNOWN = 1 << 13,
        WARN = 1 << 14
    }; 
    time_t pStart, pEnd; 
    unordered_map<string, int> stackEntries; // eg: <jenkins.security.ImpersonatingExecutorService$1.run(ImpersonatingExecutorService.java:68), 1688>
    unordered_map<string, int> messageEntries; 

    // Line Item Variables; 
    string fileName, line;
    ifstream fh, fileStream;
    istringstream ss;
    vector<javaLogEntry> logEntries;
    string filters;
    vector<string> fileNames; 
    vector<string>::const_iterator iter; // Unused thusfar, may remove as a global iterator seems pointless ? "dweeb" : "that's one hypothesis"

    // Log File Fields
    string timestamp; 
    int long unsigned id; 
    string logLevel;
    string message; 

    // Logging Output Vars
    string ule_log, uste_log, stats_log; 

public:
    javaLogParser (string fileName, string filters = "") {
        this->pStart = time(nullptr);       // Metrics for Efficiency; 
        this->fileName = fileName; 
        if (!filters.empty()){
            this->filters = filters;
        }
        
        // Logging output filename propagation:
        this->ule_log      = "logs/" + fileName + "_ule.log";
        this->uste_log     = "logs/" + fileName + "_uste.log";
        this->stats_log    = "logs/" + fileName + "_stats.log";

        // Metrics for Line Item Types; 
        lineCount=allCount=debugCount=errorCount=fatalCount=fineCount=finerCount=finestCount= infoCount= severeCount=stackTraceCount=traceCount=unknownCount=warnCount=0;
        
        this->processFile();
    }

    ~javaLogParser () {
        this->fh.close ();
        this->printStats ();
        this->serialize ();
    }

    bool operator ==(javaLogParser const &target) {
        // Compare pertinent values; 
        //      logLevel ; message
        //      date, time, and id can be disregarded for line comparisons; 
        return true; 
    }

    void addCounterMetrics (string logLevel) {
        switch (hashit(logLevel)) {
            case ALL:
                this->allCount++;
                break;
            case DEBUG:
                this->debugCount++;
                break;
            case ERROR:
                this->errorCount++;
                break;
            case FATAL:
                this->fatalCount++;
                break;
            case FINE:
                this->fineCount++;
                break;
            case FINER:
                this->finerCount++;
                break;
            case FINEST:
                this->finestCount++;
                break;
            case INFO:
                this->infoCount++;
                break;
            case OFF:
                this->offCount++;
                break;
            case SEVERE:
                this->severeCount++;
                break;
            case TRACE:
                this->traceCount++;
                break;
            case WARN:
                this->warnCount++;
                break;
            default:
                this->unknownCount++;
                cout << "Failure to log Metrics for UNKNOWN Log Level Type: " << logLevel << endl;
        }
    }

    void addStackItem (string line) {
        istringstream ss(line);
        string firstWord, traceItem; 
        ss >> firstWord;
        getline(ss,traceItem);
        stackEntries[traceItem]++;
    }

    void dump () {
        for ( long unsigned int i=0; i< this->logEntries.size(); i++) {
          logEntries[i].dump ();
        }

        // Order the unordered_maps; 
        multimap<int, string> stackEntriesOrdered; 
        multimap<int, string> messageEntriesOrdered; 
        for (auto x : stackEntries) {
            stackEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        for (auto x : messageEntries) {
            messageEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }

        // Create a map reverse iterator
        multimap<int, string>::reverse_iterator it;

        // Iterate them reversely: 
        
        cout << "\n==============================" << " Unique Stack Trace Entries " << "==============================\n" << endl;
        for (it = stackEntriesOrdered.rbegin(); it != stackEntriesOrdered.rend(); it++) {
            cout << it->first << " " << it->second << endl;
        }
        cout << "\n==============================" << " Unique Log Entry Messages " << "==============================\n" << endl;
        for (it = messageEntriesOrdered.rbegin(); it != messageEntriesOrdered.rend(); it++) {
            cout << it->first << " " << it->second << endl;
        }
    }

    vector<string> generateStats () {
        vector<string> stats; 
        int elements = this->lineCount;
        stats.push_back ("\n`-._.-`-._.-`-> Log Parsing Summary " + this->fileName + "<-`-._.-`-._.-`\n");
        stats.push_back ("\tLines:\t\t\t\t" + to_string(elements) + "\n");
        stats.push_back ("\tStack Trace Lines:\t\t" + to_string(stackTraceCount) + "\n"); 
        stats.push_back ("\tUniq Stack Trace Items:\t\t" + to_string(stackEntries.size ()) + "\n");
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
        if(warnCount != 0){ stats.push_back ("\t\tWARN Entries:\t\t" + to_string(warnCount) + "\n"); }
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

    bool isStackTrace (string firstWord) {
        string ln; 
        struct tm tm; 

        if (strptime(firstWord.c_str(), "%Y-%m-%d", &tm)) {
            getline(ss, ln);
            return false;
            // cout << firstWord << ln /*<< "\t\t // This is a head JVM Logging Event " */<< endl;
        } else {
            getline(ss, ln);
            return true; 
            // cout << firstWord << ln << endl;
        }
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
        this->fh = ifstream(this->fileName);
        if (!fh) { cout << "Error in File" << endl; }

        while (getline(this->fh, this->line)){
            string firstWord, timestamp, id, logLevel, msg; 
            this->ss = istringstream(this->line);
            this->ss >> firstWord >> timestamp >> id >> logLevel; 
            getline(this->ss, msg);  // Get remaining line entries as one object; 
            
            //cout << "firstWord:\t" << firstWord << endl; 
            if (!this->isStackTrace(firstWord)) {
                
                javaLogEntry logEntry = this->processLine(line);
                this->logEntries.push_back(logEntry); 
                this->messageEntries[msg]++;
                addCounterMetrics (logLevel);
            }
            else {
                //  Recursively add/pop line to previous javaLogEntry into stack trace vector; 
                this->processLine(line, true);
            }
        }
    }

    javaLogEntry processLine(string line, bool stackTrace = false) {
        if(!stackTrace) {
            //cout << "Initial Log Entry - Not a Stack Trace:" << line << endl;
            javaLogEntry logEntry = javaLogEntry(line);
            this->lineCount++;
            return logEntry;
        } 
        else {
            this->lineCount++;
            this->stackTraceCount++;

            this->addStackItem(line);

            if(this->logEntries.size() -1 == 0) {
                javaLogEntry logEntry (line);
                return logEntry;
            } else {
                javaLogEntry logEntry = this->logEntries[this->logEntries.size() -1]; 
                logEntry.pushST(line);

                return logEntry;
            }
        }
    }

    void serialize() {
        // Dump the metrics and data to file(s); 
        //  <log_name>_ule.log      -> Uniq Log Entries (Ordered most occurring to least)
        //  <log_name> uste.log     -> Uniq Stack Trace Entries (Ordered most occurring to least)
        //  <log_name>_stats.log    -> stats () / metrics (); 
        ofstream uleLogFile(this->ule_log), usteLogFile(this->uste_log), statsLogFile(this->stats_log); 
                // Order the unordered_maps; TODO: make reused code for the ordering of maps, so far failed; find way; 
        multimap<int, string> stackEntriesOrdered;
        multimap<int, string> messageEntriesOrdered;
        for (auto x : stackEntries) {
            stackEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }
        for (auto x : messageEntries) {
            messageEntriesOrdered.insert(pair<int, string>(x.second, x.first));
        }

        // Create a map reverse iterator
        multimap<int, string>::reverse_iterator it;
        vector<string>::iterator sit;

        // Iterate them reversely: 
        
        usteLogFile << "\n==============================" << " Unique Stack Trace Entries " << "==============================\n" << endl;
        for (it = stackEntriesOrdered.rbegin(); it != stackEntriesOrdered.rend(); it++) {
            usteLogFile << it->first << " " << it->second << endl;
        }

        uleLogFile << "\n==============================" << " Unique Log Entry Messages " << "==============================\n" << endl;
        for (it = messageEntriesOrdered.rbegin(); it != messageEntriesOrdered.rend(); it++) {
            uleLogFile << it->first << " " << it->second << endl;
        }

        statsLogFile << "\n==============================" << " Java Log Parsing Statsistics " << "==============================\n" << endl;
        vector<string> stats = this->generateStats ();
        for (sit = stats.begin(); sit != stats.end(); ++sit) {
            statsLogFile << *sit;
        }

        uleLogFile.close ();
        usteLogFile.close ();
        statsLogFile.close ();
    }
};
