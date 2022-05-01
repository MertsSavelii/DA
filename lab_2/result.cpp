#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <stdint.h>
#include <string>

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
    friend bool operator<(const BTreeItem& A, const BTreeItem& B);
};

bool operator<(const BTreeItem& A, const BTreeItem& B) {
    return (A.Key.compare(B.Key) == -1) ? true : false;
}

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
    if (Key < Data[0].Key && Child[0] != nullptr) {
        return Child[0]->FindKey(Key);
    }
    if (Key > Data[Data.size() - 1].Key && Child[Child.size() - 1] != nullptr) {
        return Child[Child.size() - 1]->FindKey(Key);
    }
    for (int i = 0; i < Data.size() - 1; ++i) {
        if (Key > Data[i].Key && Key < Data[i + 1].Key && Child[i + 1] != nullptr) {
            return Child[i + 1]->FindKey(Key);
        }
    }
    return false;
}

// InsertToNode
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
    delete this;// надо удалить в береве хотя хз
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

    if (Child[index] == nullptr) {
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

class BTree {
public:
    BTree();
    ~BTree();
    bool Search(std::string Key);
    void Insert(BTreeItem& elem);
    void Erase(BTreeItem& elem);
    void Save();
    void Load();


    BTreeNode* Root;
};

BTree::BTree() {
    Root = nullptr;
}

BTree::~BTree() {
    delete Root;
}

bool BTree::Search(std::string Key) {
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
    
}

int main() {
    BTree tree;
    BTreeItem item;
    item.Key = "d";
    item.Value = 50;
    tree.Insert(item);
    if (tree.Search("d")) {
        printf("d passed");
    }
    item.Key = "c";
    item.Value = 30;
    tree.Insert(item);
    if (tree.Search("c")) {
        printf("c passed");
    }
    item.Key = "e";
    item.Value = 40;
    tree.Insert(item);
    if (tree.Search("e")) {
        printf("e passed");
    }
    item.Key = "b";
    item.Value = 20;
    tree.Insert(item);
    if (tree.Search("b")) {
        printf("b passed");
    }
    item.Key = "f";
    item.Value = 45;
    tree.Insert(item);
    if (tree.Search("f")) {
        printf("f passed");
    }
    item.Key = "g";
    item.Value = 10;
    tree.Insert(item);
    if (tree.Search("g")) {
        printf("g passed");
    }
    item.Key = "a";
    item.Value = 60;
    tree.Insert(item);
    if (tree.Search("a")) {
        printf("a passed");
    }
    item.Key = "h";
    item.Value = 5;
    tree.Insert(item);
    if (tree.Search("h")) {
        printf("h passed");
    }
    return 0;
}