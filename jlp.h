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
        if (!fh) { cout << "Error in File" << endl; return 0; }
        
        // Load File into this->lines;
        while (getline(this->fh, this->line)){
            struct tm tm; 
            string firstWord, ln; 
            istringstream ss(this->line);
            ss >> firstWord; 
            if (strptime(firstWord.c_str(), "%Y-%m-%d", &tm)) {
              cout << "This is a head JVM Logging Event: " << endl;
              getline(ss, ln);
              cout << firstWord << ln << endl;
            } else if(firstWord.find("at")) {
              cout << "This line begins a stack trace: " << endl;
              getline(ss, ln);
              cout << firstWord << ln << endl;
            } else if(!firstWord.find("\n")) {
              cout << "New Line Detected: End of Stack Trace" << firstWord << endl;
            } else {
              getline(ss, ln);
              cout << "\t" << firstWord << ln << endl;
            }
            lines.push_back(line);
        }
    }

    ~javaLogParser () {
        this->fh.close();
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
