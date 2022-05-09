#ifndef T_B_TREE_NODE_HPP
#define T_B_TREE_NODE_HPP

#include "t_b_tree_item.hpp"
#include <vector>

const uint8_t TREE_DEGREE = 2;

class TBTreeNode
{
private:
    std::vector<TBTreeItem*> data;
    std::vector<TBTreeNode*> child;

    uint8_t BinarySearch(const std::vector<TBTreeItem*> Data, const TBTreeItem elem) {
        int16_t left = -1;
        uint8_t right = Data.size();
        while (left + 1 < right) {
            uint8_t mid = (left + right) / 2;
            if (*Data[mid] < elem) {
                left = mid;
            } else {
                right = mid;
            }
        }
        return right;
    }
    bool ItemInNode(const uint8_t& findIndex, const TBTreeItem& itemForSearch) {
        return (findIndex < data.size() && *data[findIndex] == itemForSearch) ? true : false;
    }
    bool NodeIsLeaf() {
        for (uint16_t i = 0; i < child.size(); ++i) {
            if (child[i] != nullptr) {
                return false;
            }
        }
        return true;
    }
    TBTreeNode* SplitNode() {
        // create midNode
        TBTreeNode* midNode = new TBTreeNode(*data[TREE_DEGREE - 1]);
        // create leftChild
        midNode->child[0] = new TBTreeNode;
        midNode->child[0]->data.resize(TREE_DEGREE - 1, nullptr);
        midNode->child[0]->child.resize(TREE_DEGREE, nullptr);
        // create rightChild
        midNode->child[1] = new TBTreeNode;
        midNode->child[1]->data.resize(TREE_DEGREE - 1, nullptr);
        midNode->child[1]->child.resize(TREE_DEGREE, nullptr);
        // initialization leftChild and rightChild data
        for (uint16_t i = 0; i < TREE_DEGREE - 1; ++i) {
            midNode->child[0]->data[i] = data[i];
            midNode->child[1]->data[i] = data[TREE_DEGREE + i];
        }
        // initialization leftChild and rightChild childs
        for (uint16_t i = 0; i < TREE_DEGREE; ++i) {
            midNode->child[0]->child[i] = child[i];
            midNode->child[1]->child[i] = child[TREE_DEGREE + i];
        }
        // nullptring data of splitingNode
        for (uint16_t i = 0; i < 2 * TREE_DEGREE - 1; ++i) {
            data[i] = nullptr;
        }
        // nullptring childs of splitingNode
        for (uint16_t i = 0; i < 2 * TREE_DEGREE; ++i) {
            child[i] = nullptr;
        }
        delete this;
        return midNode;
    }
    // for InsertToNode
    void InsertToLeaf(const uint8_t& findIndex, const TBTreeItem& itemForInsert) {
        data.insert(data.begin() + findIndex, new TBTreeItem(itemForInsert));
        child.insert(child.begin() + findIndex, nullptr);
    }
    void SplitChild(uint8_t& childIndex) {
        TBTreeNode* splitNode = child[childIndex]->SplitNode();
        data.insert(data.begin() + childIndex, splitNode->data[0]);
        child[childIndex] = splitNode->child[0];
        child.insert(child.begin() + childIndex + 1, splitNode->child[1]);
        splitNode->data[0] = nullptr;
        splitNode->child[0] = nullptr;
        splitNode->child[1] = nullptr;
        delete splitNode;
    }
    // for EraseFromNode
    bool NodeIsMin() {
        return (data.size() == TREE_DEGREE - 1) ? true : false;
    }
    void Merge(const uint8_t& findIndex);
        // for EraseFromLeaf
        
        //
    void EraseFromLeaf(const uint8_t& findIndex);
        // for EraseFromNonLeaf

        //
    void EraseFromNonLeaf(const uint8_t& findIndex);
        // for EraseFromNonLeaf

        //
    void FillChild(const uint8_t& findIndex);
public:
    TBTreeNode() {
        data.resize(1, nullptr);
        child.resize(2, nullptr);
    }
    TBTreeNode(const TBTreeItem& inItem) {
        data.resize(1, new TBTreeItem(inItem));
        child.resize(2, nullptr);
    }
    TBTreeNode(const std::string& inKey, const uint64_t& inValue) {
        data.resize(1, new TBTreeItem(inKey, inValue));
        child.resize(2, nullptr);
    }
    ~TBTreeNode() {
        for (uint8_t i = 0; i < data.size(); ++i) {
            delete data[i];
        }
        for (uint8_t i = 0; i < child.size(); ++i) {
            delete child[i];
        }
    }

    TBTreeItem* SearchInNode(const TBTreeItem& itemForSearch) {
        uint8_t findIndex = BinarySearch(data, itemForSearch);
        if (ItemInNode(findIndex, itemForSearch)) {
            return data[findIndex];
        }
        if (child[findIndex] != nullptr) {
            return child[findIndex]->SearchInNode(itemForSearch);
        }
        return nullptr;
    }
    void InsertToNode(const TBTreeItem& itemForInsert) {
        uint8_t findIndex = BinarySearch(data, itemForInsert);
        if (NodeIsLeaf()) {
            InsertToLeaf(findIndex, itemForInsert);
            return;
        }
        if (child[findIndex]->NodeIsFull()) {
            SplitChild(findIndex);
            findIndex = BinarySearch(data, itemForInsert);
        }
        child[findIndex]->InsertToNode(itemForInsert);
    }
    void EraseFromNode(const TBTreeItem& itemForErase) {
        uint8_t findIndex = BinarySearch(data, itemForErase);
        if (ItemInNode(findIndex, itemForErase)) {
            if (NodeIsLeaf()) {
                EraseFromLeaf(findIndex);
                return;
            }
            EraseFromNonLeaf(findIndex);
            return;
        }
        if (child[findIndex]->NodeIsMin()) {
            FillChild(findIndex);
            findIndex = BinarySearch(data, itemForErase);
        }
        child[findIndex]->EraseFromNode(itemForErase);
    }

    bool NodeIsFull() {
        return (data.size() == 2 * TREE_DEGREE - 1) ? true : false;
    }
    bool NodeIsEmpty() {
        return (data.size() == 0) ? true : false;
    }
    TBTreeNode* SplitRoot() {
        return SplitNode();
    }
    TBTreeNode* FillRoot() {
        if (!NodeIsLeaf()) {
            return child[0];
        }
        return nullptr;
    }
};  

#endif /*T_B_TREE_NODE_HPP*/