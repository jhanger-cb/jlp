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
    string fileName, line;
    ifstream fh;
    ifstream fileStream;    // May remove;    
    istringstream ss;
    vector<string> lines; 
    vector<string>::const_iterator iter;

    /* Log File Fields*/
    string timestamp; 
    int long unsigned id; 
    string logLevel;
    string message; 

public:
    javaLogParser (string fileName) {
        string firstWord; 
        this->fh = ifstream(fileName);
        if (!fh) { cout << "Error in File" << endl; }

        // Load File into this->lines;
        while (getline(this->fh, this->line)){
            this->ss = istringstream(this->line);
            this->ss >> firstWord; 
            if (this->isStackTrace(firstWord)) {
                //cout << "This line is a part of a Stack Trace" << endl;
            }
            else {
                //cout << "This line is NOT part of a Stack Trace" << endl;
            }
            lines.push_back(line);
            this->lineCount++;
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
