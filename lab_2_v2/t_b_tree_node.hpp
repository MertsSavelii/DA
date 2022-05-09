#ifndef T_B_TREE_NODE_HPP
#define T_B_TREE_NODE_HPP

#include <vector>
#include "t_b_tree_item.hpp"

const uint8_t TREE_DEGREE = 55;

class TBTreeNode
{
private:
    std::vector<TBTreeItem*> data;
    std::vector<TBTreeNode*> child;

    uint8_t BinarySearch(const std::vector<TBTreeItem*> Data, const TBTreeItem elem);
public:
    bool NodeIsLeaf();
    bool NodeIsFull();
    bool NodeIsMin();
    bool NodeIsEmpty();
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
    // For EraseFromNode
    bool ElemInNode(const uint8_t& findIndex, const TBTreeItem& elenForErase);
    void EraseFromLeaf(const uint8_t& findIndex);
    void EraseFromNonLeaf(const uint8_t& findIndex);
    TBTreeItem* FindMaxInSubTree();
    TBTreeItem* FindMinInSubTree();
    void Merge(const uint8_t findIndex);
    void BorrowFromLeftChild(const uint8_t findIndex);
    void BorrowFromRightChild(const uint8_t findIndex);
    void FillChild(const uint8_t& findIndex);
    // EraseFromNode
    void EraseFromNode(const TBTreeItem& elemForErase);
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
    bool TBTreeNode::NodeIsEmpty() {
        return (data.size() == 0) ? true : false;
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
        TBTreeNode* midNode = new TBTreeNode(*data[TREE_DEGREE - 1]);
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
        for (uint16_t i = 0; i < 2 * TREE_DEGREE - 1; ++i) {
            data[i] = nullptr;
        }
        delete this;
        return midNode;
    }
    void TBTreeNode::SplitChild(const uint8_t& childIndex) {
        TBTreeNode* splitNode = child[childIndex]->SplitNode();
        data.insert(data.begin() + childIndex, splitNode->data[0]);
        child[childIndex] = splitNode->child[0];
        child.insert(child.begin() + childIndex + 1, splitNode->child[1]);
        splitNode->data[0] = nullptr;
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

// For EraseFromNode
bool TBTreeNode::ElemInNode(const uint8_t& findIndex, const TBTreeItem& elenForErase) {
    return (findIndex < data.size() && *data[findIndex] == elenForErase) ? true : false;
}
void TBTreeNode::EraseFromLeaf(const uint8_t& findIndex) {
    // delete(data[findIndex]);
    // delete(child[findIndex]);// вообще он nullptr но всё же
    data.erase(data.begin() + findIndex);
    child.erase(child.begin() + findIndex);
}
void TBTreeNode::EraseFromNonLeaf(const uint8_t& findIndex) {
    if (!child[findIndex]->NodeIsMin()) {
        data[findIndex] = child[findIndex]->FindMaxInSubTree();
        child[findIndex]->EraseFromNode(*data[findIndex]);
        return;
    }
    if (!child[findIndex + 1]->NodeIsMin()) {
        data[findIndex] = child[findIndex]->FindMinInSubTree();
        child[findIndex]->EraseFromNode(*data[findIndex]);
        return;
    }
    TBTreeItem elemForErase = *data[findIndex];
    Merge(findIndex);
    child[findIndex]->EraseFromNode(elemForErase);
    return;
}
TBTreeItem* TBTreeNode::FindMaxInSubTree() {
    if (child[data.size()] != nullptr) {
        return child[data.size()]->FindMaxInSubTree();
    }
    return data[data.size() - 1];
}
TBTreeItem* TBTreeNode::FindMinInSubTree() {
    if (child[0] != nullptr) {
        return child[0]->FindMaxInSubTree();
    }
    return data[0];
}
void TBTreeNode::Merge(const uint8_t findIndex) {
    // push_back parent data to leftСhild
    child[findIndex]->data.push_back(data[findIndex]);
    // push_back rightСhild data to leftСhild
    child[findIndex]->data.insert(child[findIndex]->data.end(), child[findIndex + 1]->data.begin(), child[findIndex + 1]->data.end());
    // push_back rightСhild childs to leftСhild
    child[findIndex]->child.insert(child[findIndex]->child.end(), child[findIndex + 1]->child.begin(), child[findIndex + 1]->child.end());
    // erase parent data
    data[findIndex] = nullptr;
    delete data[findIndex];
    data.erase(data.begin() + findIndex);
    // nullptring rightСhild data
    for (uint16_t i = 0; i < child[findIndex + 1]->data.size(); ++i) {
        child[findIndex + 1]->data[i] = nullptr;
    }
    // nullptring rightСhild childs
    for (uint16_t i = 0; i < child[findIndex + 1]->child.size(); ++i) {
        child[findIndex + 1]->child[i] = nullptr;
    }
    // delete rightСhild
    delete child[findIndex + 1];
    // erase rightСhild from parent child
    child.erase(child.begin() + findIndex + 1);
}
bool TBTreeNode::NodeIsMin() {
    return (data.size() == TREE_DEGREE - 1) ? true : false;
}
void TBTreeNode::FillChild(const uint8_t& findIndex) {
    if (findIndex != 0 && !child[findIndex - 1]->NodeIsMin()) {
        BorrowFromLeftChild(findIndex);
        return;
    } 
    if (findIndex != data.size() && !child[findIndex + 1]->NodeIsMin()) {
        BorrowFromRightChild(findIndex);
        return;
    }
    if (findIndex != 0) {
        Merge(findIndex - 1);
        return;
    }
    if (findIndex != data.size()) {
        Merge(findIndex);
        return;
    }
}
void TBTreeNode::BorrowFromLeftChild(const uint8_t findIndex) {
    TBTreeNode* currChild = child[findIndex];
    TBTreeNode* leftChild = child[findIndex - 1];
    // insert parent data to currChild
    currChild->data.insert(currChild->data.begin(), data[findIndex - 1]);
    // parent data = leftChild max data
    data[findIndex - 1] = leftChild->data[leftChild->data.size() - 1];
    // erase max data and child from leftChild
    leftChild->data.erase(leftChild->data.end() - 1);
    // insert max child from leftChuld to begin of currChild 
    currChild->child.insert(currChild->child.begin(), leftChild->child[leftChild->child.size() - 1]);
    // erase max child from leftChild
    leftChild->child.erase(leftChild->child.end() - 1);    
}
void TBTreeNode::BorrowFromRightChild(const uint8_t findIndex) {
    TBTreeNode* currChild = child[findIndex];
    TBTreeNode* rightChild = child[findIndex + 1];
    // push_back parent data to currChild
    currChild->data.push_back(data[findIndex]);
    // parent data = rightChild min data
    data[findIndex] = rightChild->data[0];
    // erase min data from rightChild
    rightChild->data.erase(rightChild->data.begin());
    // insert min child from rightChild to end of currChild
    currChild->child.push_back(rightChild->child[0]);
    // erase min child from rightChild
    rightChild->child.erase(rightChild->child.begin());
}
// EraseFromNode
void TBTreeNode::EraseFromNode(const TBTreeItem& elemForErase) {
    uint8_t findIndex = BinarySearch(data, elemForErase);
    if (ElemInNode(findIndex, elemForErase)) {
        if (NodeIsLeaf()) {
            EraseFromLeaf(findIndex);
            return;
        } else {
            EraseFromNonLeaf(findIndex);
            return;
        }
        if (data.size() == 0) {
            *this = *child[findIndex];
        }
    }
    if (child[findIndex]->NodeIsMin()) {
        FillChild(findIndex);
        findIndex = BinarySearch(data, elemForErase);
    }
    child[findIndex]->EraseFromNode(elemForErase);
    return;
}

#endif /*T_B_TREE_NODE_HPP*/