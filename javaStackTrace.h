/*************
* jle.h
*   This file declares the class for JavaLogEntry; 
*   This class is responsible for: 
*    1. Storing the log lines and stack traces related to the log line; 
*    2. One object per Line of log (not counting stack trace)
*    3. All the lines of a stack trace -> TODO: convert to objects to stack trace line item objects (to be made); 
*************/

/*******************************************
 * headLine and lineItems formats (Anatomy): 
 *  These inline stack traces from jenkins.logs differ from thread dump stack traces. 
 * headLine: %s: %s: %s*\n
 *           ^ exception DOM; exception msg; &e msg details;  
 *           groovy.lang.MissingPropertyException: No such property: POD_NAME for class: groovy.lang.Binding
 * lineItem(1): %s(at) %s.*.%s(%s:%d)
 *             ^at starter word; DOM; (filename:line number)
 *           at groovy.lang.Binding.getVariable(Binding.java:63)
 * lineItem(2): %s(Caused): %s(exception): %s (msg): %s (msg details);
 *           Caused: java.lang.RuntimeException: Failed to evaluate script: No such property: POD_NAME for class: groovy.lang.Binding
 *
 * Example 2:
 *           groovy.lang.MissingPropertyException: No such property: POD_NAME for class: groovy.lang.Binding
 *               at groovy.lang.Binding.getVariable(Binding.java:63)
 *           Caused: java.lang.RuntimeException: Failed to evaluate script: No such property: POD_NAME for class: groovy.lang.Binding
 * 
 * 
*******************************************/

using namespace std;

class javaStackTrace {
private:
    // Stack Trace Line Items; 
    string headLine             = "";
    vector<string> lineItems    = {};
    // Line Item Elements: 
    string caused               = "";
    string exception            = "";
    string exceptionMsg         = "";
    string exceptionMsgDetails  = "";

public:
    vector<string>* liPtr       = &lineItems; 
    javaStackTrace (); 
    javaStackTrace (string ln); 
    javaStackTrace (javaStackTrace const &jst);
    void dumpElements ();
    void push_back(string lineItem);
    vector<string>* getStackTrace () const;
    bool operator== (const javaStackTrace &jst) const;
    size_t hashit () const;
    friend ostream& operator<< (ostream &out, const javaStackTrace &jst);
};

// In order to create an unordered_map object we have to create helper hash class;
// unordered_map<javaStackTrace, int, javaStackHash> ... 
//
class javaStackHash {
public: 
    size_t operator()(const javaStackTrace& jst) const {
        return jst.hashit ();
    }
};