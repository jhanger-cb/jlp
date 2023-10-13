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
    int lineCount, allCount, debugCount, errorCount, fatalCount, fineCount, finerCount, finestCount, infoCount, severeCount, stackTraceCount, traceCount, unknownCount, warnCount; 
    enum logType { ALL, CONFIG, DEBUG, ERROR, FATAL, FINE, FINER, FINEST, INFO, SEVERE, TRACE, UNKNOWN, WARN }; 
    time_t pStart, pEnd; 

    // Line Item Variables; 
    string fileName, line;
    ifstream fh, fileStream;
    istringstream ss;
    vector<javaLogEntry> logEntries;
    vector<string> lines; 
    vector<string>::const_iterator iter;

    /* Log File Fields*/
    string timestamp; 
    int long unsigned id; 
    string logLevel;
    string message; 

public:
    javaLogParser (string fileName) {
        this->pStart = time(nullptr);       // Metrics for Efficiency; 
        // Metrics for Line Item Types; 
        lineCount=allCount=debugCount=errorCount=fatalCount=fineCount=finerCount=finestCount= infoCount= severeCount=stackTraceCount=traceCount=unknownCount=warnCount=0;
        // Open File from cli param; 
        this->fh = ifstream(fileName);
        if (!fh) { cout << "Error in File" << endl; }

        // Load File into this->lines;
        // Converting to load file into vector of Java Log Entries: 
        //   Each entry object could contain a stack trace and up to 1024 elements of string objects representing an individual line; 
        while (getline(this->fh, this->line)){
            string firstWord, timestamp, id, logLevel; 
            this->ss = istringstream(this->line);
            this->ss >> firstWord >> timestamp >> id >> logLevel; 
            addCounterMetrics (logLevel);
            
            if (this->isStackTrace(firstWord)) {
                //cout << "This line is a part of a Stack Trace" << endl;
                this->stackTraceCount++;
                this->unknownCount--;
            }
            else {
                //cout << "This line is NOT part of a Stack Trace" << endl;
            }
            lines.push_back(line);
            this->lineCount++;
        }
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
                //cout << "Failure to log Metrics for UNKNOWN Log Level Type" << endl;
        }
    }

    void printStats () {
        int elements = this->lines.size();
        cout.imbue(locale());
        cout << "\n`-._.-`-._.-`-> Log Parsing Summary <-`-._.-`-._.-`" << endl;
        cout << "\tLines:\t\t\t" << elements << endl;
        cout << "\tStack Traces:\t\t" << stackTraceCount << endl; 
        cout << "\t\tALL Entries:\t\t" << allCount << endl;
        cout << "\t\tDEBUG Entries:\t\t" << debugCount << endl;
        cout << "\t\tERROR Entries:\t\t" << errorCount << endl;
        cout << "\t\tFATAL Entries:\t\t" << fatalCount << endl;
        cout << "\t\tFINE Entries:\t\t" << fineCount << endl;
        cout << "\t\tFINER Entries:\t\t" << finerCount << endl;
        cout << "\t\tFINEST Entries:\t\t" << finestCount << endl;
        cout << "\t\tINFO Entries:\t\t" << infoCount << endl;
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
        cout << "\tEfficiency:\t\t" << lps << " Events per second" << endl;
    }

    vector<string> getLines() const {
        return lines; 
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
    void dump (){
        for ( long unsigned int i=0; i< this->lines.size(); i++) {
          cout << lines[i] << endl;
        }
    }
};
