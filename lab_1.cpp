#include <iostream>
#include <cstdint>
#include <iomanip>
#include <vector>

typedef struct Item {
    uint16_t key;
    uint64_t value;
} Item;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::vector <Item> in_arr;
    Item in_item;
    while (std::cin >> in_item.key >> in_item.value)
        in_arr.push_back(in_item);
    
    std::vector <uint32_t> count_arr(UINT16_MAX + 1, 0);

    for(int i = 0; i < in_arr.size(); ++i)
        count_arr[in_arr[i].key] ++;

    for(int i = 1; i < UINT16_MAX + 1; ++i)
        count_arr[i] += count_arr[i - 1];

    std::vector <Item*> out_arr (in_arr.size(), 0);
    for(int i = in_arr.size() - 1; i >= 0; --i)
        out_arr[--count_arr[in_arr[i].key]] = &in_arr[i];

    for(int i = 0; i < in_arr.size(); ++i)
        std::cout << out_arr[i]->key << '\t' << out_arr[i]->value << std::endl;

    return 0;
}
