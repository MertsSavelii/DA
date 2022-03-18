#include <iostream>
#include <cstdint>
#include <iomanip>
#include <vector>

using namespace std;

typedef struct Item {
    uint_fast16_t key; //0-UINT16_MAX
    uint_fast64_t value; //0-UINT63_MAX
} Item;

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    vector<Item> in_arr;
    Item in_item;
    while (cin >> in_item.key >> in_item.value)
        in_arr.push_back(move(in_item));

    vector<uint_fast16_t> count(UINT16_MAX + 1, 0);
    for (int i = 0; i < in_arr.size(); ++i)
        ++count[in_arr[i].key];

    for (int i = 1; i < count.size(); ++i)
        count[i] +=  count[i - 1];

    vector<Item*> out_arr(in_arr.size());
    for (int i = in_arr.size() - 1; i >= 0; --i)
        out_arr[--count[in_arr[i].key]] = &in_arr[i];

    for (int i = 0; i < in_arr.size(); i++)
        cout << out_arr[i]->key << '\t' << out_arr[i]->value << '\n';
}
