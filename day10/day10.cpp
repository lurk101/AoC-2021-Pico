#define title "--- Day 10: Syntax Scoring ---"
#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "pico/stdlib.h"

string s;
unsigned pos;
list<char> stack;

char inline nextc(void) {
    if (pos >= s.length())
        return 0;
    return s[pos++];
}

string l("([{<");
string r(")]}>");
unsigned p1[4] = {3, 57, 1197, 25137};

bool left(char c) { return l.find(c) != l.npos; }

char right(char c) { return r[l.find(c)]; }

void inline push(char c) { stack.push_back(c); }

char inline pop(void) {
    char c = stack.back();
    stack.pop_back();
    return c;
}
unsigned parse1() {
    stack.clear();
    for (;;) {
        char c = nextc();
        if (c == 0)
            return 0;
        if (left(c)) {
            push(right(c));
            continue;
        }
        if (c == pop())
            continue;
        return p1[r.find(c)];
    }
}

uint64_t parse2() {
    stack.clear();
    for (;;) {
        char c = nextc();
        if (c == 0) {
            uint64_t sum = 0;
            while (stack.size()) {
                sum = sum * 5 + r.find(stack.back()) + 1;
                stack.pop_back();
            }
            return sum;
        }
        if (left(c)) {
            push(right(c));
            continue;
        }
        if (c == pop())
            continue;
        return 0;
    }
}

vector<uint64_t> scores;

const string lines[] = {
#include "day10.txt"
};

uint64_t part(unsigned p) {
    uint64_t sum = 0;
    for (auto& line : lines) {
        s = line;
        pos = 0;
        if (p == 1)
            sum += parse1();
        else {
            uint64_t t = parse2();
            if (t)
                scores.push_back(t);
        }
    }
    if (p == 1)
        return sum;
    else {
        sort(scores.begin(), scores.end());
        return scores[scores.size() / 2];
    }
}

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl
         << "Part 1 - " << part(1) << endl
         << "Part 2 - " << part(2) << endl
         << "Elapsed - " << (time_us_32() - start) / 1e3 << " ms." << endl;
}
