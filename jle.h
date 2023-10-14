#include <sstream>

using namespace std;

class javaLogEntry {
private:
    bool containsStackTrace;
    vector<string> lines,stackTrace; 

    // Line Item Variables; 
    string fileName, line;

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

    vector<string> getStackTrace () {
        return stackTrace;
    }
    vector<string> getLines () {
        return lines; 
    }

    void pushST (string li) {
        stackTrace.push_back (li);
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

    void dump () {
        for ( long unsigned int i=0; i < lines.size(); i++) {
            cout << lines[i] << endl;
        }
        for ( long unsigned int i=0; i< stackTrace.size(); i++) {
            cout << stackTrace[i] << endl;
        }
    }
};