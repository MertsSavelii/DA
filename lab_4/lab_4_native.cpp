#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>

using namespace std;

const long long WALL = -1;

struct intChar
{
    long long value;
    unsigned int numInString;
    unsigned int numOfString;
};

vector<int> zFuncNative(const vector<intChar>& inText) {
    unsigned int strLength = inText.size();
    vector<int> zf(strLength);
    for (int i = 0; i < strLength; ++i) {
        while (i + zf[i] < strLength && inText[zf[i]].value == inText[i + zf[i]].value) {
            zf[i]++;
        }
    }
    return zf;
}

void ReadPattern(vector<intChar>& inText) {
    long long inWord;
    string inString;
    getline(cin, inString);
    stringstream inStringStream(inString);
    while (inStringStream >> inWord) {
        inText.push_back({inWord});
    }
}

void ReadText(vector<intChar>& inText) {
    long long inWord;
    string inString;
    unsigned int lineNumber = 1;
    while (!cin.eof()) {
        getline(cin, inString);
        stringstream inStringStream(inString);
        unsigned int wordNumber = 1;
        while (inStringStream >> inWord) {
            inText.push_back({inWord, wordNumber, lineNumber});
            wordNumber++;
        }
        lineNumber++;
    };
}

int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); 
    cout.tie(nullptr);

    vector<intChar> inText;

    ReadPattern(inText);

    unsigned int patternSize = inText.size();
    inText.push_back({WALL});

    ReadText(inText);
    chrono::system_clock::time_point start = chrono::system_clock::now();
    vector<int> zFuncRes = zFuncNative(inText);
    

    for (unsigned int i = patternSize; i < inText.size(); ++i) {
        if (zFuncRes[i] == patternSize) {
            //cout << inText[i].numOfString << ", " << inText[i].numInString << '\n';
        }
    }
    chrono::system_clock::time_point end = chrono::system_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "\n";
    return 0;
}