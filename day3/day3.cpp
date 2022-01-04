#define title "--- Day 3: Binary Diagnostic ---"
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;

#include "pico/stdlib.h"

list<uint16_t> codes, save_codes;

int part1(void) {
    uint16_t gamma = 0;
    for (int b = 11; b >= 0; b--) {
        gamma <<= 1;
        int count = 0;
        uint16_t mask = 1 << b;
        for (auto& code : codes)
            if (code & mask)
                count++;
        if (count > (codes.size() - count))
            gamma |= 1;
    }
    return gamma * (gamma ^ (uint16_t(-1) >> 4));
}

const string lines[] = {
#include "day3.txt"
};

int rating(bool one) {
    codes.clear();
    for (auto& s : lines)
        codes.push_back(strtoul(s.c_str(), NULL, 2));
    for (int b = 11; b >= 0; b--) {
        list<uint16_t> pruned;
        int count = 0;
        uint16_t mask = 1 << b;
        for (auto& code : codes)
            if (code & mask)
                count++;
        int match = (count >= (codes.size() - count)) ? mask : 0;
        for (auto& code : codes)
            if (((code & mask) == match) ^ one)
                pruned.push_back(code);
        if (pruned.size() == 1)
            return pruned.front();
        codes = move(pruned);
    }
    return 0;
}

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl;
    for (auto& s : lines)
        codes.push_back(strtoul(s.c_str(), NULL, 2));
    cout << "Part 1 - " << part1() << endl
         << "Part 2 - " << rating(true) * rating(false) << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
