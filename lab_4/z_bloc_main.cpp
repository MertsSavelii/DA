#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>

using namespace std;

struct intChar
{
    uint32_t value;
    uint16_t numInString;
    uint16_t numOfString;
};

bool operator<(const intChar& a, const intChar& b) {
    return (a.value < b.value) ? true : false;
}
bool operator==(const intChar& a, const intChar& b) {
    return (a.value == b.value) ? true : false;
}

void zFunc(const vector<intChar>& inString, vector<int>& zFRes) {
    int strLength = inString.size();
	zFRes.resize(strLength, 0);
    int leftBound = 0;
    int rightBound = 0;
	for (int i = 1; i < strLength - 1; ++i) {
		zFRes[i] = max(0, min (rightBound - i, zFRes[i - leftBound]));
		while (i + zFRes[i] < strLength && inString[zFRes[i]] == inString[i + zFRes[i]]) {
        	++zFRes[i];    
        }
		if (i + zFRes[i] > rightBound) {
			leftBound = i;  
            rightBound = i + zFRes[i];
        }
	}
}

void PatternRead(istream &in, vector<intChar>& pattern) {
    string inString;
    getline(in, inString);
    stringstream inStream(inString);
    uint32_t inChar = 1;
    while (inStream >> inChar)
    {
        pattern.push_back({inChar, 0, 0});
    }
}

void TextRead(istream &in, vector<intChar>& inText) {
    string inString;
    uint32_t inChar;
    uint16_t lineNumber = 1;
    while (!in.eof()) {
        getline(in, inString);
        stringstream inStream(inString);
        uint16_t wordNumber = 1;
        while (inStream >> inChar) {
            inText.push_back({inChar, wordNumber, lineNumber});
            wordNumber++;
        }
        lineNumber++;
    };
}

void substringInTextSearch (istream &in, ostream &out) {
    vector<intChar> inText;
    PatternRead(in, inText);
    uint16_t patternSize = inText.size();
    TextRead(in, inText);

    vector<int> zFuncRes;
    zFunc(inText, zFuncRes);

    for (uint16_t i = patternSize; i < inText.size(); ++i) {
        if (zFuncRes[i] >= patternSize) { // вот тут ошибка должно быть ==
            out << inText[i].numOfString << ", " << inText[i].numInString << '\n';
        }
    }
}

int main () {
    substringInTextSearch(cin, cout);
    return 0;
}