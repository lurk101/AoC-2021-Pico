#define title "--- Day 2: Dive! ---"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "pico/stdlib.h"

const vector<string> lines = {
#include "day2.txt"
};

int part1() {
    int x = 0, z = 0;
    for (auto& l : lines) {
        int distance;
        switch (l[0]) {
        case 'f':
            distance = stoi(l.substr(8));
            x += distance;
            break;
        case 'd':
            distance = stoi(l.substr(5));
            z += distance;
            break;
        default:
            distance = stoi(l.substr(3));
            z -= distance;
            break;
        }
    }
    return x * z;
}

int part2() {
    int x = 0, z = 0, aim = 0;
    for (auto& l : lines) {
        int distance;
        switch (l[0]) {
        case 'f':
            distance = stoi(l.substr(8));
            x += distance;
            z += aim * distance;
            break;
        case 'd':
            distance = stoi(l.substr(5));
            aim += distance;
            break;
        default:
            distance = stoi(l.substr(3));
            aim -= distance;
            break;
        }
    }
    return x * z;
}

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl
         << "Part 1 - " << part1() << endl
         << "Part 2 - " << part2() << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
