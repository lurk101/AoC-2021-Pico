#define title "--- Day 24: Arithmetic Logic Unit ---"
#include <cstdint>
#include <iostream>
#include <sstream>
using namespace std;

#include "pico/stdlib.h"

#define N 14

struct block {
    bool is_pop;
    int8_t div, off, mod;
} blocks[N];

int64_t part(int part_n) {
    typedef pair<int8_t, int8_t> item;
    item stack[N];
    unsigned stack_idx = 0;
    int8_t w[N];
    for (auto& i : w)
        i = 0;
    for (int i = 0; i < 14; i++) {
        auto& block(blocks[i]);
        if (!block.is_pop)
            stack[stack_idx++] = {i, block.mod};
        else {
            auto& it(stack[--stack_idx]);
            int8_t d = block.off + it.second;
            w[it.first] = part_n == 2 ? max(1 - d, 1) : min(9 - d, 9);
            w[i] = w[it.first] + d;
        }
    }
    uint64_t n = 0;
    for (auto& digit : w)
        n = n * 10 + digit;
    return n;
}

const string lines[] = {
#include "day24.txt"
};

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl;
    unsigned block_id = 0, line_id = 0;
    for (auto& l : lines) {
        switch (line_id) {
        case 4:
            blocks[block_id].div = stol(l.substr(6));
            blocks[block_id].is_pop = blocks[block_id].div == 26;
            break;
        case 5:
            blocks[block_id].off = stol(l.substr(6));
            break;
        case 15:
            blocks[block_id].mod = stol(l.substr(6));
        }
        line_id = (line_id + 1) % 18;
        if (line_id == 0)
            block_id++;
    }
    cout << "Part 1  - " << part(1) << endl
         << "Part 2  - " << part(2) << endl
         << "Elapsed - " << (time_us_32() - start) / 1e3 << " ms." << endl;
}
