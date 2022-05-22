#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct TInputEntry {
    unsigned int value;
    int stringNumber;
    int wordNumber;
};

vector<int> countZFunction(const vector<TInputEntry>& text) {
    unsigned int n = text.size();
    vector<int> result(n);
    int leftBound = 0;
    int rightBound = 0;
    for (int i = 1; i < n; i++) {
        result[i] = max(0, min(rightBound - i, result[i - leftBound]));
        while (i + result[i] < n
        && text[result[i]].value == text[i + result[i]].value)
        {
            result[i]++;
        }
        if (i + result[i] > rightBound) {
            leftBound = i;
            rightBound = i + result[i];
        }
    }
    return result;
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    vector <unsigned int> pattern;

    string patternInput;
    getline(cin, patternInput);

    vector<TInputEntry> text;

    stringstream patternStream(patternInput);

    unsigned int inputWord;

    while (patternStream >> inputWord) {
        text.push_back({inputWord, 0});
    }

    unsigned int patternSize = text.size();

    text.push_back({2546, 0});

    string textLine;
    int lineNumber = 1;

    while (!cin.eof()) {
        getline(cin, textLine);
        stringstream textLineStream(textLine);
        int wordNumber = 1;
        while (textLineStream >> inputWord) {
            text.push_back({inputWord, lineNumber, wordNumber});
            wordNumber++;
        }
        lineNumber++;
    }

    auto zVector = countZFunction(text);

    for (unsigned int i = patternSize + 1; i < zVector.size(); i++) {
        if (zVector[i] == patternSize) {
            cout << text[i].stringNumber << ", " << text[i].wordNumber << '\n';
        }
    }

    return 0;
}