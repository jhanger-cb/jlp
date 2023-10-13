using namespace std;

class javaLogEntry {
private:
    bool lineType;
    int lineCount;
    vector<string> lines;
    string line;

public:
    javaLogEntry (bool lnT, string ln) {
        this->lineType = lnT;
    }

    bool isStackTrace () const {
        return lineType; 
    };

    string getLine () {
        return line; 
    }

    void push (string li) {
        lines.push_back (li);
    }
};