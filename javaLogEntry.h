/*************
* jle.h
*   This file declares the class for JavaLogEntry; 
*   This class is responsible for: 
*    1. Storing the log lines and stack traces related to the log line; 
*    2. One object per Line of log (not counting stack trace)
*    3. All the lines of a stack trace -> TODO: convert to objects to stack trace line item objects (to be made); 
*************/

using namespace std;

class javaLogEntry {
private:
    bool containsStackTrace     = false;
    vector<string> lines        = {};
    vector<string> stackTrace   = {}; 

    // Line Item Variables; 
    string fileName             = "";
    string line                 = "";

    // Log File Fields
    string timestamp            = "";
    int long unsigned id        = 0; 
    string logLevel             = "";
    string message              = ""; 

public:
    javaLogEntry ();
    javaLogEntry (string ln);
    javaLogEntry (javaLogEntry const &logEntry);
    void dumpElements ();
    string getLine ();
    string getMessage ();
    vector<string> getStackTrace ();
    bool isStackTrace (string firstWord);
    void pushST (string li);
};