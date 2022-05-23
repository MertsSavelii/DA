#include "t_b_tree.hpp"
#include <utility>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string command;
    std::string keyWord;
    std::string path;
    uint64_t value;
    TBTree tree;
    while (std::cin >> command) {
        if (command == "+") {
            std::cin >> keyWord >> value;
            std::transform(keyWord.begin(), keyWord.end(), keyWord.begin(), tolower);
            TBTreeItem ToInsertItem(keyWord, value);
            if (tree.Search(ToInsertItem) != nullptr) {
                printf("Exist\n");
            } else {
                tree.Insert(ToInsertItem);
                printf("OK\n");
            }
        } else if (command == "-") {
            std::cin >> keyWord;
            std::transform(keyWord.begin(), keyWord.end(), keyWord.begin(), tolower);
            TBTreeItem ToEraseItem(keyWord, 0);
            if (tree.Search(ToEraseItem) == nullptr){
                printf("NoSuchWord\n");
            } else {
                tree.Erase(ToEraseItem);
                printf("OK\n");
            }
        } else if (command == "!") {
            std::cin >> command;
            if (command == "Save") {
                std::cin >> path;
                std::ofstream toWriteFile(path, std::ios::trunc | std::ios::binary);
                tree.Save(toWriteFile);
                toWriteFile.close();
                printf("OK\n");
            } else if (command == "Load") {
                std::cin >> path;
                std::ifstream toReadFile(path, std::ios::binary);
                tree.Load(toReadFile);
                toReadFile.close();
                printf("OK\n");
            }
        } else {
            std::transform(command.begin(), command.end(), command.begin(), tolower);
            TBTreeItem ToSearchItem(command, 0);
            TBTreeItem* FoundItem = tree.Search(ToSearchItem);
            if (FoundItem != nullptr) {
                printf("OK: %llu\n", FoundItem->Value());
            } else {
                printf("NoSuchWord\n");
            }
        }
    }
    return 0;
}