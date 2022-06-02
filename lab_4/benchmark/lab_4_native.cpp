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
        for (int j = 0; j < strLenght; ++j) {
            if (inText[j] == inText[j - i] && j >= i)) {
                zf[i]++;
            }
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
    vector<intChar> inText;

    ReadPattern(inText);
    inText.push_back({WALL});
    ReadText(inText);

    chrono::system_clock::time_point start = chrono::system_clock::now();
    zFuncNative(inText);
    chrono::system_clock::time_point end = chrono::system_clock::now();

    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "\n";
    return 0;
}