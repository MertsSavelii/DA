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
    void DeleteTree() {
        if (root == nullptr) {
            return;
        }
        root->DeleteNode();
        root = nullptr;
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
    void Save(std::ofstream& toWtiteFile) {
        if (root == nullptr) {
            size_t dataSize = 0;
            toWtiteFile.write(reinterpret_cast<const char *>(&dataSize), sizeof(size_t));
            return;
        }
        root->SaveNode(toWtiteFile);
    }
    void Load(std::ifstream& ToLoadFile) {
        DeleteTree();
        root = new TBTreeNode;
        root->LoadNode(ToLoadFile);
        if (root->NodeIsEmpty()) {
            delete root;
            root = nullptr;
        }
    }
};

#endif /*T_B_TREE_HPP*/