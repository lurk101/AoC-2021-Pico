#define title "--- Day 20: Trench Map ---"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;

#include "pico/stdlib.h"

struct universe {
    bool decoder_ring[512];
    vector<vector<bool>> points;
    bool infinity = false;

    uint32_t encode(int y, int x) const {
        uint32_t result = 0;
        for (int i = y - 1; i <= y + 1; i++)
            for (int j = x - 1; j <= x + 1; j++)
                result =
                    (result << 1) |
                    (i >= 0 && i < points.size() && j >= 0 && j < points[i].size() ? points[i][j]
                                                                                   : infinity);
        return result;
    }

    void enhance() {
        vector<vector<bool>> t(points.size() + 2, vector<bool>(points.size() + 2, false));
        for (int i = -1; i <= int(points.size()); i++)
            for (int j = -1; j <= int(points[0].size()); j++)
                t[i + 1][j + 1] = decoder_ring[encode(i, j)];
        infinity ^= decoder_ring[0];
        points = move(t);
    }

    uint32_t points_on() const {
        uint32_t on = 0;
        for (auto& y : points)
            for (auto x : y)
                if (x)
                    on++;
        return on;
    }
};

const string lines[] = {
#include "day20.txt"
};

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl;
    universe uni;
    for (int x = 0; x < 512; x++)
        uni.decoder_ring[x] = lines[0][x] == '#';
    for (int y = 2; y < sizeof(lines) / sizeof(lines[0]); y++) {
        vector<bool> line;
        for (auto c : lines[y])
            line.push_back(c == '#');
        uni.points.push_back(line);
    }
    uni.enhance();
    uni.enhance();
    cout << "Part 1 -  " << uni.points_on() << endl;
    for (int i = 2; i < 50; i++)
        uni.enhance();
    cout << "Part 2 -  " << uni.points_on() << endl
         << "Elapsed - " << (time_us_32() - start) / 1e6 << " s." << endl;
}
