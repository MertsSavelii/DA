#ifndef B_TREE_NODE_HPP
#define B_TREE_NODE_HPP

#include <stdint.h>
#include <stdio.h>
#include <vector>
#include "b_tree_item.hpp"

const uint8_t TREE_DEGREE = 2;

class BTreeNode {
public:
    std::vector<BTreeItem> Data;
    std::vector<BTreeNode*> Child;
    BTreeNode();
    ~BTreeNode();

    bool FindKey(std::string Key);

    // for InsertToNode
    BTreeNode* SplitNode();
    bool NodeIsLeaf();
    bool NodeIsFull();
    uint8_t BinarySearch(std::vector<BTreeItem>& Data, BTreeItem& elem);
    void SplitChild(uint8_t ChildIndex);
    // InsertToNode
    void InsertToNode(BTreeItem& elem);

    // for EraseFormNode
    bool ElemInNode(uint8_t index, BTreeItem& elem);
    void EraseFromLeaf(uint8_t);
    void EraseFromNonLeaf(uint8_t);
    BTreeItem FindMaxInSubTree();
    BTreeItem FindMinInSubTree();
    bool NodeIsMin();
    void Merge(uint8_t index);
    void FillChild(uint8_t index);
    void BorrowFromLeftChild(uint8_t index);
    void BorrowFromRightChild(uint8_t index);
    // EraseFormNode
    void BTreeNode::EraseFromNode(BTreeItem& elem);
};

BTreeNode::BTreeNode() {
    Data.resize(1);
    Child.resize(2);
}

BTreeNode::~BTreeNode() {
    for (uint8_t i = 0; i < Child.size(); ++i)
        delete Child[i];
}

// FindNode
bool BTreeNode::FindKey(std::string Key) {
    for (int i = 0; i < Data.size(); ++i) {
        if (Key == Data[i].Key) {
            return true;
        }
    }
    if (Key < Data[0].Key && Child[0] != nullptr) {
        return Child[0]->FindKey(Key);
    }
    if (Key < Data[Data.size() - 1].Key && Child[Child.size() - 1] != nullptr) {
        return Child[Child.size() - 1]->FindKey(Key);
    }
    for (int i = 0; i < Data.size(); ++i) {
        if (Key > Data[i].Key && Key < Data[i + 1].Key && Child[i + 1] != nullptr) {
            return Child[i + 1]->FindKey(Key);
        }
    }
    return false;
}

// InsertToNode
void BTreeNode::InsertToNode(BTreeItem& elem) {
    uint8_t index = BinarySearch(Data, elem);

    if (Child[index] == nullptr) {
        Data.insert(Data.begin() + index, elem);
        Child.insert(Child.begin() + index, nullptr);
    } else {
        if (Child[index]->NodeIsFull()) {
            SplitChild(index);
        } else {
            Child[index]->InsertToNode(elem);
        }
    }
}

BTreeNode* BTreeNode::SplitNode() {
    BTreeNode* newNode = new BTreeNode;
    newNode->Data[0] = Data[TREE_DEGREE - 1];
    newNode->Child[0] = new BTreeNode;
    newNode->Child[1] = new BTreeNode;
    for (uint8_t i = 0; i < TREE_DEGREE - 1; ++i) {
        newNode->Child[0]->Data[i] = Data[i];
        newNode->Child[0]->Child[i] = Child[i];
        newNode->Child[1]->Data[i] = Data[TREE_DEGREE + i];
        newNode->Child[1]->Child[i] = Child[TREE_DEGREE + i];
    }
    for (uint8_t i = 0; i < 2 * TREE_DEGREE; ++i) {
        Child[i] = nullptr;
    }
    delete this;// хз сработает ли, но должно, если что просто добавить входным аргументом ноду которую сплитуем
    return newNode;
}

bool BTreeNode::NodeIsLeaf() {
    for (int i = 0; i < Child.size(); ++i)
        if (Child[i] != nullptr)
            return false;
    return true;
}

bool BTreeNode::NodeIsFull() {
    return (Data.size() == 2 * TREE_DEGREE - 1) ? true : false;
}

