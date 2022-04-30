#ifndef B_TREE_HPP
#define B_TREE_HPP

#include "b_tree_node.hpp"

class BTree {
public:
    BTree();
    ~BTree();
    bool Search(std::string Key);
    void Insert(BTreeItem& elem);
    void Erase(BTreeItem& elem);
    void Save();
    void Load();

private:
    BTreeNode* Root;
};

BTree::BTree() {
    Root = nullptr;
}

BTree::~BTree() {
    delete Root;
}

bool BTree::Search(std::string Key) {
    return Root->FindKey(Key);
}

void BTree::Insert(BTreeItem& elem) {
    if(Root == nullptr) {
        Root = new BTreeNode;
        Root->Data[0] = elem;
        Root->Child[0] = nullptr;
        Root->Child[1] = nullptr;
    } else {
        if(Root->NodeIsFull()) {
            Root = Root->SplitNode();
        }
        Root->InsertToNode(elem);
    }
}

void BTree::Erase(BTreeItem& elem) {

}

#endif /* B_TREE_HPP*/