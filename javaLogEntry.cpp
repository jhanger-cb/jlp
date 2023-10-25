#include <string>
#include <vector>
#include <iostream>
#include <ostream>
#include <sstream>
#include "javaStackTrace.h"
#include "javaLogEntry.h"

javaLogEntry::javaLogEntry () {
    lines = {};
}

javaLogEntry::javaLogEntry (string ln) {
    this->line = ln;
    if(this->isStackTrace (ln)) {
        this->lines.push_back(ln);
        this->stackTrace.push_back(ln);
        this->containsStackTrace = true;
    }
    else {
        // Process First Line data into variables;
        //      All other data will be stored in stackTrace; 
        this->lines.push_back(ln);
        this->containsStackTrace = false;
    }
}

javaLogEntry::javaLogEntry (javaLogEntry const &logEntry) {
    this->containsStackTrace    = logEntry.containsStackTrace;
    this->lines                 = logEntry.lines;
    this->stackTrace            = logEntry.stackTrace;
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
    stackTrace.dumpElements();
}

vector<string>* javaLogEntry::getLines () {
    return &this->lines; 
}

string javaLogEntry::getMessage () {
    return this->message;
}

javaStackTrace& javaLogEntry::getStackTrace () {
    return this->stackTrace;
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

void javaLogEntry::push_back (string lineItem) {
    this->lines.push_back (lineItem);
    this->stackTrace.push_back (lineItem);
}