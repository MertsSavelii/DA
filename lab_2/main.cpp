#include "b_tree.hpp"

int main() {
    BTree tree;
    BTreeItem item;
    item.Key = "sfsfdsfsd";
    item.Value = 123;
    tree.Insert(item);
    if (tree.Search("sfsfdsfsd")) {
        printf("fine");
    }
    
    return 0;
}