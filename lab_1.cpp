#include <iostream>
#include <cstdint>
#include <iomanip>
#include <vector>
#include <chrono>

using namespace std;

typedef struct Item {
    uint_fast16_t key;
    uint_fast64_t value;
} Item;

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    vector<Item> in_arr;
    Item in_item;
    while (cin >> in_item.key >> in_item.value)
        in_arr.push_back(move(in_item));

    vector<Item*> out_arr(in_arr.size());
    vector<uint_fast16_t> count(UINT16_MAX + 1, 0);

    chrono::system_clock::time_point start = chrono::system_clock::now();
    for (int i = 0; i < in_arr.size(); ++i)
        ++count[in_arr[i].key];

    for (int i = 1; i < count.size(); ++i)
        count[i] +=  count[i - 1];

    for (int i = in_arr.size() - 1; i >= 0; --i)
        out_arr[--count[in_arr[i].key]] = &in_arr[i];
    chrono::system_clock::time_point end = chrono::system_clock::now();
    for (int i = 0; i < in_arr.size(); i++)
        cout << out_arr[i]->key << '\t' << out_arr[i]->value << '\n';
    cout << duration_cast<chrono::milliseconds>(end - start).count() << "\n";
}
