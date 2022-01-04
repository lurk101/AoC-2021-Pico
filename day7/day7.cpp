#define title "--- Day 7: The Treachery of Whales ---"
#include <bits/stdc++.h>
#include <iostream>
#include <list>
#include <sstream>
using namespace std;

#include "pico/stdlib.h"

list<int> crabs;
int part_n;

int inline fuel_cost(int n) {
    if ((part_n == 1) || (n < 2))
        return n;
    return n * (n + 1) / 2;
}

int part(int p_n) {
    part_n = p_n;
    int min_x = INT_MAX, max_x = INT_MIN;
    for (auto x : crabs) {
        if (x > max_x)
            max_x = x;
        if (x < min_x)
            min_x = x;
    }
    int min_fuel = INT_MAX;
    for (int x = min_x; x <= max_x; x++) {
        int fuel = 0;
        for (auto x2 : crabs)
            fuel += fuel_cost(abs(x2 - x));
        if (fuel < min_fuel)
            min_fuel = fuel;
    }
    return min_fuel;
}

const string line =
#include "day7.txt"
    ;

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl;
    char c;
    int n;
    stringstream ss(line);
    while (ss >> n) {
        crabs.push_back(n);
        ss >> c;
    }
    cout << "Part 1 - " << part(1) << endl
         << "Part 2 - " << part(2) << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
