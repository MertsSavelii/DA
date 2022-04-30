#ifndef B_TREE_NODE_HPP
#define B_TREE_NODE_HPP

#include <stdint.h>
#include <stdio.h>
#include <vector>
#include "b_tree_item.hpp"

const uint8_t TREE_DEGREE = 12;

class BTreeNode {
public:
    BTreeNode();
    std::vector<BTreeItem> Data;
    std::vector<BTreeNode*> Child;
    ~BTreeNode();

    bool FindKey(std::string Key);

    // for InsertToNode
    BTreeNode* SplitNode();
    bool NodeIsLeaf();
    bool NodeIsFull();
    uint8_t BinarySearch(std::vector<BTreeItem>& Data, BTreeItem& elem);
    void BalancingChild(uint8_t ChildIndex);

    void InsertToNode(BTreeItem& elem);
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
    if (!NodeIsLeaf()) {
        if (Key < Data[0].Key) {
            return Child[0]->FindKey(Key);
        }
        if (Key < Data[Data.size()].Key) {
            return Child[Child.size()]->FindKey(Key);
        }
        for (int i = 0; i < Data.size() - 1; ++i) {
            if (Key > Data[i].Key && Key < Data[i + 1].Key) {
                return Child[i + 1]->FindKey(Key);
            }
        }
    } 
    return false;
}

// InsertToNode
BTreeNode* BTreeNode::SplitNode() {
    BTreeNode* newNode = new BTreeNode;
    newNode->Data[0] = Data[TREE_DEGREE - 1];
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

void BTreeNode::BalancingChild(uint8_t ChildIndex) {
    BTreeNode* SplitNode = Child[ChildIndex]->SplitNode();
    Data.insert(Data.begin() + ChildIndex, SplitNode->Data[0]);
    Child[ChildIndex] = SplitNode->Child[1];
    Child.insert(Child.begin() + ChildIndex, SplitNode->Child[0]);
    SplitNode->Child[0] = nullptr;
    SplitNode->Child[1] = nullptr;
    delete SplitNode;
}

void BTreeNode::InsertToNode(BTreeItem& elem) {
    uint8_t index = BinarySearch(Data, elem);

    if (NodeIsLeaf()) {
        Data.insert(Data.begin() + index, elem);
        Child.insert(Child.begin() + index, nullptr);
    } else {
        if (Child[index]->NodeIsFull()) {
            BalancingChild(index);
        } else {
            Child[index]->InsertToNode(elem);
        }
    }
}



#endif /* B_TREE_NODE_HPP */