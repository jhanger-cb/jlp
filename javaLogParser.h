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

using namespace std; 

class javaLogParser {
private:
    // Global Level Variables not specific to any particular method: 
    string fileName;

    enum logLevelType { ALL = 1 << 1, CONFIG = 1 << 2, DBG = 1 << 3, ERROR = 1 << 4, FATAL = 1 << 5, FINE = 1 << 6, FINER = 1 << 7, FINEST = 1 << 8, INFO = 1 << 9, OFF = 1 << 10, SEVERE = 1 << 11, TRACE = 1 << 12, UNKNOWN = 1 << 13, WARN = 1 << 14 }; 

    // Metrics: 
    // Count each type of logType encountered; 
    int lineCount		= 0;
	int allCount		= 0;
	int debugCount		= 0;
	int errorCount		= 0;
	int exceptionCount	= 0;
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
    time_t pEnd = time(nullptr); 

    unordered_map<string, int> messageEntries, allEntries, debugEntries, errorEntries, exceptionEntries, fatalEntries, fineEntries, finerEntries, finestEntries, infoEntries, offEntries, severeEntries,  stackTraceLineEntries, traceEntries, unknownEntries, warningEntries; 
    unordered_map<javaStackTrace, int, javaStackHash> stackTraceEntries;
    
    // Line Item Specific Variables;
    string line;
    string firstWord;
    ifstream fh;
    istringstream ss;

    vector<javaLogEntry*> logEntries = {};

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
    string exception_log = "";
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


public:
    //Global Options paramount to this class:
    static bool debug; 
    static bool aggregate;
    static bool serialize;
    static bool dump;
    static bool stats;
    static string filters;
    javaLogEntry* currentLogEntry;
    static regex re;
    static regex reDate;
    static regex reException;

    // Member Function Declarations; 
    // Overloads/Constructors/Destructor
    javaLogParser (string fileName);
    javaLogParser (const javaLogParser& source);
    javaLogParser ();
    ~javaLogParser ();
    javaLogParser& operator +=(javaLogParser const &source);
    bool operator ==(javaLogParser const &target);

    // Static Functions: 
    static bool getDebug ();
    static bool setDebug (bool dbg);
    static bool getDump();
    static bool setDump (bool dmp);
    static bool getAggregate ();
    static bool setAggregate (bool agg);
    static bool getSerialize ();
    static bool setSerialize (bool ser);
    static bool getStats();
    static bool setStats (bool ser);
    static string getFilters ();
    static string setFilters (string fltrs);

    // The rest of the functions ;P 
    void addCounterMetrics (string logLevel);
    void addStackItem (string line);
    void dumpElements ();
    vector<string> generateStats ();
    logLevelType hashit (string const& inString);
    string header (string title, int style = 2);
    void initFileNames ();
    void initParser (string fn);
    bool isStackTrace ();
    bool hasException ();
    bool joinable (); 
    void join ();
    multimap<int, string> orderMap (unordered_map<string, int>& sourceMap);
    void printStats ();
    void processFile();
    javaLogEntry* processLine();
    void serializeData();
    string escaped(const string& input);
    javaLogEntry* getCurrentLogEntry();
};