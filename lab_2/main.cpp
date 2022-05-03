#include "b_tree.hpp"
#include <iostream>
#include <utility>
#include <fstream>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string Command;
    TBTree tree;
    while (std::cin >> Command) {
        if (Command == "+") {
            std::string KeyWord;
            uint64_t Value;
            TBTreeItem ToInsertItem;
            std::cin >> KeyWord >> Value;
            std::transform(KeyWord.begin(), KeyWord.end(), KeyWord.begin(), tolower);
            ToInsertItem.Key = KeyWord;
            ToInsertItem.Value = Value;
            if (tree.Search(KeyWord) != nullptr) {
                printf("Exist\n");
            } else {
                tree.Insert(ToInsertItem);
                printf("OK\n");
            }
        } else if (Command == "-") {
            std::string KeyWord;
            std::cin >> KeyWord;
            std::transform(KeyWord.begin(), KeyWord.end(), KeyWord.begin(), tolower);
            TBTreeItem ToEraseItem;
            ToEraseItem.Key = KeyWord;
            if (tree.Search(KeyWord) == nullptr){
                printf("NoSuchWord\n");
            } else {
                tree.Erase(ToEraseItem);
                printf("OK\n");
            }
        } else if (Command == "!") {
            std::string Path;
            std::cin >> Command >> Path;
            if (Command == "Save") {
                std::ofstream ToWriteFile(Path, std::ios::trunc | std::ios::binary);
               // tree.Save(ToWriteFile);
                printf("OK\n");
            } else if (Command == "Load") {

            }
        } else {
            std::transform(Command.begin(), Command.end(), Command.begin(), tolower);
            TBTreeItem* FoundItem = tree.Search(Command);
            if (FoundItem != nullptr) {
                printf("OK: %llu\n", FoundItem->Value);
            } else {
                printf("NoSuchWord\n");
            }
        }
    }
    return 0;
}