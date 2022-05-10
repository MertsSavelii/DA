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
    std::ifstream in("test.txt");
    std::ofstream out("result.txt");
    while (in >> Command) {
        if (Command == "+") {
            std::string KeyWord;
            uint64_t Value;
            TBTreeItem ToInsertItem;
            in >> KeyWord >> Value;
            std::transform(KeyWord.begin(), KeyWord.end(), KeyWord.begin(), tolower);
            ToInsertItem.Key = KeyWord;
            ToInsertItem.Value = Value;
            if (tree.Search(KeyWord) != nullptr) {
                out << "Exist\n";
                //printf("Exist\n");
            } else {
                tree.Insert(ToInsertItem);
                out << "OK\n";
                //printf("OK\n");
            }
        } else if (Command == "-") {
            std::string KeyWord;
            in >> KeyWord;
            std::transform(KeyWord.begin(), KeyWord.end(), KeyWord.begin(), tolower);
            TBTreeItem ToEraseItem;
            ToEraseItem.Key = KeyWord;
            if (tree.Search(KeyWord) == nullptr){
                out << "NoSuchWord\n";
                //printf("NoSuchWord\n");
            } else {
                tree.Erase(ToEraseItem);
                out << "OK\n";
                //printf("OK\n");
            }
        } else if (Command == "!") {
            std::string Path;
            in >> Command >> Path;
            if (Command == "Save") {
                std::ofstream ToWriteFile(Path, std::ios::trunc | std::ios::binary);
                //tree.Save(ToWriteFile);
                out << "OK\n";
                //printf("OK\n");
            } else if (Command == "Load") {

            }
        } else {
            std::transform(Command.begin(), Command.end(), Command.begin(), tolower);
            TBTreeItem* FoundItem = tree.Search(Command);
            if (FoundItem != nullptr) {
                out << "OK: " << FoundItem->Value << "\n";
                //printf("OK: %llu\n", FoundItem->Value);
            } else {
                out << "NoSuchWord\n";
                //printf("NoSuchWord\n");
            }
        }
    }
    return 0;
}