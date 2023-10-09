#include "jle.h"
using namespace std; 

class javaLogParser {
private:
    int lineCount; 
    vector<string> lines; 
    vector<string>::const_iterator iter;
    string line; 
    ifstream fileName;

public:
    javaLogParser (){

    }
    vector<string> getLines() const {
        return lines; 
    }; 
};
