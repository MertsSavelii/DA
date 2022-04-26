#ifndef B_TREE_NODE_HPP
#define B_TREE_NODE_HPP

#include <stdint.h>
#include <stdio.h>
#include <vector>
#include "b_tree_item.hpp"

const uint16_t TREE_DEGREE = 12;

class BTreeNode {
    public:
        BTreeNode();
        std::vector<BTreeItem*> Data;
        std::vector<BTreeNode*> Child;
        bool Leaf;
        ~BTreeNode();

        void PushNodeInFile(BTreeNode* curr, FILE* file);
        BTreeNode* PullNodeFromFile(FILE* file);
        BTreeNode* FindNode(BTreeNode* root, uint8_t* pos, const BTreeItem* Pattern);
        BTreeNode* SplitNode();
        void InsertNotFull();
        void Erase();
};

BTreeNode::BTreeNode() {
    Data.resize(1, nullptr);
    Child.resize(2, nullptr);
    Leaf = true;
}

BTreeNode::~BTreeNode() {
    for (uint64_t i = 0; i < Child.size(); ++i)
        delete Child[i];
}

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
    delete this;// хз сработает ли но должно, если что просто добавить входным аргументом ноду которую сплитуем
    return newNode;
}

void  BTreeNode::PushNodeInFile(BTreeNode* curr, FILE* file) {
    uint8_t size = 0;
    if (curr == nullptr) {
        fwrite(&size, sizeof(uint8_t), 1, file);
    } else {
        size = curr->Data.size();
        fwrite(&size, sizeof(uint8_t), 1, file);
        for (uint8_t i = 0; i < curr->Data.size(); ++i) {
            fwrite(&curr->Data[i]->KeySize, sizeof(uint16_t), 1, file);
            fwrite(&curr->Data[i]->Key, sizeof(char), MAX_KEY_SIZE + 1, file);
            fwrite(&curr->Data[i]->Value, sizeof(uint64_t), 1, file);
        }
        for (uint8_t i = 0; i < curr->Child.size(); ++i) {
            PushNodeInFile(curr->Child[i], file);
        }
    }
}

BTreeNode*  BTreeNode::PullNodeFromFile(FILE* file) {
    uint8_t size = 0;
    fread(&size, sizeof(uint8_t), 1, file);
    if (size == 0) {
        return nullptr;
    } else {
        BTreeNode* curr = new BTreeNode;
        curr->Data.resize(size);
        curr->Child.resize(size + 1);
        for (uint8_t i = 0; i < curr->Data.size(); ++i) {
            fread(&curr->Data[i]->KeySize, sizeof(uint16_t), 1, file);
            fread(&curr->Data[i]->Key, sizeof(char), MAX_KEY_SIZE + 1, file);
            fread(&curr->Data[i]->Value, sizeof(uint64_t), 1, file);
        }
        for (uint8_t i = 0; i < curr->Child.size(); ++i) {
            curr->Child[i] = PullNodeFromFile(file);
        }
        return curr;
    }
}

BTreeNode* BTreeNode::FindNode(BTreeNode* root, uint8_t* pos, const BTreeItem* Pattern) {
    for(*pos = 0; *pos < root->Data.size(); ++*pos) {
        if (Pattern->Key == root->Data[*pos]->Key) {
            return root;
        }
    }
    if (Pattern->Key == root->Data[0]->Key) {
        return FindNode(root->Child[0], pos, Pattern);
    }
    if (Pattern->Key == root->Data[root->Data.size() - 1]->Key) {
        return FindNode(root->Child[root->Data.size() - 1], pos, Pattern);
    }
    for(*pos = 0; *pos < root->Data.size() - 1; ++*pos) {
        if (Pattern->Key > root->Data[*pos]->Key && 
            Pattern->Key < root->Data[*pos + 1]->Key) {
            return FindNode(root->Child[*pos + 1], pos, Pattern);
        }
    }
    // if (Key == DataI.key) return DataI;
    // if (Key < Data0.key) return Find(Child0, Key);
    // if (Key > DataLast.key) return Find(ChildLast, Key);
    // if (Key > DataI.key && Key < DataI+1.key) return Find(ChildI+1, Key);
}

void BTreeNode::InsertNotFull() {
    
}

void BTreeNode::Erase() {

}

#endif /* B_TREE_NODE_HPP */