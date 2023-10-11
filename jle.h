using namespace std;

class javaLogEntry {
private:
    enum lineType { SINGLE, MULTI }; 
    lineType lte; 
    string line;

public:
    lineType getLineType () const {
        return lte; 
    };

    string getLine () {
        return line; 
    }
};