#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <ctype.h>

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

std::vector<int> zFunc (std::vector<intChar> inString) {
    int strLength = inString.size();
	std::vector<int> zF (strLength, 0);
	for (int i = 1, l = 0, r = 0; i < strLength - 1; ++i) {
		zF[i] = std::max(0, std::min (r - i, zF[i - l]));
		while (i + zF[i] < strLength && inString[zF[i]] == inString[i + zF[i]])
			++zF[i];
		if (i + zF[i] > r) {
			l = i;  
            r = i + zF[i];
        }
	}
	return zF;
}

std::vector<intChar> PatternRead(std::istream &in) {
    std::vector<intChar> pattern;
    int wordCount = 1;
    intChar inChar;
    char c;
    do {
        if (std::cin >> inChar) {
            inChar.numInString = wordCount;
            inChar.numOfString = 1;
            pattern.push_back(inChar);
            wordCount++;
        }
        while(!isdigit(c = std::cin.peek()) && c != '\n' && c != EOF) {
            std::cin.get();
        }
    } while (c != '\n' && c != EOF);
    std::cin.get();
    return pattern;
}

std::vector<intChar> TextRead(std::istream &in) {
    std::vector<intChar> inText;
    intChar inChar;
    char c;
    int newLineCunt = 1;
    do
    {
        int wordCount = 1; 
        do {
            while(!isdigit(c = std::cin.peek()) && c != '\n' && c != EOF) {
                std::cin.get();
            }
            if (isdigit(c)) {
                std::cin >> inChar;
                inChar.numInString = wordCount;
                inChar.numOfString = newLineCunt;
                inText.push_back(inChar);
                wordCount++;
            }
        } while (c != '\n' && c != EOF);
        if (c == '\n') {
            std::cin.get();
        }
        newLineCunt++;
    } while (c = std::cin.peek() != EOF);
    return inText;
}

void substringInTextSearch () {
    std::vector<intChar> pattern, inText, stringForZFunc;
    pattern = PatternRead(std::cin);
    inText = TextRead(std::cin);
    stringForZFunc.insert(stringForZFunc.begin(), pattern.begin(), pattern.end());
    stringForZFunc.insert(stringForZFunc.end(), inText.begin(), inText.end());

    std::vector<int> zFuncRes = zFunc(stringForZFunc);

    for (uint16_t i = pattern.size(); i < stringForZFunc.size(); ++i) {
        if (zFuncRes[i] >= pattern.size()) {
            printf("%d, %d\n", stringForZFunc[i].numOfString, stringForZFunc[i].numInString);
        }
    }
}

int main () {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    substringInTextSearch();
    return 0;
}