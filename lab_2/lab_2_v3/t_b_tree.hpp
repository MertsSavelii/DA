#ifndef T_B_TREE_HPP
#define T_B_TREE_HPP

#include "t_b_tree_node.hpp"
#include <fstream>

class TBTree
{
private:
    TBTreeNode* root;
public:
    TBTree() {
        root = nullptr;
    }
    TBTree(TBTreeNode* inNode) {
        root = inNode;
    }
    ~TBTree() {
        delete root;
    }
    TBTreeItem* Search(const TBTreeItem& itemForSearch) {
        if (root == nullptr) {
            return nullptr;
        }
        return root->SearchInNode(itemForSearch);
    }
    void Insert(const TBTreeItem& itemForInsert) {
        if (root == nullptr) {
            root = new TBTreeNode(itemForInsert);
            return;
        }
        if (root->NodeIsFull()) {
            root = root->SplitRoot();
        }
        root->InsertToNode(itemForInsert);
        if (root->NodeIsFull()) {
            root = root->SplitRoot();
        }
    }
    void Erase(const TBTreeItem& itemForErase) {
        if (root == nullptr) {
            return;
        }
        if (root->NodeIsEmpty()) {
            root = root->FillRoot();
        }
        root->EraseFromNode(itemForErase);
        if (root->NodeIsEmpty()) {
            root = root->FillRoot();
        }
    }
    void Save(const std::ofstream ToWtiteFile);
    void Load(const std::ofstream ToLoadFile);
};

#endif /*T_B_TREE_HPP*/