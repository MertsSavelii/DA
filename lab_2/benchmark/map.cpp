#include <iostream>
#include <map>
#include <chrono>

int main() {
    std::map<std::string, unsigned long long> data;
    std::string buf;
    unsigned long long value;
    unsigned amount;
    std::cin >> amount;
    std::chrono::steady_clock::time_point pushStart = std::chrono::steady_clock::now();
    for (unsigned i = 0; i < amount; ++i) {
        std::cin >> buf >> value;
        if (data.count(buf)) {
            printf("Exist\n");
        }
        else {
            data.insert({buf, value});
            printf("OK\n");
        }
    }
    std::chrono::steady_clock::time_point pushFinish = std::chrono::steady_clock::now();
    unsigned timeOfPush = std::chrono::duration_cast<std::chrono::milliseconds>(pushFinish - pushStart).count();

    std::chrono::steady_clock::time_point findStart = std::chrono::steady_clock::now();
    for (unsigned i = 0; i < amount; ++i) {
        std::cin >> buf;
        if (data.count(buf)) {
            printf("OK: %lu\n", data[buf]);
        }
        else {
            printf("NoSuchWord\n");
        }
    }
    std::chrono::steady_clock::time_point findFinish = std::chrono::steady_clock::now();
    unsigned timeOfFind = std::chrono::duration_cast<std::chrono::milliseconds>(findFinish - findStart).count();

    std::chrono::steady_clock::time_point popStart = std::chrono::steady_clock::now();
    for (unsigned i = 0; i < amount; ++i) {
        std::cin >> buf >> value;
        if (data.count(buf)) {
            data.erase(buf);
            printf("OK\n");
        }
        else {
            printf("NoSuchWord\n");
        }
    }
    std::chrono::steady_clock::time_point popFinish = std::chrono::steady_clock::now();
    unsigned timeOfPop = std::chrono::duration_cast<std::chrono::milliseconds>(popFinish - popStart).count();

    std::cout << "!!!!! Time to push " << amount << " elements: " << timeOfPush << std::endl;
    std::cout << "!!!!! Time to find " << amount << " elements: " << timeOfFind << std::endl;
    std::cout << "!!!!! Time to pop " << amount << " elements: " << timeOfPop << std::endl;
    return 0;
}