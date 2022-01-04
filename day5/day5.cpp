#define title "--- Day 5: Hydrothermal Venture ---"
#include "stdio.h"
#include <algorithm>
using namespace std;

#include "pico/stdlib.h"

struct vents {

    void addy(unsigned x, unsigned y1, unsigned y2) {
        if (y1 > y2)
            swap(y2, y1);
        for (; y1 <= y2; y1++)
            add(x, y1);
    }

    void addx(unsigned y, unsigned x1, unsigned x2) {
        if (x1 > x2)
            swap(x2, x1);
        for (; x1 <= x2; x1++)
            add(x1, y);
    }

    void addxy(unsigned x1, unsigned x2, unsigned y1, unsigned y2) {
        if (x1 > x2) {
            swap(y2, y1);
            swap(x2, x1);
        }
        int y_incr = 1;
        if (y1 > y2)
            y_incr = -1;
        for (; x1 <= x2; x1++, y1 += y_incr)
            add(x1, y1);
    }

    unsigned count(void) {
        unsigned n = 0;
        for (unsigned y = 0; y < 1000; y++)
            for (unsigned x = 0; x < 1000; x++)
                if (get(x, y) > 1)
                    n++;
        return n;
    }

  private:
    void add(unsigned x, unsigned y) {
        unsigned n = get(x, y);
        if (n < 2)
            set(x, y, n + 1);
    }

    unsigned get(unsigned x, unsigned y) {
        unsigned offs = y * 1000 + x;
        return (v[offs / 16] >> ((offs % 16) * 2)) & 3;
    }

    void set(unsigned x, unsigned y, unsigned n) {
        unsigned offs = y * 1000 + x;
        v[offs / 16] = (v[offs / 16] & ~(3 << ((offs % 16) * 2))) | (n << ((offs % 16) * 2));
    }

    unsigned v[(1000 * 1000 * 2 + 31) / 32];
};

vents v;

const char* lines[] = {
#include "day5.txt"
};

unsigned part(int part_n) {
    for (unsigned i = 0; i < sizeof(lines) / sizeof(lines[0]); i++) {
        unsigned x1, y1, x2, y2;
        sscanf(lines[i], "%u,%u -> %u,%u", &x1, &y1, &x2, &y2);
        if (part_n == 2) {
            if ((x1 != x2) && (y1 != y2))
                v.addxy(x1, x2, y1, y2);
        } else {
            if (x1 == x2)
                v.addy(x1, y1, y2);
            else if (y1 == y2)
                v.addx(y1, x1, x2);
        }
    }
    return v.count();
}

int main() {
    unsigned start = time_us_32();
    stdio_init_all();
    printf("%s\nPart 1  -  %d\nPart 2  -  %d\nElapsed - %f ms.\n", title, part(1), part(2),
           (time_us_32() - start) / 1e3);
}
