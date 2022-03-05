#include <iostream>
#include <cstdint>
#include <iomanip>
#include <vector>

using namespace std;

const uint16_t UINT16_MIN = 0;

typedef struct Item {
    uint16_t key;
    uint64_t value;
} Item;

int main() {
    vector<Item> in_arr;
    //Enter array
    Item in_item;
    while (cin >> in_item.key >> in_item.value) {
        in_arr.push_back(in_item);
    }
    cout<<endl;
    //Find min max
    uint16_t max_key = UINT16_MIN, min_key = UINT16_MAX;
    for(int i = 0; i < in_arr.size(); i++) {
        min_key = (in_arr[i].key < min_key) ? in_arr[i].key : min_key;
        max_key = (in_arr[i].key > max_key) ? in_arr[i].key : max_key;
    }
    // cout<<min_key<<' '<< max_key<<endl;
    
    uint16_t count_arr_size = max_key - min_key + 1;
    uint16_t count_arr[count_arr_size] = {0};

    for(int i = 0; i < in_arr.size(); i++) {
        count_arr[in_arr[i].key - min_key] ++;
    }


    // for(int i = 0; i < count_arr_size; i++) {
    //     cout << count_arr[i] << ' ';
    // }
    // cout << endl;

    for(int i = 0; i < count_arr_size - 1; i++) {
        count_arr[i + 1] += count_arr[i];
    }

    // for(int i = 0; i < count_arr_size; i++) {
    //     cout << count_arr[i] << ' ';
    // }
    // cout << endl;

    vector<Item> out_arr(in_arr.size());
    uint16_t count_arr_ind, out_arr_ind;
    for(int i = in_arr.size() - 1; i >= 0; i--) {
        count_arr_ind = in_arr[i].key - min_key;
        out_arr_ind = count_arr[count_arr_ind] - 1;
        out_arr[out_arr_ind] = in_arr[i];
        count_arr[count_arr_ind] --;
    }

    for(int i = 0; i < out_arr.size(); i++) {
        cout << out_arr[i].key << ' ' << out_arr[i].value << endl;
    }
    
    return 0;
}
