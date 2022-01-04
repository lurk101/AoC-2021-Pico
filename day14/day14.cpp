#define title "--- Day 14: Extended Polymerization ---"
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <unordered_map>
using namespace std;

#include "pico/stdlib.h"

unordered_map<string, char> pairs;
unordered_map<string, uint64_t> pair_counts;
unordered_map<char, uint64_t> elements;

const string lines[] = {
#include "day14.txt"
};

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl;
    string poly;
    poly = lines[0];
    for (int i = 2; i < sizeof(lines) / sizeof(lines[0]); i++) {
        stringstream ss(lines[i]);
        string pare, elem;
        ss >> pare;
        ss >> elem;
        ss >> elem;
        pairs[pare] = elem[0];
        pair_counts[pare] = 0;
        elements[pare[0]] = 0;
        elements[pare[1]] = 0;
    }
    for (auto& c : poly)
        elements[c]++;
    for (auto pc : pair_counts)
        pc.second = 0;
    for (unsigned i = 0; i < poly.length() - 1; i++) {
        string pare = poly.substr(i, 2);
        pair_counts[pare]++;
    }
    for (unsigned step = 1; step <= 40; step++) {
        unordered_map<string, uint64_t> new_counts;
        for (auto pc : pair_counts)
            new_counts.insert(pair<string, unsigned>(pc.first, 0));
        for (auto pc : pair_counts) {
            string pare = pc.first;
            uint64_t cnt = pc.second;
            if (cnt) {
                // the problem sticks to x*x pairs for x elements, all combinations are viable
                elements[pairs[pare]] += cnt;
                char middle_c = pairs[pare];
                string new_pair(string(1, pc.first[0]) + middle_c);
                new_counts[new_pair] += cnt;
                new_pair = string(1, middle_c) + pc.first[1];
                new_counts[new_pair] += cnt;
            }
        }
        pair_counts = new_counts;
        if ((step == 10) || (step == 40)) {
            list<uint64_t> cnt;
            for (auto pe : elements)
                cnt.push_back(pe.second);
            cnt.sort();
            if (step == 10)
                cout << "Part 1 - ";
            else
                cout << "Part 2 - ";
            cout << cnt.back() - cnt.front() << endl;
        }
    }
    cout << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
