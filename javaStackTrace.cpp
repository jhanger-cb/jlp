#include <string>
#include <vector>
#include <iostream>
#include <ostream>
#include "javaStackTrace.h"

javaStackTrace::javaStackTrace (string ln) {
    this->lineItems.push_back(ln);
}

javaStackTrace::javaStackTrace (javaStackTrace const &jst) {
    this->lineItems = jst.lineItems;
}

void javaStackTrace::dumpElements () {
    for ( auto x : lineItems) {
        cout << x << endl;
    }
}

vector<string> javaStackTrace::getStackTrace () {
    return this->lineItems;
}

void javaStackTrace::addLineItem (string lineItem) {
    lineItems.push_back (lineItem);
}