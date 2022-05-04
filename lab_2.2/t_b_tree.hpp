#ifndef T_B_TREE_HPP
#define T_B_TREE_HPP

#include <fstream>
#include "t_b_tree_node.hpp"

class TBTree
{
private:
    TBTreeNode* root;
public:
    TBTree() {
        root = nullptr;
    }
    ~TBTree() {
        delete root;
    }

    TBTreeItem* Search(const std::string& keyForSearch);
    void Insert(const TBTreeItem& elemForInsert);
    void Erase(const TBTreeItem& elemForErase);
    void Save(const std::ofstream ToWtiteFile);
    void Load(const std::ofstream ToLoadFile);
};

TBTreeItem* TBTree::Search(const std::string& keyForSearch) {
    if (root == nullptr) {
        return nullptr;
    }
    return root->SearchInNode(keyForSearch);
}

void TBTree::Insert(const TBTreeItem& elemForInsert) {
    if (root == nullptr) {
        root = new TBTreeNode(elemForInsert);
        return;
    }
    if (root->NodeIsFull()) {
        root = root->SplitNode();
    }
    root->InsertToNode(elemForInsert);
}

void TBTree::Erase(const TBTreeItem& elemForErase) {
    // if (root == nullptr) {
    //     return;
    // }
    // root->EraseFromNode(const TBTreeItem& elemForErase);
    // if (root->data.size() == 0) {
    //     if (root->NodeIsLeaf()) {
    //         root = nullptr;
    //     } else {
    //         root = root->child[0];
    //     }
    // }
}

void TBTree::Save(const std::ofstream ToWtiteFile) {

}
void TBTree::Load(const std::ofstream ToLoadFile) {

}

#endif /*T_B_TREE_HPP*/