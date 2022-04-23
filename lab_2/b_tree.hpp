#ifdef B_TREE_HPP
#define B_TREE_HPP

#include <stdint.h>
#include <vector>
#include "b_tree_item.hpp"

const uint16_t TREE_DEGREE= 12;

class BTreeNode {
    BTree();
    std::vector<BTreeItem*> Data;
    std::vector<BTreeNode*> Child;
    ~BTree();

    void PushNodeInFile(BTreeNode* curr, FILE* file);
    BTreeNode* PullNodeFromFile(FILE* file);
    BTreeNode* FindNode(const uint16_t KeySize, const char& Key[MAX_KEY_SIZE + 1]);
    void Insert();
    void Erase();
}

BTreeNode::BTreeNode() {
    Data(1);
    Child(2);
}

BTreeNode::~BTreeNode() {
    for (uint64_t i = 0; i < Child.Size; ++i)
        delete Child[i];
}

void PushNodeInFile(BTreeNode* curr, FILE* file) {
    uint8_t size = 0;
    if (curr == nullptr) {
        fwrite(&size, sizeof(uint8_t), 1, file);
    } else {
        size = curr->Data.Size();
        fwrite(&size, sizeof(uint8_t), 1, file);
        for (uint8_t i = 0; i < curr->Data.Size(); ++i) {
            fwrite(&curr->Data[i].KeySize, sizeof(uint16_t), 1, file);
            fwrite(&curr->Data[i].Key, sizeof(char), MAX_KEY_SIZE + 1, file);
            fwrite(&curr->Data[i].Value, sizeof(uint64_t), 1, file);
        }
        for (uint8_t i = 0; i < curr->Child.Size(); ++i) {
            PushNodeInFile(curr->Child[i], file);
        }
    }
}

BTreeNode* PullNodeFromFile(FILE* file) {
    uint8_t size = 0;
    fread(&size, sizeof(uint8_t), 1, file);
    if (size == 0) {
        return nullptr;
    } else {
        BTreeNode* curr = new BTreeNode;
        curr->Data.Resize(size);
        curr->Child.Resize(size + 1);
        for (uint8_t i = 0; i < curr->Data.Size(); ++i) {
            fread(&curr->Data[i].KeySize, sizeof(uint16_t), 1, file);
            fread(&curr->Data[i].Key, sizeof(char), MAX_KEY_SIZE + 1, file);
            fread(&curr->Data[i].Value, sizeof(uint64_t), 1, file);
        }
        for (uint8_t i = 0; i < curr->Child.Size(); ++1) {
            curr->Child[i] = PullNodeFromFile(file);
        }
        return curr;
    }
}

BTreeNode* FindNode(const uint16_t KeySize, const char& Key[MAX_KEY_SIZE + 1]) {
    
}

#endif /* B_TREE_HPP */