uint8_t BTreeNode::BinarySearch(std::vector<BTreeItem>& Data, BTreeItem& elem) {
    uint8_t left = -1;
    uint8_t right = Data.size();
    while (left + 1 < right) {
        uint8_t mid = (left + right) / 2;
        if (Data[mid] < elem) {
            left = mid;
        }
        else {
            right = mid;
        }
    }
    return right;
}

void BTreeNode::SplitChild(uint8_t ChildIndex) {
    BTreeNode* SplitNode = Child[ChildIndex]->SplitNode();
    Data.insert(Data.begin() + ChildIndex, SplitNode->Data[0]);
    Child[ChildIndex] = SplitNode->Child[1];
    Child.insert(Child.begin() + ChildIndex, SplitNode->Child[0]);
    SplitNode->Child[0] = nullptr;
    SplitNode->Child[1] = nullptr;
    delete SplitNode;
}

// EraseFromNode
void BTreeNode::EraseFromNode(BTreeItem& elem) {
    uint8_t index = BinarySearch(Data, elem);
    if (ElemInNode(index, elem)) {
        if (NodeIsLeaf()) {
            EraseFromLeaf(index);
        } else {
            EraseFromNonLeaf(index);
        }
    } else {
        if (Child[index]->NodeIsMin()) {
            FillChild(index); 
        } else {
            EraseFromChild();
        }
    }
}

bool BTreeNode::ElemInNode(uint8_t index, BTreeItem& elem) {
    return (index < Data.size() && Data[index].Key == elem.Key) ? true : false;
}

void BTreeNode::EraseFromLeaf(uint8_t index) {
    Data.erase(Data.begin() + index);
}

void BTreeNode::EraseFromNonLeaf(uint8_t index) {
    if (!Child[index]->NodeIsMin()) {
        BTreeItem leftMax = Child[index]->FindMaxInSubTree();
        Data[index] = leftMax;
        EraseFromNode(leftMax);
    } else if (!Child[index+ 1]->NodeIsMin()) {
        BTreeItem rightMin = Child[index + 1]->FindMinInSubTree();
        Data[index] = rightMin;
        EraseFromNode(rightMin);
    } else {
        Merge(index);
        Child[index]->EraseFromNode(Data[index]);
    }
}

BTreeItem BTreeNode::FindMaxInSubTree() {
    if (Child[Data.size()] != nullptr) {
        return Child[Data.size()]->FindMaxInSubTree();
    } else {
        return Data[Data.size() - 1];
    }
}

BTreeItem BTreeNode::FindMinInSubTree() {
    if (Child[0] != nullptr) {
        return Child[0]->FindMaxInSubTree();
    } else {
        return Data[0];
    }
}

bool BTreeNode::NodeIsMin() {
    return (Data.size() < TREE_DEGREE) ? true : false;
}

void BTreeNode::Merge(uint8_t index) {
    Child[index]->Data.push_back(Data[index]);
    Child[index]->Data.insert(Child[index]->Data.end(), Child[index + 1]->Data.begin(), Child[index + 1]->Data.end());
    if (!Child[index]->NodeIsLeaf()) {
        Child[index]->Child.insert(Child[index]->Child.end(), Child[index + 1]->Child.begin(), Child[index + 1]->Child.end());
    }
    Data.erase(Data.begin() + index);
    delete(Child[index + 1]);
    Child.erase(Child.begin() + index + 1);
}

void BTreeNode::FillChild(uint8_t index) {
    if (index != 0 && !Child[index - 1]->NodeIsMin()) {
        BorrowFromLeftChild(index);
    } else if (index != Data.size() && !Child[index + 1]->NodeIsMin()) {
        BorrowFromRightChild(index);
    } else {
        if (index != Data.size()) {
            Merge(index);
        } else {
            Merge (index - 1);
        }
    }
}

void BTreeNode::BorrowFromLeftChild(uint8_t) {
    
}

void BTreeNode::BorrowFromRightChild(uint8_t) {
    
}

#endif /* B_TREE_NODE_HPP */