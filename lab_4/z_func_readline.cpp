#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>

using namespace std;

struct intChar
{
    int64_t value;
    uint32_t numInString;
    uint32_t numOfString;
};

vector<int> zFunc(const vector<intChar>& inText) {
    unsigned int strLength = inText.size();
    vector<int> zf(strLength);
    int left = 0;
    int right = 0;
    for (int i = 1; i < strLength; i++) {
        zf[i] = max(0, min(right - i, zf[i - left]));
        while (inText[zf[i]].value == inText[i + zf[i]].value && i + zf[i] < strLength) {
            zf[i]++;
        }
        if (i + zf[i] > right) {
            left = i;
            right = i + zf[i];
        }
    }
    return zf;
}

int main () {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); 
    cout.tie(nullptr);

    vector<intChar> inText;
    int64_t inWord;

    string inString;
    getline(cin, inString);
    stringstream inStringStream(inString);
    while (inStringStream >> inWord) {
        inText.push_back({inWord});
    }

    unsigned int patternSize = inText.size();
    inText.push_back({-1});

    uint32_t lineNumber = 1;
    while (!cin.eof()) {
        getline(cin, inString);
        stringstream inStringStream(inString);
        uint32_t wordNumber = 1;
        while (inStringStream >> inWord) {
            inText.push_back({inWord, wordNumber, lineNumber});
            wordNumber++;
        }
        lineNumber++;
    };

    vector<int> zFuncRes = zFunc(inText);
    
    for (unsigned int i = patternSize; i < inText.size(); ++i) {
        if (zFuncRes[i] == patternSize) {
            cout << inText[i].numOfString << ", " << inText[i].numInString << '\n';
        }
    }
    return 0;
}