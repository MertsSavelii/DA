#include "b_tree.hpp"

int main() {
    BTree tree;
    BTreeItem item;
    item.Key = "d";
    item.Value = 50;
    tree.Insert(item);
    //tree.Erase(item);
    if (tree.Search("d")) {
        printf("d passed\n");
    }
    item.Key = "c";
    item.Value = 30;
    tree.Insert(item);
    //tree.Erase(item);
    if (tree.Search("c")) {
        printf("c passed\n");
    }
    item.Key = "e";
    item.Value = 40;
    tree.Insert(item);
    //tree.Erase(item);
    if (tree.Search("e")) {
        printf("e passed\n");
    }
    item.Key = "b";
    item.Value = 20;
    tree.Insert(item);
    //tree.Erase(item);
    if (tree.Search("b")) {
        printf("b passed\n");
    }
    item.Key = "f";
    item.Value = 45;
    tree.Insert(item);
    //tree.Erase(item);
    if (tree.Search("f")) {
        printf("f passed\n");
    }
    item.Key = "g";
    item.Value = 10;
    tree.Insert(item);
    //tree.Erase(item);
    if (tree.Search("g")) {
        printf("g passed\n");
    }
    item.Key = "a";
    item.Value = 60;
    tree.Insert(item);
    //tree.Erase(item);
    if (tree.Search("a")) {
        printf("a passed\n");
    }
    item.Key = "h";
    item.Value = 5;
    tree.Insert(item);
    tree.Erase(item);
    if (tree.Search("h")) {
        printf("h passed\n");
    }
    return 0;
}