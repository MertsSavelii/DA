#include "t_b_tree.hpp"
#include <iostream>
#include <utility>
#include <fstream>
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
            std::cin >> command >> path;
            if (command == "Save") {
                std::ofstream ToWriteFile(path, std::ios::trunc | std::ios::binary);
                tree.Save(ToWriteFile);
                printf("OK\n");
            } else if (command == "Load") {
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

// int main() {
//     std::ios_base::sync_with_stdio(false);
//     std::cin.tie(nullptr);
//     std::string command;
//     std::string keyWord;
//     uint64_t value;
//     TBTree tree;
//     std::ifstream in("test.txt");
//     std::ofstream out("result.txt");
//     while (in >> command) {
//         if (command == "+") {
//             in >> keyWord >> value;
//             std::transform(keyWord.begin(), keyWord.end(), keyWord.begin(), tolower);
//             TBTreeItem toInsertItem(keyWord, value);
//             if (tree.Search(toInsertItem) != nullptr) {
//                 out << "Exist" << std::endl;
//             } else {
//                 tree.Insert(toInsertItem);
//                 out << "OK" << std::endl;
//             }
//         } else if (command == "-") {
//             in >> keyWord;
//             std::transform(keyWord.begin(), keyWord.end(), keyWord.begin(), tolower);
//             TBTreeItem toEraseItem(keyWord, 0);
//             if (tree.Search(toEraseItem) == nullptr){
//                 out << "NoSuchWord" << std::endl;
//             } else {
//                 tree.Erase(toEraseItem);
//                 out << "OK" << std::endl;
//             }
//         } else if (command == "!") {
//             // std::string path;
//             // in >> command >> path;
//             // if (command == "Save") {
//             //     std::ofstream ToWriteFile(path, std::ios::trunc | std::ios::binary);
//             //     out << "OK\n";
//             // } else if (command == "Load") {
//             // }
//         } else {
//             std::transform(command.begin(), command.end(), command.begin(), tolower);
//             TBTreeItem toSearchItem(command, 0);
//             TBTreeItem* foundItem = tree.Search(toSearchItem);
//             if (foundItem != nullptr) {
//                 out << "OK: " << foundItem->Value() << std::endl;
//             } else {
//                 out << "NoSuchWord" << std::endl;
//             }
//         }
//     }
//     return 0;
// }