#include <stdint.h>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <fstream>
#include <algorithm>

const uint16_t MAX_KEY_SIZE = 256;

class TBTreeItem {
public:
    TBTreeItem() {
        Key.resize(MAX_KEY_SIZE + 1);
        Value = 0;
    }
    ~TBTreeItem(){}

    std::string Key;
    uint64_t Value;
    friend bool operator<(const TBTreeItem& A, const TBTreeItem& B) {
        return (A.Key.compare(B.Key) == -1) ? true : false;
    }
};

const uint8_t TREE_DEGREE = 2;

class TBTreeNode {
public:
    std::vector<TBTreeItem> Data;
    std::vector<TBTreeNode*> Child;
    TBTreeNode();
    ~TBTreeNode();

    TBTreeItem* FindKey(std::string Key);

    // for InsertToNode
    TBTreeNode* SplitNode();
    bool NodeIsLeaf();
    bool NodeIsFull();
    uint8_t BinarySearch(std::vector<TBTreeItem>& Data, TBTreeItem& elem);
    void SplitChild(uint8_t ChildIndex);
    // InsertToNode
    void InsertToNode(TBTreeItem& elem);

    // for EraseFormNode
    bool ElemInNode(uint8_t index, TBTreeItem& elem);
    void EraseFromLeaf(uint8_t);
    void EraseFromNonLeaf(uint8_t);
    TBTreeItem FindMaxInSubTree();
    TBTreeItem FindMinInSubTree();
    bool NodeIsMin();
    void Merge(uint8_t index);
    void FillChild(uint8_t index);
    void BorrowFromLeftChild(uint8_t index);
    void BorrowFromRightChild(uint8_t index);
    // EraseFormNode
    void EraseFromNode(TBTreeItem& elem);
};

TBTreeNode::TBTreeNode() {
    Data.resize(1);
    Child.resize(2);
}

TBTreeNode::~TBTreeNode() {
    for (uint8_t i = 0; i < Child.size(); ++i)
        delete Child[i];
}

