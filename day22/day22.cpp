#define title "--- Day 22: Reactor Reboot ---"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
using namespace std;

#include "pico/stdlib.h"

struct point {
    int64_t x : 21;
    int64_t y : 21;
    int64_t z : 21;
};

struct cuboid {
  public:
    point low, high;
    uint64_t volume() const {
        return abs((int64_t(high.x) - low.x + 1) * (int64_t(high.y) - low.y + 1) *
                   (int64_t(high.z) - low.z + 1));
    }
    bool is_overlapped(const cuboid& b) const { return overlap(b) || b.overlap(*this); }
    bool valid(void) const { return (high.x >= low.x) && (high.y >= low.y) && (high.z >= low.z); }

  private:
    bool overlap(const cuboid& c) const {
        return (low.x <= c.high.x && high.x >= c.low.x) &&
               (low.y <= c.high.y && high.y >= c.low.y) && (low.z <= c.high.z && high.z >= c.low.z);
    }
};

bool operator==(const cuboid& a, const cuboid& b) {
    return a.high.x == b.high.x && a.high.y == b.high.y && a.high.z == b.high.z &&
           a.low.x == b.low.x && a.low.y == b.low.y && a.low.z == b.low.z;
}

struct cube_rule {
    bool on;
    cuboid cube;
} __attribute__((packed));

struct a_cube {
    list<point> p;
    cuboid c;
};
list<a_cube> all_c;

bool operator<(const a_cube& a, const a_cube& b) { return a.p.size() > b.p.size(); }

const cuboid invalid_cube = {{1, 1, 1}, {0, 0, 0}};

