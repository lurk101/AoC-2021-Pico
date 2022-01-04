#define title "--- Day 17: Trick Shot ---"
#include <bits/stdc++.h>
#include <iostream>
#include <list>
#include <sstream>
using namespace std;

#include "pico/stdlib.h"

list<int> candidates;
int target_x_lo, target_y_lo, target_x_hi, target_y_hi, hit_count = 0, max_max_h = INT_MIN;

bool hit_x(int x) { return (x >= target_x_lo) && (x <= target_x_hi); }
bool hit(int x, int y) { return hit_x(x) && (y >= target_y_lo) && (y <= target_y_hi); }

void fire() {
    for (int init_vx = 0; init_vx <= target_x_hi; init_vx++) {
        int pos_x = 0, vx = init_vx;
        while ((pos_x <= target_x_hi) && vx) {
            pos_x += vx;
            if (hit_x(pos_x)) {
                candidates.push_back(init_vx);
                break;
            }
            vx--;
        }
    }
    int last_vx = 0, last_vy = 0;
    for (auto& init_vx : candidates) {
        for (int init_vy = abs(target_y_lo); init_vy >= target_y_lo; init_vy--) {
            int pos_x = 0, pos_y = 0, vy = init_vy, vx = init_vx;
            bool done = false;
            int max_h = INT_MIN;
            while (pos_y >= target_y_lo) {
                pos_x += vx;
                if (vx)
                    vx--;
                pos_y += vy;
                vy--;
                if (pos_y > max_h)
                    max_h = pos_y;
                if (hit(pos_x, pos_y)) {
                    if ((last_vx != init_vx) || (last_vy != init_vy)) {
                        hit_count++;
                        last_vx = init_vx;
                        last_vy = init_vy;
                        if (max_max_h < max_h)
                            max_max_h = max_h;
                    }
                }
            }
        }
    }
}

const string line =
#include "day17.txt"
    ;

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl;
    stringstream ss(line);
    ss.seekg(15);
    char c;
    ss >> target_x_lo >> c >> c >> target_x_hi >> c >> c >> c >> target_y_lo >> c >> c >>
        target_y_hi;
    if (target_x_lo < 0) {
        target_x_lo *= -1;
        target_x_hi *= -1;
        swap(target_x_lo, target_x_hi);
    }
    fire();
    cout << "Part 1 - " << max_max_h << endl
         << "Part 2 - " << hit_count << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
