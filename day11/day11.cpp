#define title "--- Day 11: Dumbo Octopus ---"
#include <bitset>
#include <iostream>
#include <list>
#include <sstream>
using namespace std;

#include "pico/stdlib.h"

uint8_t dumbo[100];
bitset<100> flashed;
list<short> adjacent[100];

short inline coord(short y, short x) { return y * 10 + x; }

void generate_adjacents() {
    for (short y = 0; y < 10; y++)
        for (short x = 0; x < 10; x++)
            for (short y2 = y - 1; y2 <= y + 1; y2++)
                for (short x2 = x - 1; x2 <= x + 1; x2++)
                    if (y2 >= 0 && y2 < 10 && x2 >= 0 && x2 < 10 && (y != y2 || x != x2))
                        adjacent[coord(y, x)].push_back(coord(y2, x2));
}

const string lines[] = {
#include "day11.txt"
};

void load(void) {
    int i = 0;
    for (auto& line : lines)
        for (auto c : line)
            dumbo[i++] = c - '0';
}

unsigned flash(short i) {
    unsigned sum = 0;
    if (!flashed[i]) {
        dumbo[i]++;
        if (dumbo[i] == 10) {
            dumbo[i] = 0;
            flashed[i] = true;
            sum++;
            for (auto j : adjacent[i])
                sum += flash(j);
        }
    }
    return sum;
}

unsigned step(void) {
    flashed.reset();
    unsigned sum = 0;
    for (short i = 0; i < 100; i++)
        sum += flash(i);
    return sum;
}

unsigned part1(void) {
    load();
    unsigned sum = 0;
    for (short s = 1; s <= 100; s++)
        sum += step();
    return sum;
}

unsigned part2(void) {
    load();
    for (short s = 1;; s++)
        if (step() == 100)
            return s;
    return 0;
}

int main() {
    auto start = time_us_32();
    stdio_init_all();
    generate_adjacents();
    cout << title << endl
         << "Part 1 - " << part1() << endl
         << "Part 2 - " << part2() << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