list<cuboid> explode(const cuboid& c1, const cuboid& c2) {
    struct range {
        int low, high;
    };
    typedef range slice[3];
    const range below_x = {int(c1.low.x), int(c2.low.x - 1)},
                above_x = {int(c2.high.x + 1), int(c1.high.x)},
                below_y = {int(c1.low.y), int(c2.low.y - 1)},
                above_y = {int(c2.high.y + 1), int(c1.high.y)},
                below_z = {int(c1.low.z), int(c2.low.z - 1)},
                above_z = {int(c2.high.z + 1), int(c1.high.z)},
                middle_x = {max(int(c1.low.x), int(c2.low.x)), min(int(c1.high.x), int(c2.high.x))},
                middle_y = {max(int(c1.low.y), int(c2.low.y)), min(int(c1.high.y), int(c2.high.y))},
                middle_z = {max(int(c1.low.z), int(c2.low.z)), min(int(c1.high.z), int(c2.high.z))};
    const slice x_slices = {below_x, middle_x, above_x}, y_slices = {below_y, middle_y, above_y},
                z_slices = {below_z, middle_z, above_z};
    list<cuboid> new_quads;
    cuboid quadrants[3][3][3];
    for (auto& a : quadrants)
        for (auto& b : a)
            for (auto& c : b)
                c = invalid_cube;
    for (int x = 0; x < 3; x++)
        for (int y = 0; y < 3; y++)
            for (int z = 0; z < 3; z++) {
                cuboid c3 = {{x_slices[x].low, y_slices[y].low, z_slices[z].low},
                             {x_slices[x].high, y_slices[y].high, z_slices[z].high}};
                if (c3.valid()) {
                    if (!c3.is_overlapped(c1) || !c3.is_overlapped(c2)) {
                        quadrants[x][y][z] = c3;
                        new_quads.push_back(c3);
                    }
                }
            }
    list<cuboid> recombined;
    while (new_quads.size()) {
        for (auto& a_c : all_c) {
            bool found = true;
            for (auto& p : a_c.p) {
                if (!quadrants[p.x][p.y][p.z].valid()) {
                    found = false;
                    break;
                }
            }
            if (found) {
                cuboid t;
                t.low.x = quadrants[a_c.c.low.x][a_c.c.low.y][a_c.c.low.z].low.x;
                t.low.y = quadrants[a_c.c.low.x][a_c.c.low.y][a_c.c.low.z].low.y;
                t.low.z = quadrants[a_c.c.low.x][a_c.c.low.y][a_c.c.low.z].low.z;
                t.high.x = quadrants[a_c.c.high.x][a_c.c.high.y][a_c.c.high.z].high.x;
                t.high.y = quadrants[a_c.c.high.x][a_c.c.high.y][a_c.c.high.z].high.y;
                t.high.z = quadrants[a_c.c.high.x][a_c.c.high.y][a_c.c.high.z].high.z;
                recombined.push_back(t);
                for (auto& x : quadrants)
                    for (auto& y : x)
                        for (auto& z : y)
                            if (z.is_overlapped(t))
                                z = invalid_cube;
                bool done = false;
                while (!done) {
                    done = true;
                    for (auto it = new_quads.begin(); it != new_quads.end(); it++) {
                        if (t.is_overlapped(*it)) {
                            new_quads.erase(it);
                            done = false;
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
    return recombined;
}

list<cuboid> cubes;

void add_cube(const cuboid& c, bool add) {
    list<cuboid> new_cubes;
    list<cuboid> pos;
    for (auto it = cubes.begin(); it != cubes.end(); it++) {
        if (c.is_overlapped(*it)) {
            list<cuboid> n_c = explode(*it, c);
            for (auto& c2 : n_c)
                new_cubes.push_back(c2);
            pos.push_back(*it);
        }
    }
    for (auto& c2 : pos)
        for (auto it = cubes.begin(); it != cubes.end(); it++)
            if (*it == c2) {
                cubes.erase(it);
                break;
            }
    for (auto& c2 : new_cubes)
        cubes.push_back(c2);
    if (add)
        cubes.push_back(c);
}

const string lines[] = {
#include "day22.txt"
};

uint64_t part(int part_n) {
    static const cuboid init_region = {{-50, -50, -50}, {50, 50, 50}};
    cubes.clear();
    for (auto& l : lines) {
        stringstream ss(l);
        string s;
        cube_rule rule;
        ss >> s;
        rule.on = s == "on";
        char c;
        ss >> c >> c;
        int i;
        ss >> i;
        rule.cube.low.x = i;
        ss >> c >> c;
        ss >> i;
        rule.cube.high.x = i;
        ss >> c >> c >> c;
        ss >> i;
        rule.cube.low.y = i;
        ss >> c >> c;
        ss >> i;
        rule.cube.high.y = i;
        ss >> c >> c >> c;
        ss >> i;
        rule.cube.low.z = i;
        ss >> c >> c;
        ss >> i;
        rule.cube.high.z = i;
        if (part_n == 1 && !rule.cube.is_overlapped(init_region))
            continue;
        add_cube(rule.cube, rule.on);
    }
    uint64_t volume = 0;
    for (auto& c : cubes)
        volume += c.volume();
    return volume;
}

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl;
    for (int x = 3; x > 0; x--)
        for (int y = 3; y > 0; y--)
            for (int z = 3; z > 0; z--)
                for (int x2 = 0; x2 + x < 4; x2++)
                    for (int y2 = 0; y2 + y < 4; y2++)
                        for (int z2 = 0; z2 + z < 4; z2++) {
                            a_cube a;
                            a.c.low.x = x2;
                            a.c.high.x = x2 + x - 1;
                            a.c.low.y = y2;
                            a.c.high.y = y2 + y - 1;
                            a.c.low.z = z2;
                            a.c.high.z = z2 + z - 1;
                            for (int x3 = x2; x3 < x2 + x; x3++)
                                for (int y3 = y2; y3 < y2 + y; y3++)
                                    for (int z3 = z2; z3 < z2 + z; z3++)
                                        a.p.push_back({x3, y3, z3});
                            all_c.push_back(a);
                        }
    all_c.sort();
    cout << "Part 1  - " << part(1) << endl
         << "Part 2  - " << part(2) << endl
         << "Elapsed - " << (time_us_32() - start) / 1e6 << " s." << endl;
}