// FindNode
TBTreeItem* TBTreeNode::FindKey(std::string Key) {
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
void TBTreeNode::InsertToNode(TBTreeItem& elem) {
    uint8_t index = BinarySearch(Data, elem);

    if (NodeIsLeaf()) {
        Data.insert(Data.begin() + index, elem);
        Child.insert(Child.begin() + index, nullptr);
    } else {
        if (Child[index] == nullptr) {
            Child[index] = new TBTreeNode;
        }
        if (Child[index]->NodeIsFull()) {
            SplitChild(index);
            index = BinarySearch(Data, elem);
        } 
        Child[index]->InsertToNode(elem);
    }
}

TBTreeNode* TBTreeNode::SplitNode() {
    TBTreeNode* newNode = new TBTreeNode;
    newNode->Data[0] = Data[TREE_DEGREE - 1];
    newNode->Child[0] = new TBTreeNode;
    newNode->Child[1] = new TBTreeNode;
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

bool TBTreeNode::NodeIsLeaf() {
    for (int i = 0; i < Child.size(); ++i)
        if (Child[i] != nullptr)
            return false;
    return true;
}

bool TBTreeNode::NodeIsFull() {
    return (Data.size() == 2 * TREE_DEGREE - 1) ? true : false;
}

uint8_t TBTreeNode::BinarySearch(std::vector<TBTreeItem>& Data, TBTreeItem& elem) {
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

void TBTreeNode::SplitChild(uint8_t ChildIndex) {
    TBTreeNode* SplitNode = Child[ChildIndex]->SplitNode();
    Data.insert(Data.begin() + ChildIndex, SplitNode->Data[0]);
    Child[ChildIndex] = SplitNode->Child[1];
    Child.insert(Child.begin() + ChildIndex, SplitNode->Child[0]);
    SplitNode->Child[0] = nullptr;
    SplitNode->Child[1] = nullptr;
    delete SplitNode;
}

// EraseFromNode
void TBTreeNode::EraseFromNode(TBTreeItem& elem) {
    uint8_t index = BinarySearch(Data, elem);
    if (ElemInNode(index, elem)) {
        if (NodeIsLeaf()) {
            EraseFromLeaf(index);
        } else {
            EraseFromNonLeaf(index);
        }
    } else if (Child[index] != nullptr) {
        if (Child[index]->NodeIsMin()) {
            FillChild(index); 
            index = BinarySearch(Data, elem);
        } else {
            Child[index]->EraseFromNode(elem);
        }
    }
}

bool TBTreeNode::ElemInNode(uint8_t index, TBTreeItem& elem) {
    return (index < Data.size() && Data[index].Key == elem.Key) ? true : false;
}

void TBTreeNode::EraseFromLeaf(uint8_t index) {
    Data.erase(Data.begin() + index);
    Child.erase(Child.begin() + index);
}

void TBTreeNode::EraseFromNonLeaf(uint8_t index) {
    if (!Child[index]->NodeIsMin()) {
        TBTreeItem leftMax = Child[index]->FindMaxInSubTree();
        EraseFromNode(leftMax);
        Data[index] = leftMax;
    } else if (!Child[index+ 1]->NodeIsMin()) {
        TBTreeItem rightMin = Child[index + 1]->FindMinInSubTree();
        EraseFromNode(rightMin);
        Data[index] = rightMin;
    } else {
        Merge(index);
        Child[index]->EraseFromNode(Data[index]);
    }
}

TBTreeItem TBTreeNode::FindMaxInSubTree() {
    if (Child[Data.size()] != nullptr) {
        return Child[Data.size()]->FindMaxInSubTree();
    } else {
        return Data[Data.size() - 1];
    }
}

TBTreeItem TBTreeNode::FindMinInSubTree() {
    if (Child[0] != nullptr) {
        return Child[0]->FindMaxInSubTree();
    } else {
        return Data[0];
    }
}

bool TBTreeNode::NodeIsMin() {
    return (Data.size() < TREE_DEGREE) ? true : false;
}

void TBTreeNode::Merge(uint8_t index) {
    Child[index]->Data.push_back(Data[index]);
    Child[index]->Data.insert(Child[index]->Data.end(), Child[index + 1]->Data.begin(), Child[index + 1]->Data.end());
    if (!Child[index]->NodeIsLeaf()) {
        Child[index]->Child.insert(Child[index]->Child.end(), Child[index + 1]->Child.begin(), Child[index + 1]->Child.end());
    }
    Data.erase(Data.begin() + index);
    for (int i = 0; i < Child[index + 1]->Child.size(); ++i) {
        Child[index + 1]->Child[i] = nullptr;
    }
    delete(Child[index + 1]);
    Child.erase(Child.begin() + index + 1);
}

void TBTreeNode::FillChild(uint8_t index) {
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

void TBTreeNode::BorrowFromLeftChild(uint8_t index) {
    TBTreeNode* curr = Child[index];
    TBTreeNode* leftBro = Child[index - 1];
    curr->Data.insert(curr->Data.begin(), Data[index - 1]);
    Data.erase(Data.begin() + index - 1);
    Data.insert(Data.begin() + index - 1, leftBro->Data[leftBro->Data.size() - 1]);
    leftBro->Data.erase(leftBro->Data.end() - 1);
    
    if (!curr->NodeIsLeaf()) {
        curr->Child.insert(curr->Child.begin(), leftBro->Child[leftBro->Child.size() - 1]);
        leftBro->Child.erase(leftBro->Child.end() - 1);
    }
}

void TBTreeNode::BorrowFromRightChild(uint8_t index) {
    TBTreeNode* curr = Child[index];
    TBTreeNode* rightBro = Child[index + 1];
    curr->Data.insert(curr->Data.end(), Data[index]);
    Data.erase(Data.begin() + index);
    Data.insert(Data.begin() + index, rightBro->Data[0]);
    rightBro->Data.erase(rightBro->Data.begin());

    if (!curr->NodeIsLeaf()) {
        curr->Child.insert(curr->Child.end(), rightBro->Child[0]);
        rightBro->Child.erase(rightBro->Child.begin());
    }
}

class TBTree {
public:
    TBTree();
    ~TBTree();
    TBTreeItem* Search(std::string Key);
    void Insert(TBTreeItem& elem);
    void Erase(TBTreeItem& elem);
    void Save(std::ofstream ToWtiteFile);
    void Load(std::ofstream ToLoadFile);

private:
    TBTreeNode* Root;
};

TBTree::TBTree() {
    Root = nullptr;
}

TBTree::~TBTree() {
    delete Root;
}

TBTreeItem* TBTree::Search(std::string Key) {
    if (Root == nullptr) {
        return nullptr;
    }
    return Root->FindKey(Key);
}

void TBTree::Insert(TBTreeItem& elem) {
    if(Root == nullptr) {
        Root = new TBTreeNode;
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

void TBTree::Erase(TBTreeItem& elem) {
    if (Root == nullptr) {
        return;
    }
    Root->EraseFromNode(elem);
    if (Root->Data.size() == 0) {
        if (Root->NodeIsLeaf()) {
            Root = nullptr;
        } else {
            //Root = Root->Child[0];
        }
    }
}

void TBTree::Save(std::ofstream ToWtiteFile) {

}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string Command;
    TBTree tree;
    std::ifstream in("test.txt");
    std::ofstream out("result.txt");
    while (in >> Command) {
        if (Command == "+") {
            std::string KeyWord;
            uint64_t Value;
            TBTreeItem ToInsertItem;
            in >> KeyWord >> Value;
            std::transform(KeyWord.begin(), KeyWord.end(), KeyWord.begin(), tolower);
            ToInsertItem.Key = KeyWord;
            ToInsertItem.Value = Value;
            if (tree.Search(KeyWord) != nullptr) {
                out << "Exist\n";
                //printf("Exist\n");
            } else {
                tree.Insert(ToInsertItem);
                out << "OK\n";
                //printf("OK\n");
            }
        } else if (Command == "-") {
            std::string KeyWord;
            in >> KeyWord;
            std::transform(KeyWord.begin(), KeyWord.end(), KeyWord.begin(), tolower);
            TBTreeItem ToEraseItem;
            ToEraseItem.Key = KeyWord;
            if (tree.Search(KeyWord) == nullptr){
                out << "NoSuchWord\n";
                //printf("NoSuchWord\n");
            } else {
                tree.Erase(ToEraseItem);
                out << "OK\n";
                //printf("OK\n");
            }
        } else if (Command == "!") {
            std::string Path;
            in >> Command >> Path;
            if (Command == "Save") {
                std::ofstream ToWriteFile(Path, std::ios::trunc | std::ios::binary);
                //tree.Save(ToWriteFile);
                out << "OK\n";
                //printf("OK\n");
            } else if (Command == "Load") {

            }
        } else {
            std::transform(Command.begin(), Command.end(), Command.begin(), tolower);
            TBTreeItem* FoundItem = tree.Search(Command);
            if (FoundItem != nullptr) {
                out << "OK: " << FoundItem->Value << "\n";
                //printf("OK: %llu\n", FoundItem->Value);
            } else {
                out << "NoSuchWord\n";
                //printf("NoSuchWord\n");
            }
        }
    }
    return 0;
}