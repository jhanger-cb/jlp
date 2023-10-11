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
    int lineCount; 
    string fileName;
    ifstream fh;
    vector<string> lines; 
    vector<string>::const_iterator iter;
    string line; 
    ifstream fileStream;

    /* Log File Fields*/
    string timestamp; 
    int long unsigned id; 
    string logLevel;
    string message; 

public:
    javaLogParser (string fileName) {
        this->fh = ifstream(fileName);
    }

    ~javaLogParser () {

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
};
