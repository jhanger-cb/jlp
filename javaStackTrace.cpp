#include <string>
#include <vector>
#include <iostream>
#include <ostream>
#include "javaStackTrace.h"

javaStackTrace::javaStackTrace () {
}

javaStackTrace::javaStackTrace (string ln) {
    this->headLine = ln;
    this->lineItems.push_back(ln);
}

javaStackTrace::javaStackTrace (javaStackTrace const &jst) {
    this->headLine              = jst.headLine;
    for ( auto x : jst.lineItems) {
        this->lineItems.push_back(x);
    }
    this->caused                = jst.caused;
    this->exception             = jst.exception;
    this->exceptionMsg          = jst.exceptionMsg;
    this->exceptionMsgDetails   = jst.exceptionMsgDetails;
}

void javaStackTrace::dumpElements () {
    for ( auto x : lineItems) {
        cout << x << endl;
    }
}

vector<string>* javaStackTrace::getStackTrace () const {
    return this->liPtr;
}

void javaStackTrace::push_back (string lineItem) {
    lineItems.push_back (lineItem);
}

bool javaStackTrace::operator== (const javaStackTrace &jst) const {
    bool ret = false;
    if (this->lineItems.size() != jst.liPtr->size ()) {
        return false;
    } else {
        //(this->lineItems == jst.lineItems) ? return true : return false; 
        int y = 0;
        ret = false;
        for (auto x : this->lineItems) {
            (x == jst.lineItems[y]) ? ret = true : ret = false;
            y++;
        }
        return ret;
    }
    return true;
}

size_t javaStackTrace::hashit () const {
    size_t hash = 0;
    for (auto x : this->lineItems) {
        hash += x.size();
    }
    return hash;
}

ostream& operator << (ostream &out, const javaStackTrace &jst) {
    for (size_t i=0; i<jst.liPtr[0].size(); i++) {
        //out << jst.liPtr[i];
        out << "" << jst.liPtr[0][i] << endl; //<< "\n" << jst.liPtr[i];
    }
    return out; 
}