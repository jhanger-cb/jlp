// STL Inclusions: 
#include <fstream>
#include <iterator>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>

// Custom Inclusions: 
#include "jlp.h"

// Namespacing: 
using namespace std;


int main()
{
  /* Read line into vector instead of printing out directly*/
    vector<string> lines;

    ifstream file_in("jenkins.log");
    if (!file_in) {/*error*/}

    string line;
    while (getline(file_in, line))
    {
        istringstream ss(line);
        lines.push_back(line);
    }

    for ( long unsigned int i=0; i< lines.size(); i++) {
      //cout << lines[i] << endl;
      string word;
      istringstream iss(lines[i]);

      while (getline(iss, word, ' ')) {
        regex e ("^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]");
        regex a ("\tat");
        if (regex_match(word, e) || regex_match(word, a)) {
          cout << endl; 
        }
        cout << word << " ";
      }
    }
}
