#ifndef B_TREE_HPP
#define B_TREE_HPP

#include "b_tree_node.hpp"

const uint16_t TREE_DEGREE = 12;

class BTree {
public:
    BTree();
    ~BTree();
    void Insert();
    void Erase();
    BTreeNode* Search();
    void Save();
    void Load();

private:
    BTreeNode* root;

    //for Insert
    bool RootIsFull();
    void SplitRoot();
    bool CanInsertInRoot();
    void InsertToRoot();
    void SearchChildToInsert();
    bool ChildIsFull();
    void SplitChild();
    void InsertToChild(); 
};

bool BTree::RootIsFull() {
    return root->Data.size() == 2 * TREE_DEGREE - 1 ? true : false;
}

void BTree::SplitRoot() {
    BTreeNode* newRoot;
    
}

void BTree::Insert() {
    if(RootIsFull()) {
        SplitRoot();
    } else {
        if(CanInsertInRoot()) {
            InsertToRoot();
        } else {
            SearchChildToInsert();
            if(ChildIsFull()) {
                SplitChild();
            } else {
                InsertToChild();
            }
        }
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