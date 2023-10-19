#include <string>
#include <vector>
#include <iostream>
#include <ostream>
#include <sstream>
#include "javaLogEntry.h"

javaLogEntry::javaLogEntry () {
    lines = stackTrace = {};
}

javaLogEntry::javaLogEntry (string ln) {
    this->line = ln;
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

javaLogEntry::javaLogEntry (javaLogEntry const &logEntry) {
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

void javaLogEntry::dumpElements () {
    for ( long unsigned int i=0; i < lines.size(); i++) {
        cout << lines[i] << endl;
    }
    for ( long unsigned int i=0; i< stackTrace.size(); i++) {
        cout << stackTrace[i] << endl;
    }
}

string javaLogEntry::getLine () {
    return this->line; 
}

string javaLogEntry::getMessage () {
    return this->message;
}

vector<string> javaLogEntry::getStackTrace () {
    return stackTrace;
}

bool javaLogEntry::isStackTrace (string firstWord) {
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

void javaLogEntry::pushST (string li) {
    stackTrace.push_back (li);
}