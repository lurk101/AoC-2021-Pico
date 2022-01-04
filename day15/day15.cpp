#define title "--- Day 15: Chiton ---"
/*  Advent of Code 2021 Day 15 Chiton
    Written 2021 by Eric Olson */
#include <climits>
#include <iostream>
#include <string>
using namespace std;

#include "pico/stdlib.h"

#define SX 100
#define SY 100

char move_cost[SX][SY];
short row_cost[SY * 5], cost[2 * SX + 2][2 * SY + 2], imax, jmax;

typedef struct {
    short i, j;
} direction;
direction dir[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int get_cost(int i, int j, int b) {
    int di = (i - 1) / imax, ii = (i - 1) % imax, dj = (j - 1) / jmax, jj = (j - 1) % jmax;
    return (move_cost[ii][jj] + di + dj + b - 1) % 9 + 1;
}

void dorelax(short im, short jm, int b) {
    int c;
    do {
        c = 0;
        for (int j = 1; j <= jm; j++)
            for (int i = 1; i <= im; i++)
                for (int d = 0; d < 4; d++) {
                    int id = i + dir[d].i;
                    int jd = j + dir[d].j;
                    int r = cost[id][jd] + get_cost(i, j, b);
                    if (r < cost[i][j]) {
                        cost[i][j] = r;
                        c++;
                    }
                }
    } while (c > 0);
}

int docost1(void) {
    for (int i = 0; i < imax + 2; i++)
        for (int j = 0; j < jmax + 2; j++)
            cost[i][j] = SHRT_MAX;
    cost[1][1] = 0;
    dorelax(imax, jmax, 0);
    return cost[imax][jmax];
}

int docost2(void) {
    for (int i = 0; i < 2 * imax + 2; i++)
        for (int j = 0; j < 2 * jmax + 2; j++)
            cost[i][j] = SHRT_MAX;
    cost[1][1] = 0;
    for (int j = 0; j < 5 * jmax; j++)
        row_cost[j] = SHRT_MAX;
    for (int si = 0; si < 4; si++) {
        for (int sj = 0; sj < 4; sj++) {
            for (int j = 0; j < 2 * jmax; j++)
                cost[0][j + 1] = row_cost[sj * jmax + j];
            if (sj == 0)
                for (int i = 0; i < 2 * imax + 2; i++)
                    cost[i][0] = SHRT_MAX;
            else
                for (int i = 0; i < 2 * jmax + 2; i++)
                    cost[i][0] = cost[i][jmax];
            if (si + sj > 0)
                for (int i = 1; i <= 2 * imax; i++)
                    for (int j = 1; j <= 2 * jmax; j++)
                        cost[i][j] = SHRT_MAX;
            dorelax(2 * imax, 2 * jmax, si + sj);
            if (sj < 3)
                for (int j = 0; j < jmax; j++)
                    row_cost[sj * jmax + j] = cost[imax][j + 1];
            else
                for (int j = 0; j < 2 * jmax; j++)
                    row_cost[sj * jmax + j] = cost[imax][j + 1];
        }
    }
    return cost[2 * imax][2 * jmax];
}

const string lines[] = {
#include "day15.txt"
};

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl;
    char s[128];
    for (imax = 0; imax < sizeof(lines) / sizeof(lines[0]); imax++)
        for (jmax = 0; jmax < lines[imax].length(); jmax++)
            move_cost[imax][jmax] = lines[imax][jmax] - '0';
    cout << "Part 1  - " << docost1() << endl
         << "Part 2  - " << docost2() << endl
         << "Elapsed - " << (time_us_32() - start) / 1e6 << " s." << endl;
}
