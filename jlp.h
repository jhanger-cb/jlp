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

    // Line Item Variables; 
    string fileName, line;
    ifstream fh, fileStream;
    istringstream ss;
    vector<javaLogEntry> logEntries;
    // deprecated: vector<string> lines;
    string filters;
    vector<string> fileNames; 
    vector<string>::const_iterator iter; // Unused thusfar, may remove as a global iterator seems pointless ? "dweeb" : "that's one hypothesis"

    /* Log File Fields*/
    string timestamp; 
    int long unsigned id; 
    string logLevel;
    string message; 

public:
    javaLogParser (string fileName, string filters = "") {
        this->pStart = time(nullptr);       // Metrics for Efficiency; 
        this->fileName = fileName; 
        if (!filters.empty()){
            this->filters = filters;
        }
        // Metrics for Line Item Types; 
        lineCount=allCount=debugCount=errorCount=fatalCount=fineCount=finerCount=finestCount= infoCount= severeCount=stackTraceCount=traceCount=unknownCount=warnCount=0;
        
        this->processFile();
    }

    ~javaLogParser () {
        this->fh.close ();
        this->printStats ();
    }

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

    void printStats () {
        int elements = this->lineCount;
        cout.imbue(locale());
        cout << "\n`-._.-`-._.-`-> Log Parsing Summary " << this->fileName << "<-`-._.-`-._.-`" << endl;
        cout << "\tLines:\t\t\t\t" << elements << endl;
        cout << "\tStack Trace Lines:\t\t" << stackTraceCount << endl; 
        cout << "\t\tALL Entries:\t\t" << allCount << endl;
        cout << "\t\tDEBUG Entries:\t\t" << debugCount << endl;
        cout << "\t\tERROR Entries:\t\t" << errorCount << endl;
        cout << "\t\tFATAL Entries:\t\t" << fatalCount << endl;
        cout << "\t\tFINE Entries:\t\t" << fineCount << endl;
        cout << "\t\tFINER Entries:\t\t" << finerCount << endl;
        cout << "\t\tFINEST Entries:\t\t" << finestCount << endl;
        cout << "\t\tINFO Entries:\t\t" << infoCount << endl;
        cout << "\t\tOFF Entries:\t\t" << offCount << endl;
        cout << "\t\tSEVERE Entries:\t\t" << severeCount << endl;
        cout << "\t\tTRACE Entries:\t\t" << traceCount << endl;
        cout << "\t\tUNKNOWN Entries:\t" << unknownCount << endl;
        cout << "\t\tWARN Entries:\t\t" << warnCount << endl;
        cout << "\tStart Ts:\t\t" << pStart << endl;
        this->pEnd = time(nullptr);
        int duration = pEnd - pStart; 
        cout << "\tEnd Time:\t\t" << pEnd << endl;
        double lps; 
        (duration == 0) ? lps = elements : lps = elements / duration; 
        cout << "\tDuration:\t\t" << duration << " s" << endl;
        cout << "\tProcessed:\t\t" << lps/1000 << "K lines per second" << endl;
    }

    vector<javaLogEntry> getElements() const {
        return this->logEntries; 
    }; 

    string peekNextLine () {
        string localLine; 
        streampos curPos = this->fileStream.tellg();
        getline(this->fileStream, localLine);
        this->fileStream.seekg(curPos, ios_base::beg);

        return localLine;
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

    void dump () {
        for ( long unsigned int i=0; i< this->logEntries.size(); i++) {
          logEntries[i].dump ();
        }
    }

    void processFile() {
        this->fh = ifstream(this->fileName);
        if (!fh) { cout << "Error in File" << endl; }

        while (getline(this->fh, this->line)){
            string firstWord, timestamp, id, logLevel; 
            this->ss = istringstream(this->line);
            this->ss >> firstWord >> timestamp >> id >> logLevel; 
            
            //cout << "firstWord:\t" << firstWord << endl; 
            if (!this->isStackTrace(firstWord)) {
                javaLogEntry logEntry = this->processLine(line);
                this->logEntries.push_back(logEntry); 
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
};
