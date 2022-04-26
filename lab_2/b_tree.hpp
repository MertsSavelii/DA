#ifndef B_TREE_HPP
#define B_TREE_HPP

#include "b_tree_node.hpp"

const uint16_t TREE_DEGREE = 12;

class BTree {
public:
    BTree();
    ~BTree();
    void Insert(BTreeItem* elem);
    void Erase();
    BTreeNode* Search();
    void Save();
    void Load();

private:
    BTreeNode* Root;

    //for Insert
    bool RootIsFull();
    void SplitRoot();
    void InsertToRoot();
    void InsertToChild(BTreeItem* elem); 
};

bool BTree::RootIsFull() {
    return Root->Data.size() == 2 * TREE_DEGREE - 1 ? true : false;
}

void BTree::SplitRoot() {
    Root = Root->SplitNode();
}

void BTree::InsertToRoot() {
    // нужен метод ноды бинпоиска индекса куда вставлять

}


void BTree::Insert(BTreeItem* elem) {
    if(Root == nullptr) {
        Root = new BTreeNode;
        Root->Data[0] = elem;
    }
    if(RootIsFull()) {
        SplitRoot();
    }
    if(Root->Leaf) {
        InsertToRoot();
    } else {
        InsertToChild(elem);
    }
}

void BTree::Erase() {

}

BTreeNode* BTree::Search() {
    // if (Key == DataI.key) return DataI;
    // if (Key < Data0.key) return Find(Child0, Key);
    // if (Key > DataLast.key) return Find(ChildLast, Key);
    // if (Key > DataI.key && Key < DataI+1.key) return Find(ChildI+1, Key);
}

#endif /* B_TREE_HPP*/