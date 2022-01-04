#define title "--- Day 13: Transparent Origami ---"
#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>
using namespace std;

#include "pico/stdlib.h"

typedef struct {
    unsigned y, x;
} dot_t;

bool operator<(const dot_t& a, const dot_t& b) {
    if (a.y < b.y)
        return true;
    if (a.y > b.y)
        return false;
    return a.x < b.x;
};

set<dot_t> paper;

void do_x_fold(unsigned x) {
    list<dot_t> found;
    for (auto& dot : paper)
        if (dot.x > x)
            found.push_back(dot);
    for (auto& dot : found) {
        paper.erase(dot);
        dot.x = x - (dot.x - x);
        if (paper.find(dot) == paper.end())
            paper.insert(dot);
    }
}

void do_y_fold(unsigned y) {
    list<dot_t> found;
    for (auto& dot : paper)
        if (dot.y > y)
            found.push_back(dot);
    for (auto& dot : found) {
        paper.erase(dot);
        dot.y = y - (dot.y - y);
        if (paper.find(dot) == paper.end())
            paper.insert(dot);
    }
}

const string lines[] = {
#include "day13.txt"
};

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl;
    string s;
    int i;
    for (i = 0;; i++) {
        string s = lines[i];
        if (s.length() == 0)
            break;
        stringstream ss(s);
        dot_t dot;
        char c;
        ss >> dot.x >> c >> dot.y;
        paper.insert(dot);
    }
    i++;
    bool first = true;
    for (; i < sizeof(lines) / sizeof(lines[0]); i++) {
        string s = lines[i];
        stringstream ss(s.substr(11));
        char x, c;
        ss >> x;
        unsigned row_col;
        ss >> c >> row_col;
        if (x == 'x')
            do_x_fold(row_col);
        else
            do_y_fold(row_col);
        if (first) {
            first = false;
            cout << "Part 1 - " << paper.size() << endl << "Part 2" << endl;
        }
    }
    unsigned max_x = 0, max_y = 0;
    for (auto& dot : paper) {
        if (dot.x > max_x)
            max_x = dot.x;
        if (dot.y > max_y)
            max_y = dot.y;
    }
    for (unsigned y = 0; y <= max_y; y++) {
        for (unsigned x = 0; x <= max_x; x++) {
            dot_t dot = {y, x};
            cout << ((paper.find(dot) == paper.end()) ? ' ' : '#');
        }
        cout << endl;
    }
    cout << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
