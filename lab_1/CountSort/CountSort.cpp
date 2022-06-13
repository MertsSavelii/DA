#include <iostream>
#include <vector>

typedef struct Item {
    unsigned short int key;
    unsigned long int value;
} Item;

std::vector<Item> CounSort (std::vector<Item>& inArr) {
    std::vector<Item*> out_arr(inArr.size());
    std::vector<unsigned short int> count(UINT16_MAX + 1, 0);
    for (int i = 0; i < inArr.size(); ++i) {
        ++count[inArr[i].key];
    }
    for (int i = 1; i < count.size(); ++i) {
        count[i] +=  count[i - 1];
    }
    for (int i = inArr.size() - 1; i >= 0; --i) {
        out_arr[--count[inArr[i].key]] = &inArr[i];
    }
}


int main(void) {
    std::vector<Item> inArr;
    Item inItem;
    while (std::cin >> inItem.key >> inItem.value) {
        inArr.push_back(std::move(inItem));
    }
    std::vector<Item> out_arr = CounSort(inArr);
    for (int i = 0; i < inArr.size(); i++) {
        std::cout << out_arr[i].key << '\t' << out_arr[i].value << '\n';
    }  
}