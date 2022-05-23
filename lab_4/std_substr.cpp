#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>

using namespace std;

int main() {
    string pattern;

    string inStr;
    string inWord;
    getline(cin, inStr);
    stringstream inStringStream(inStr);
    while(inStringStream >> inWord) {
        pattern += inWord + ' ';
    }


    string inText;
    while (!cin.eof()){
        getline(cin, inStr);
        stringstream inStringStream(inStr);
        while(inStringStream >> inWord) {
            inText += inWord + ' ';
        }
    }
    
    chrono::system_clock::time_point start = chrono::system_clock::now();
    while (inText.find(pattern));
    chrono::system_clock::time_point end = chrono::system_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "\n";
    return 0;
}