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

    if(cmdOptionExists(argv, argv+argc, "-h") || !cmdOptionExists(argv, argv+argc, "-")) {
        cout << "Usage: jlp -f [FILE(S)]\n\
  Parse and Aggregate JVM Log File Events\n\
  -f  <filename(s)>\n" << endl;
    }

    char * filename = getCmdOption(argv, argv + argc, "-f");

    if (filename) {
        javaLogParser jlp = javaLogParser(filename);
    }

}
