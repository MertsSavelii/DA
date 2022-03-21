#include <iostream>
#include <fstream>

int main() {
    std::ofstream out("in_file.txt");
    int n = 0;
    std::cin >> n;
    for (int i = 0; i < n; i++)
        out << (rand() % 65535) << '\t' << rand() << '\n';
    out.close();
    return 0;
}