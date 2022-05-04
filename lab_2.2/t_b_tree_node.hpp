#ifndef T_B_TREE_NODE_HPP
#define T_B_TREE_NODE_HPP

#include <vector>
#include "t_b_tree_item.hpp"

const uint8_t TREE_DEGREE = 2;

class TBTreeNode
{
private:
    std::vector<TBTreeItem*> data;
    std::vector<TBTreeNode*> child;

    uint8_t BinarySearch(const std::vector<TBTreeItem*> Data, const TBTreeItem elem);
public:
    bool NodeIsLeaf();
    bool NodeIsFull();
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

    // SearchInNode
    TBTreeItem* SearchInNode(const std::string& keyForSearch);
    // For InsertToNode
    TBTreeNode* SplitNode();
    void SplitChild(const uint8_t& childIndex);
    // InsertToNode
    void InsertToNode(const TBTreeItem& itemForInsert);
};



    uint8_t TBTreeNode::BinarySearch(const std::vector<TBTreeItem*> Data, const TBTreeItem elem) {
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

    bool TBTreeNode::NodeIsLeaf() {
        for (uint16_t i = 0; i < child.size(); ++i) {
            if (child[i] != nullptr) {
                return false;
            }
        }
        return true;
    }

    bool TBTreeNode::NodeIsFull() {
        return (data.size() == 2 * TREE_DEGREE - 1) ? true : false;
    }

    // SearchInNode
    TBTreeItem* TBTreeNode::SearchInNode(const std::string& keyForSearch) {
        uint8_t findIndex = BinarySearch(data, TBTreeItem(keyForSearch, 0));


        if (findIndex < data.size()) {
            if (data[findIndex]->Key() == keyForSearch) {
                return data[findIndex];
            }
        }
        if (child[findIndex] != nullptr) {
            return child[findIndex]->SearchInNode(keyForSearch);
        }
        return nullptr;        
    }
    
    // For InsertToNode
    TBTreeNode* TBTreeNode::SplitNode() {
        TBTreeNode* midNode = new TBTreeNode(*data[TREE_DEGREE]);
        midNode->child[0] = new TBTreeNode;
        midNode->child[0]->data.resize(TREE_DEGREE - 1, nullptr);
        midNode->child[0]->child.resize(TREE_DEGREE, nullptr);
        midNode->child[1] = new TBTreeNode;
        midNode->child[1]->data.resize(TREE_DEGREE - 1, nullptr);
        midNode->child[1]->child.resize(TREE_DEGREE, nullptr);
        for (uint16_t i = 0; i < TREE_DEGREE - 1; ++i) {
            midNode->child[0]->data[i] = data[i];
            midNode->child[1]->data[i] = data[TREE_DEGREE + i]; 
        }
        for (uint16_t i = 0; i < TREE_DEGREE; ++i) {
            midNode->child[0]->child[i] = child[i];
            midNode->child[1]->child[i] = child[TREE_DEGREE + i];
        }
        for (uint16_t i = 0; i < 2 * TREE_DEGREE; ++i) {
            child[i] = nullptr;
        }
        delete this;
        return midNode;
    }
    
    void TBTreeNode::SplitChild(const uint8_t& childIndex) {
        TBTreeNode* splitNode = child[childIndex]->SplitNode();
        data.insert(data.begin() + childIndex, splitNode->data[0]);
        child[childIndex] = splitNode->child[0];
        child.insert(child.begin() + childIndex + 1, splitNode->child[1]);
        splitNode->child[0] = nullptr;
        splitNode->child[1] = nullptr;
        delete splitNode;
    }
    // InsertToNode
    void TBTreeNode::InsertToNode(const TBTreeItem& itemForInsert) {
        uint8_t findIndex = BinarySearch(data, itemForInsert);
        if (NodeIsLeaf()) {
            data.insert(data.begin() + findIndex, new TBTreeItem(itemForInsert));
            child.insert(child.begin() + findIndex, nullptr);
        } else {
            if (child[findIndex]->NodeIsFull()) {
                SplitChild(findIndex);
                findIndex = BinarySearch(data, itemForInsert);
            }
            child[findIndex]->InsertToNode(itemForInsert);
        }
    }







#endif /*T_B_TREE_NODE_HPP*/