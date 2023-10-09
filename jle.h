using namespace std;

class javaLogEntry {
private:
    enum lineType { SINGLE, MULTI }; 
    lineType lte; 

public:
    lineType getLineType () const {
        return lte; 
    };
};