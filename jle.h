/*************
* jle.h
*   This file declares the class for JavaLogEntry; 
*   This class is responsible for: 
*    1. Storing the log lines and stack traces related to the log line; 
*    2. One object per Line of log (not counting stack trace)
*    3. All the lines of a stack trace -> TODO: convert to objects to stack trace line item objects (to be made); 
*************/
#include <sstream>

using namespace std;

class javaLogEntry {
private:
    bool containsStackTrace;
    vector<string> lines,stackTrace; 

    // Line Item Variables; 
    string fileName, line;

    // Log File Fields
    string timestamp; 
    int long unsigned id; 
    string logLevel;
    string message; 

public:
    javaLogEntry () {
        lines = stackTrace = {};
    }

    javaLogEntry (string ln) {
        if(this->isStackTrace (ln)) {
            this->stackTrace.push_back(ln);
            this->containsStackTrace = true;
        }
        else {
            // Process First Line data into variables;
            //      All other data will be stored in stackTrace; 
            this->lines.push_back(ln);
        }
    }

    javaLogEntry (javaLogEntry const &logEntry) {
        this->containsStackTrace    = logEntry.containsStackTrace;
        this->lines                 = logEntry.lines;                               // probably need to iter item.add(...)
        this->stackTrace            = logEntry.stackTrace;                     // "
        this->fileName              = logEntry.fileName;
        this->line                  = logEntry.line;
        this->timestamp             = logEntry.timestamp;
        this->id                    = logEntry.id;
        this->logLevel              = logEntry.logLevel;
        this->message               = logEntry.message;
    }

    void dump () {
        for ( long unsigned int i=0; i < lines.size(); i++) {
            cout << lines[i] << endl;
        }
        for ( long unsigned int i=0; i< stackTrace.size(); i++) {
            cout << stackTrace[i] << endl;
        }
    }

    vector<string> getLines () {
        return lines; 
    }

    vector<string> getStackTrace () {
        return stackTrace;
    }

    bool isStackTrace (string firstWord) {
        string ln; 
        struct tm tm; 
        istringstream ss;

        if (strptime(firstWord.c_str(), "%Y-%m-%d", &tm)) {
            //getline(ss, ln);
            return false;
        } else {
            //getline(ss, ln);
            this->containsStackTrace = true; 
            return true; 
        }
    }

    void pushST (string li) {
        stackTrace.push_back (li);
    }
};