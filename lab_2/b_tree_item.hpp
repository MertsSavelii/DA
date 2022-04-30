#ifndef B_TREE_ITEM_HPP
#define B_TREE_ITEM_HPP

#include <stdint.h>
#include <string>

const uint16_t MAX_KEY_SIZE = 256;

class BTreeItem {
public:
    BTreeItem() {
        Key.resize(MAX_KEY_SIZE + 1);
        KeySize = 0;
        Value = 0;
    }
    ~BTreeItem(){}

    uint16_t KeySize;
    std::string Key;
    uint64_t Value;
    friend bool operator<(const BTreeItem& A, const BTreeItem& B);
};

bool operator<(const BTreeItem& A, const BTreeItem& B) {
    return (A.Key < B.Key) ? true : false;
}

#endif /* B_TREE_ITEM_HPP*/