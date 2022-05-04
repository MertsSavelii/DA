#include "t_b_tree.hpp"
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
            std::cin >> KeyWord >> Value;
            std::transform(KeyWord.begin(), KeyWord.end(), KeyWord.begin(), tolower);
            TBTreeItem ToInsertItem(KeyWord, Value);
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
            TBTreeItem ToEraseItem(KeyWord, 0);
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
                //tree.Save(ToWriteFile);
                printf("OK\n");
            } else if (Command == "Load") {

            }
        } else {
            std::transform(Command.begin(), Command.end(), Command.begin(), tolower);
            TBTreeItem* FoundItem = tree.Search(Command);
            if (FoundItem != nullptr) {
                printf("OK: %llu\n", FoundItem->Value());
            } else {
                printf("NoSuchWord\n");
            }
        }
    }
    return 0;
}

// int main() {
//     std::ios_base::sync_with_stdio(false);
//     std::cin.tie(nullptr);
//     std::string command;
//     TBTree tree;
//     std::ifstream in("test.txt");
//     std::ofstream out("result.txt");
//     while (in >> command) {
//         if (command == "+") {
//             std::string keyWord;
//             uint64_t value;
//             in >> keyWord >> value;
//             std::transform(keyWord.begin(), keyWord.end(), keyWord.begin(), tolower);
//             TBTreeItem toInsertItem(keyWord, value);
//             if (tree.Search(keyWord) != nullptr) {
//                 out << "Exist\n";
//                 //printf("Exist\n");
//             } else {
//                 tree.Insert(toInsertItem);
//                 out << "OK\n";
//                 //printf("OK\n");
//             }
//         } else if (command == "-") {
//             std::string keyWord;
//             in >> keyWord;
//             std::transform(keyWord.begin(), keyWord.end(), keyWord.begin(), tolower);
//             TBTreeItem toEraseItem(keyWord, 0);
//             if (tree.Search(keyWord) == nullptr){
//                 out << "NoSuchWord\n";
//                 //printf("NoSuchWord\n");
//             } else {
//                 tree.Erase(toEraseItem);
//                 out << "OK\n";
//                 //printf("OK\n");
//             }
//         } else if (command == "!") {
//             std::string path;
//             in >> command >> path;
//             if (command == "Save") {
//                 std::ofstream ToWriteFile(path, std::ios::trunc | std::ios::binary);
//                 //tree.Save(ToWriteFile);
//                 out << "OK\n";
//                 //printf("OK\n");
//             } else if (command == "Load") {

//             }
//         } else {
//             std::transform(command.begin(), command.end(), command.begin(), tolower);
//             TBTreeItem* foundItem = tree.Search(command);
//             if (foundItem != nullptr) {
//                 out << "OK: " << foundItem->Value() << "\n";
//                 //printf("OK: %llu\n", foundItem->value);
//             } else {
//                 out << "NoSuchWord\n";
//                 //printf("NoSuchWord\n");
//             }
//         }
//     }
//     return 0;
// }