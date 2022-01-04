#define title "--- Day 8: Seven Segment Search ---"
#include <iostream>
#include <vector>
using namespace std;

#include "pico/stdlib.h"

const uint8_t A = 1 << 0, B = 1 << 1, C = 1 << 2, D = 1 << 3, E = 1 << 4, F = 1 << 5, G = 1 << 6;

// clang-format off
const uint8_t N[10] = {A | B | C | E | F | G,
                       C | F,
                       A | C | D | E | G,
                       A | C | D | F | G,
                       B | C | D | F,
                       A | B | D | F | G,
                       A | B | D | E | F | G,
                       A | C | F,
                       A | B | C | D | E | F | G,
                       A | B | C | D | F | G};
// clang-format on

struct entry {
    vector<uint8_t> numbers, patterns;
};

uint8_t S[256];

entry parse(const string& input) {
    entry result;
    bool bar = false;
    auto it = input.begin();
    while (it != input.end()) {
        uint8_t d = 0;
        for (; it != input.end() && islower(*it); it++)
            d |= 1 << (*it - 'a');
        if (bar)
            result.numbers.push_back(d);
        else
            result.patterns.push_back(d);
        while (it != input.end() && (isspace(*it) || *it == '|')) {
            if (*it == '|')
                bar = true;
            it++;
        }
    }
    return result;
}

bool is_lit(uint8_t disp, uint8_t segment) { return disp & (1 << segment); }

uint8_t segments(uint8_t display) { return __builtin_popcount(display); }

void solve(const entry& input, uint8_t* result) {
    for (int r = 0; r < 7; r++)
        result[r] = N[8];
    for (auto pattern : input.patterns) {
        uint8_t w;
        switch (segments(pattern)) {
        case 2:
            for (uint8_t wire_in = 0; wire_in < 7; wire_in++)
                result[wire_in] &= is_lit(pattern, wire_in) ? (C | F) : ~(C | F);
            break;
        case 3:
            for (uint8_t wire_in = 0; wire_in < 7; wire_in++)
                result[wire_in] &= is_lit(pattern, wire_in) ? (A | C | F) : ~(A | C | F);
            break;
        case 4:
            for (uint8_t wire_in = 0; wire_in < 7; wire_in++)
                result[wire_in] &= is_lit(pattern, wire_in) ? (B | C | D | F) : ~(B | C | D | F);
            break;
        case 5:
            for (uint8_t wire_in = 0; wire_in < 7; wire_in++)
                if (!is_lit(pattern, wire_in))
                    result[wire_in] &= (B | C | E | F);
            break;
        case 6:
            for (uint8_t wire_in = 0; wire_in < 7; wire_in++)
                if (!is_lit(pattern, wire_in))
                    result[wire_in] &= (C | D | E);
            break;
        }
    }
    for (uint8_t w = 0; w < 7; w++) {
        if (__builtin_popcount(result[w]) != 1)
            continue;
        for (uint8_t wu = 0; wu < 7; wu++) {
            if (__builtin_popcount(result[wu]) == 1)
                continue;
            result[wu] &= ~result[w];
        }
    }
}

uint32_t decode(const entry& puzzle, uint8_t* wiring) {
    uint32_t result = 0;
    for (auto digit : puzzle.numbers) {
        uint8_t display = 0;
        for (uint8_t b = 0; b < 7; b++) {
            if (is_lit(digit, b))
                display |= wiring[b];
        }

        result = result * 10 + S[display];
    }
    return result;
}

const char* lines[] = {
#include "day8.txt"
};

int main() {
    auto start = time_us_32();
    stdio_init_all();
    for (int n = 0; n < 10; n++)
        S[N[n]] = n;
    string input_line;
    uint32_t part1 = 0;
    uint32_t part2 = 0;
    part1 = 0;
    for (auto& line : lines) {
        entry p = parse(line);
        for (auto num : p.numbers) {
            uint8_t cnt = segments(num);
            if (cnt == 2 || cnt == 3 || cnt == 4 || cnt == 7)
                part1++;
        };
        uint8_t result[7];
        solve(p, result);
        part2 += decode(p, result);
    }

    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << part2 << endl
         << "Elapsed - " << (time_us_32() - start) / 1e3 << " ms." << endl;
}
