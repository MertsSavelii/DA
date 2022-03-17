#include <iostream>
#include <cstdint>
#include <iomanip>
#include <vector>

const uint16_t UINT16_MIN = 0;

typedef struct Item {
    uint16_t key;
    uint64_t value;
} Item;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    uint16_t max_key = UINT16_MIN, min_key = UINT16_MAX;
    std::vector <Item> in_arr;
    Item in_item;
    int i = 0;
    while (std::cin >> in_item.key >> in_item.value) {
        in_arr.push_back(in_item);
        min_key = (in_arr[i].key < min_key) ? in_arr[i].key : min_key;
        max_key = (in_arr[i].key > max_key) ? in_arr[i].key : max_key;
        ++i;
    }
    
    uint16_t count_arr_size = max_key - min_key;
    std::vector <uint16_t> count_arr(count_arr_size + 1, 0);

    for(i = 0; i < in_arr.size(); ++i)
        count_arr[in_arr[i].key - min_key] ++;

    for(i = 0; i < count_arr_size; ++i)
        count_arr[i + 1] += count_arr[i];

    std::vector <Item *> out_arr (in_arr.size(), 0);
    // Item* out_arr[in_arr.size()];
    uint16_t count_arr_ind, out_arr_ind;
    for(i = in_arr.size() - 1; i >= 0; --i) {
        count_arr_ind = in_arr[i].key - min_key;
        out_arr_ind = count_arr[count_arr_ind] - 1;
        count_arr[count_arr_ind] --;
        out_arr[out_arr_ind] = &in_arr[i];
    }

    for(i = 0; i < in_arr.size(); ++i)
        std::cout << (*out_arr[i]).key << '\t' << (*out_arr[i]).value << std::endl;

    return 0;
}
