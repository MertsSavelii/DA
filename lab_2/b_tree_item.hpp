#ifndef B_TREE_ITEM_HPP
#define B_TREE_ITEM_HPP

#include <stdint.h>

const uint16_t MAX_KEY_SIZE = 256;

class BTreeItem {
    public:
        BTreeItem();
        BTreeItem(const BTreeItem& it);
        bool Empty();
        BTreeItem & operator = (const BTreeItem& it);
        ~BTreeItem();
    private:
        char Key[MAX_KEY_SIZE + 1];
        uint16_t KeySize;
        uint64_t Value;
};

BTreeItem::BTreeItem() {
    for(int i = 0; i < MAX_KEY_SIZE + 1; ++i)
        Key[i] = 0;
    KeySize = 0;
    Value = 0;
}

BTreeItem::BTreeItem(const BTreeItem& it) {
    for(int i = 0; i < MAX_KEY_SIZE + 1; ++i)
        Key[i] = it.Key[i];
    KeySize = it.KeySize;
    Value = it.Value;
}

bool BTreeItem::Empty() {
    return KeySize == 0 ? true : false;
}

BTreeItem & BTreeItem::operator = (const BTreeItem& it) {
    for(int i = 0; i < MAX_KEY_SIZE + 1; ++i)
        Key[i] = it.Key[i];
    KeySize = it.KeySize;
    Value = it.Value;
}

#endif /* B_TREE_ITEM_HPP*/