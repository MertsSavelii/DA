#ifndef T_B_TREE_ITEM_HPP
#define T_B_TREE_ITEM_HPP

#include <cstdint>
#include <string>

class TBTreeItem
{
private:
    std::string key;
    uint64_t value;
public:
    TBTreeItem(const TBTreeItem& inItem) {
        key = inItem.key;
        value = inItem.value;
    }
    TBTreeItem(const std::string& inKey, const uint64_t& inValue) {
        key = inKey;
        value = inValue;
    }
    ~TBTreeItem() = default;
    std::string Key() {
        return key;
    }
    uint64_t Value() {
        return value;
    }
    friend bool operator<(const TBTreeItem& a, const TBTreeItem& b) {
        return (a.key.compare(b.key) < 0) ? true : false;
    }
    friend bool operator>(const TBTreeItem& a, const TBTreeItem& b) {
        return (a.key.compare(b.key) > 0) ? true : false;
    }
    friend bool operator==(const TBTreeItem& a, const TBTreeItem& b) {
        return (a.key.compare(b.key) == 0) ? true : false;
    }
};

#endif /*T_B_TREE_ITEM_HPP*/