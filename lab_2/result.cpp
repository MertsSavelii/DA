#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <fstream>
#include <algorithm>

const uint16_t MAX_KEY_SIZE = 256;

class BTreeItem {
public:
    BTreeItem() {
        Key.resize(MAX_KEY_SIZE + 1);
        Value = 0;
    }
    ~BTreeItem(){}

    std::string Key;
    uint64_t Value;
    friend bool operator<(const BTreeItem& A, const BTreeItem& B) {
        return (A.Key.compare(B.Key) == -1) ? true : false;
    }
};

const uint8_t TREE_DEGREE = 2;

class BTreeNode {
public:
    std::vector<BTreeItem> Data;
    std::vector<BTreeNode*> Child;
    BTreeNode();
    ~BTreeNode();

    BTreeItem* FindKey(std::string Key);

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
    void EraseFromNode(BTreeItem& elem);
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
BTreeItem* BTreeNode::FindKey(std::string Key) {
    for (int i = 0; i < Data.size(); ++i) {
        if (Key == Data[i].Key) {
            return &Data[i];
        }
    }
    if (Key < Data[0].Key && Child[0] != nullptr) {
        return Child[0]->FindKey(Key);
    }
    if (Key > Data[Data.size() - 1].Key && Child[Child.size() - 1] != nullptr) {
        return Child[Child.size() - 1]->FindKey(Key);
    }
    for (int i = 1; i < Data.size(); ++i) {
        if (Key > Data[i - 1].Key && Key < Data[i].Key && Child[i] != nullptr) {
            return Child[i]->FindKey(Key);
        }
    }
    return nullptr;
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
        } 
        index = BinarySearch(Data, elem);
        Child[index]->InsertToNode(elem);
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
    delete this;
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
    int16_t left = -1;
    uint8_t right = Data.size();
    while (left + 1 < right) {
        uint8_t mid = (left + right) / 2;
        if (Data[mid].Key < elem.Key) {
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
            index = BinarySearch(Data, elem);
            Child[index]->EraseFromNode(elem);
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

void BTreeNode::BorrowFromLeftChild(uint8_t index) {
    BTreeNode* curr = Child[index];
    BTreeNode* leftBro = Child[index - 1];
    curr->Data.insert(curr->Data.begin(), Data[index - 1]);
    Data.erase(Data.begin() + index - 1);
    Data.insert(Data.begin() + index - 1, leftBro->Data[leftBro->Data.size() - 1]);
    leftBro->Data.erase(leftBro->Data.end() - 1);
    
    if (!curr->NodeIsLeaf()) {
        curr->Child.insert(curr->Child.begin(), leftBro->Child[leftBro->Child.size() - 1]);
        leftBro->Child.erase(leftBro->Child.end() - 1);
    }
}

void BTreeNode::BorrowFromRightChild(uint8_t index) {
    BTreeNode* curr = Child[index];
    BTreeNode* rightBro = Child[index + 1];
    curr->Data.insert(curr->Data.end(), Data[index]);
    Data.erase(Data.begin() + index);
    Data.insert(Data.begin() + index, rightBro->Data[0]);
    rightBro->Data.erase(rightBro->Data.begin());

    if (!curr->NodeIsLeaf()) {
        curr->Child.insert(curr->Child.end(), rightBro->Child[0]);
        rightBro->Child.erase(rightBro->Child.begin());
    }
}

class BTree {
public:
    BTree();
    ~BTree();
    BTreeItem* Search(std::string Key);
    void Insert(BTreeItem& elem);
    void Erase(BTreeItem& elem);
    void Save(std::ofstream ToWtiteFile);
    void Load(std::ofstream ToLoadFile);

private:
    BTreeNode* Root;
};

BTree::BTree() {
    Root = nullptr;
}

BTree::~BTree() {
    delete Root;
}

BTreeItem* BTree::Search(std::string Key) {
    if (Root == nullptr) {
        return nullptr;
    }
    return Root->FindKey(Key);
}

void BTree::Insert(BTreeItem& elem) {
    if(Root == nullptr) {
        Root = new BTreeNode;
        Root->Data[0] = elem;
        Root->Child[0] = nullptr;
        Root->Child[1] = nullptr;
    } else {
        if(Root->NodeIsFull()) {
            Root = Root->SplitNode();
        }
        Root->InsertToNode(elem);
    }
}

void BTree::Erase(BTreeItem& elem) {
    if (Root == nullptr) {
        return;
    } else {
        Root->EraseFromNode(elem);
    }
}

void BTree::Save(std::ofstream ToWtiteFile) {

}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string Command;
    BTree tree;
    while (std::cin >> Command) {
        if (Command == "+") {
            std::string KeyWord;
            uint64_t Value;
            BTreeItem ToInsertItem;
            std::cin >> KeyWord >> Value;
            std::transform(KeyWord.begin(), KeyWord.end(), KeyWord.begin(), tolower);
            ToInsertItem.Key = KeyWord;
            ToInsertItem.Value = Value;
            if (tree.Search(KeyWord) != nullptr) {
                printf("Exist\n");
            } else {
                tree.Insert(ToInsertItem);
                printf("OK\n");
            }
        } else if (Command == "-") {
            std::string KeyWord;
            std::cin >> KeyWord;
            std::transform(KeyWord.begin(), KeyWord.end(), KeyWord.begin(), tolower);
            BTreeItem ToEraseItem;
            ToEraseItem.Key = KeyWord;
            if (tree.Search(KeyWord) == nullptr){
                printf("NoSuchWord\n");
            } else {
                tree.Erase(ToEraseItem);
                printf("OK\n");
            }
        } else if (Command == "!") {
            std::string Path;
            std::cin >> Command >> Path;
            if (Command == "Save") {
                std::ofstream ToWriteFile(Path, std::ios::trunc | std::ios::binary);
               // tree.Save(ToWriteFile);
                printf("OK\n");
            } else if (Command == "Load") {

            }
        } else {
            std::transform(Command.begin(), Command.end(), Command.begin(), tolower);
            BTreeItem* FoundItem = tree.Search(Command);
            if (FoundItem != nullptr) {
                printf("OK: %llu\n", FoundItem->Value);
            } else {
                printf("NoSuchWord\n");
            }
        }
    }
    return 0;
}