#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <ctype.h>
#include <sstream>

class intChar
{
private:
    uint32_t value;
public:
    uint16_t numInString, numOfString;
    friend bool operator<(const intChar& a, const intChar& b) {
        return (a.value < b.value) ? true : false;
    }
    friend bool operator==(const intChar& a, const intChar& b) {
        return (a.value == b.value) ? true : false;
    }
    friend std::istream& operator>> (std::istream &in, intChar &intChar) {
        in >> intChar.value;
        return in;
    }
};

void zFunc(const std::vector<intChar>& inString, std::vector<int>& zF) {
    int strLength = inString.size();
	zF.resize(strLength, 0);
	for (int i = 1, l = 0, r = 0; i < strLength - 1; ++i) {
		zF[i] = std::max(0, std::min (r - i, zF[i - l]));
		while (i + zF[i] < strLength && inString[zF[i]] == inString[i + zF[i]])
			++zF[i];
		if (i + zF[i] > r) {
			l = i;  
            r = i + zF[i];
        }
	}
}

void PatternRead(std::istream &in, std::vector<intChar>& pattern) {
    intChar inChar;
    std::string inString;
    getline(in, inString);
    std::stringstream inStream(inString);
    int wordNumber = 1;
    while (inStream >> inChar)
    {
        inChar.numInString = wordNumber;
        inChar.numOfString = 1;
        pattern.push_back(inChar);
        wordNumber++;
    }
}

void TextRead(std::istream &in, std::vector<intChar>& inText) {
    intChar inChar;
    std::string inString;
    int lineNumber = 1;
    while (!in.eof()) {
        getline(in, inString);
        std::stringstream inStream(inString);
        int wordNumber = 1;
        while (inStream >> inChar) {
            inChar.numInString = wordNumber;
            inChar.numOfString = lineNumber;
            inText.push_back(inChar);
            wordNumber++;
        }
        lineNumber++;
    };
}

void substringInTextSearch (std::istream &in, std::ostream &out) {
    std::vector<intChar> inText;
    PatternRead(in, inText);
    uint16_t patternSize = inText.size();
    TextRead(in, inText);

    std::vector<int> zFuncRes;
    zFunc(inText, zFuncRes);

    for (uint16_t i = patternSize; i < inText.size(); ++i) {
        if (zFuncRes[i] >= patternSize) {
            out << inText[i].numOfString << ", " << inText[i].numInString << '\n';
        }
    }
}

int main () {
    substringInTextSearch(std::cin, std::cout);
    return 0;
}