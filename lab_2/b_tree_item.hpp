#ifndef B_TREE_ITEM_HPP
#define B_TREE_ITEM_HPP

#include <stdint.h>
#include <string>

const uint16_t MAX_KEY_SIZE = 256;

class BTreeItem {
    public:
        BTreeItem();
        ~BTreeItem();

        uint16_t KeySize;
        std::string Key;
        uint64_t Value;
};

BTreeItem::BTreeItem() {
    Key.resize(MAX_KEY_SIZE + 1);
    KeySize = 0;
    Value = 0;
}

#endif /* B_TREE_ITEM_HPP*/