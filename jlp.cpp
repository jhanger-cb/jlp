// STL Inclusions: 
#include <fstream>
#include <iterator>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <ctime>

// Custom Inclusions: 
#include "jlp.h"        // Class Defs;
#include "clip.h"       // Command Line Parsing; 

// Namespacing: 
using namespace std;

int main(int argc, char * argv[])
{
    /* Read line into vector instead of printing out directly*/
    vector<string> lines;
    struct tm t;
    t = {0};

    if(cmdOptionExists(argv, argv+argc, "-h"))
    {
        // Print Help Documentation; 
    }

    char * filename = getCmdOption(argv, argv + argc, "-f");

    if (filename)
    {
        ifstream file_in(filename);
        if (!file_in) {/*error*/}

        string line;
        while (getline(file_in, line))
        {
            struct tm tm; 
            string firstWord, ln; 
            istringstream ss(line);
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

        for ( long unsigned int i=0; i< lines.size(); i++) {
          //cout << lines[i] << endl;
        }
    }
}
