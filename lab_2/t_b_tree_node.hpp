#ifndef T_B_TREE_NODE_HPP
#define T_B_TREE_NODE_HPP

#include "t_b_tree_item.hpp"
#include <vector>
#include <fstream>
#include <iostream>

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
    void Merge(const uint8_t& findIndex) {
        child[findIndex]->data.push_back(data[findIndex]);
        child[findIndex]->data.insert(child[findIndex]->data.end(), child[findIndex + 1]->data.begin(), child[findIndex + 1]->data.end());
        child[findIndex]->child.insert(child[findIndex]->child.end(), child[findIndex + 1]->child.begin(), child[findIndex + 1]->child.end());
        data[findIndex] = nullptr;
        delete data[findIndex];
        data.erase(data.begin() + findIndex);
        for (uint16_t i = 0; i < TREE_DEGREE - 1; ++i) {
            child[findIndex + 1]->data[i] = nullptr;
        }
        for (uint16_t i = 0; i < TREE_DEGREE; ++i) {
            child[findIndex + 1]->child[i] = nullptr;
        }
        delete child[findIndex + 1];
        child.erase(child.begin() + findIndex + 1);
    }
    void EraseFromLeaf(const uint8_t& findIndex) {
        data[findIndex] = nullptr;
        delete(data[findIndex]);
        delete(child[findIndex]);
        data.erase(data.begin() + findIndex);
        child.erase(child.begin() + findIndex);
    }
        // for EraseFromNonLeaf
        TBTreeItem* FindMaxInSubTree() {
            if (child[child.size() - 1] != nullptr) {
                return child[child.size() - 1]->FindMaxInSubTree();
            }
            return data[data.size() - 1];
        }
        TBTreeItem* FindMinInSubTree() {
            if (child[0] != nullptr) {
                return child[0]->FindMinInSubTree();
            }
            return data[0];
        }
        //
    void EraseFromNonLeaf(const uint8_t& findIndex) {
        if (!child[findIndex]->NodeIsMin()) {
            data[findIndex] = child[findIndex]->FindMaxInSubTree();
            child[findIndex]->EraseFromNode(*data[findIndex]);
            return;
        }
        if (!child[findIndex + 1]->NodeIsMin()) {
            data[findIndex] = child[findIndex + 1]->FindMinInSubTree();
            child[findIndex + 1]->EraseFromNode(*data[findIndex]);
            return;
        }
        TBTreeItem elemForErase = *data[findIndex];
        Merge(findIndex);
        child[findIndex]->EraseFromNode(elemForErase);
        return;
    }
        // for FillChild
            void BorrowFromLeftChild(const uint8_t& findIndex) {
                TBTreeNode* fillChild = child[findIndex];
                TBTreeNode* leftChild = child[findIndex - 1];
                fillChild->data.insert(fillChild->data.begin(), data[findIndex - 1]);
                data[findIndex - 1] = leftChild->data[leftChild->data.size() - 1];
                leftChild->data.erase(leftChild->data.end() - 1); 
                fillChild->child.insert(fillChild->child.begin(), leftChild->child[leftChild->child.size() - 1]);
                leftChild->child.erase(leftChild->child.end() - 1);   
            }
            void BorrowFromRightChild(const uint8_t& findIndex) {
                TBTreeNode* fillChild = child[findIndex];
                TBTreeNode* rightChild = child[findIndex + 1];
                fillChild->data.push_back(data[findIndex]);
                data[findIndex] = rightChild->data[0];
                rightChild->data.erase(rightChild->data.begin());
                fillChild->child.push_back(rightChild->child[0]);
                rightChild->child.erase(rightChild->child.begin());
            }
        //
    void FillChild(const uint8_t& findIndex) {
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
    void SaveNode (std::ofstream& toWtiteFile) {
        size_t dataSize = data.size();
        toWtiteFile.write(reinterpret_cast<const char *>(&dataSize), sizeof(size_t));
        for (uint16_t i = 0; i < data.size(); ++i) {
            size_t keySize = data[i]->Key().size();
            toWtiteFile.write(reinterpret_cast<const char *>(&keySize), sizeof(size_t));
            toWtiteFile.write(data[i]->Key().c_str(), keySize);
            uint64_t outValue = data[i]->Value();
            toWtiteFile.write(reinterpret_cast<const char *>(&outValue), sizeof(uint64_t));
        }
        size_t childSize = 0;
        if (NodeIsLeaf()) {
            toWtiteFile.write(reinterpret_cast<const char *>(&childSize), sizeof(size_t));
            toWtiteFile.flush();
            return;
        }
        childSize = child.size();
        toWtiteFile.write(reinterpret_cast<const char *>(&childSize), sizeof(size_t));
        toWtiteFile.flush();
        for (uint16_t i = 0; i < child.size(); ++i) {
            child[i]->SaveNode(toWtiteFile);
        }
    }
    void LoadNode (std::ifstream& ToLoadFile) {
        size_t dataSize = 0;
        ToLoadFile.read(reinterpret_cast<char *>(&dataSize), sizeof(size_t));
        data.resize(dataSize, nullptr);
        if (dataSize == 0) {
            return;
        }
        for (uint16_t i = 0; i < dataSize; ++i) {
            size_t keySize = 0;
            std::string inKey;
            uint64_t inValue = 0;
            ToLoadFile.read(reinterpret_cast<char *>(&keySize), sizeof(size_t));
            inKey.resize(keySize);
            ToLoadFile.read(inKey.data(), keySize);
            ToLoadFile.read(reinterpret_cast<char *>(&inValue), sizeof(uint64_t));
            data[i] = new TBTreeItem(inKey, inValue);
        }
        size_t childSize = 0;
        ToLoadFile.read(reinterpret_cast<char *>(&childSize), sizeof(size_t));
        if (childSize == 0) {
            child.resize(dataSize + 1, nullptr);
            return;
        }
        child.resize(childSize, nullptr);
        for (uint16_t i = 0; i < childSize; ++i) {
            child[i] = new TBTreeNode;
            child[i]->LoadNode(ToLoadFile);
        }
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
    void DeleteNode() {
        for (uint16_t i = 0; i < data.size(); ++i) {
            data[i] = nullptr;
            delete(data[i]);
            data.erase(data.begin() + i);
        }
        if (NodeIsLeaf()) {
            delete this;
            return;
        }
        for (uint16_t i = 0; i < child.size(); ++i) {
            child[i]->DeleteNode();
            child[i] = nullptr;
        }
        delete this;
    }
};

#endif /*T_B_TREE_NODE_HPP*/