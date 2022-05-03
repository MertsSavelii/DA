#ifndef B_TREE_HPP
#define B_TREE_HPP

#include "b_tree_node.hpp"

class TBTree {
public:
    TBTree();
    ~TBTree();
    TBTreeItem* Search(std::string Key);
    void Insert(TBTreeItem& elem);
    void Erase(TBTreeItem& elem);
    void Save(std::ofstream ToWtiteFile);
    void Load(std::ofstream ToLoadFile);

private:
    TBTreeNode* Root;
};

TBTree::TBTree() {
    Root = nullptr;
}

TBTree::~TBTree() {
    delete Root;
}

TBTreeItem* TBTree::Search(std::string Key) {
    if (Root == nullptr) {
        return nullptr;
    }
    return Root->FindKey(Key);
}

void TBTree::Insert(TBTreeItem& elem) {
    if(Root == nullptr) {
        Root = new TBTreeNode;
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

void TBTree::Erase(TBTreeItem& elem) {
    if (Root == nullptr) {
        return;
    } else {
        Root->EraseFromNode(elem);
    }
}

void TBTree::Save(std::ofstream ToWtiteFile) {

}

#endif /* B_TREE_HPP*/