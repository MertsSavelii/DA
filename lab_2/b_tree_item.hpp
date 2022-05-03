#ifndef B_TREE_ITEM_HPP
#define B_TREE_ITEM_HPP

#include <stdint.h>
#include <string>

const uint16_t MAX_KEY_SIZE = 256;

class TBTreeItem {
public:
    TBTreeItem() {
        Key.resize(MAX_KEY_SIZE + 1);
        Value = 0;
    }
    ~TBTreeItem(){}

    std::string Key;
    uint64_t Value;
    friend bool operator<(const TBTreeItem& A, const TBTreeItem& B) {
        return (A.Key.compare(B.Key) == -1) ? true : false;
    }
};

#endif /* B_TREE_ITEM_HPP*/