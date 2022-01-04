#define title "--- Day 19: Beacon Scanner ---"
#include <algorithm>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

#include <pico/stdlib.h>

typedef struct {
    int dim, sign;
} rotation_t[3];

const int X = 0, Y = 1, Z = 2, POS = 1, NEG = -1;

const rotation_t rotations[24] = {
    {{X, POS}, {Y, POS}, {Z, POS}}, {{X, POS}, {Z, POS}, {Y, NEG}}, {{X, POS}, {Y, NEG}, {Z, NEG}},
    {{X, POS}, {Z, NEG}, {Y, POS}}, {{X, NEG}, {Y, POS}, {Z, NEG}}, {{X, NEG}, {Z, NEG}, {Y, NEG}},
    {{X, NEG}, {Y, NEG}, {Z, POS}}, {{X, NEG}, {Z, POS}, {Y, POS}}, {{Y, POS}, {X, POS}, {Z, NEG}},
    {{Y, POS}, {Z, NEG}, {X, NEG}}, {{Y, POS}, {X, NEG}, {Z, POS}}, {{Y, POS}, {Z, POS}, {X, POS}},
    {{Y, NEG}, {X, POS}, {Z, POS}}, {{Y, NEG}, {Z, POS}, {X, NEG}}, {{Y, NEG}, {X, NEG}, {Z, NEG}},
    {{Y, NEG}, {Z, NEG}, {X, POS}}, {{Z, POS}, {X, POS}, {Y, POS}}, {{Z, POS}, {Y, POS}, {X, NEG}},
    {{Z, POS}, {X, NEG}, {Y, NEG}}, {{Z, POS}, {Y, NEG}, {X, POS}}, {{Z, NEG}, {X, POS}, {Y, NEG}},
    {{Z, NEG}, {Y, NEG}, {X, NEG}}, {{Z, NEG}, {X, NEG}, {Y, POS}}, {{Z, NEG}, {Y, POS}, {X, POS}}};

struct point_s {
    int x, y, z;
    int manhattan() { return abs(x) + abs(y) + abs(z); }
    point_s rotate(const rotation_t& rot) const {
        const int vals[3] = {x, y, z};
        return {(vals[rot[0].dim]) * rot[0].sign, (vals[rot[1].dim]) * rot[1].sign,
                (vals[rot[2].dim]) * rot[2].sign};
    }
    bool parse(const char* s) {
        if (strlen(s) == 0)
            return false;
        sscanf(s, "%d,%d,%d", &x, &y, &z);
        return true;
    }
};

point_s operator-(const point_s& lhs, const point_s& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

point_s operator+(const point_s& lhs, const point_s& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

bool operator==(const point_s& lhs, const point_s& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

struct hasher {
    size_t operator()(point_s const& s) const { return s.x ^ s.y ^ s.z; }
};

typedef point_s beacon_t;

struct scanner_orientation {
    rotation_t rotation;
    point_s position;
};

struct scanner_s {
    vector<beacon_t> beacons;
    vector<int> hash;
    point_s position;
    void generate_hash(void) {
        for (auto it1 = beacons.begin(); it1 != beacons.end() - 1; it1++)
            for (auto it2 = beacons.begin() + 1; it2 != beacons.end(); it2++) {
                auto dx = it1->x - it2->x;
                auto dy = it1->y - it2->y;
                auto dz = it1->z - it2->z;
                hash.push_back(dx * dx + dy * dy + dz * dz);
            }
        sort(hash.begin(), hash.end());
    }
    bool check_hash(const scanner_s& other) const {
        unsigned match = 0;
        for (auto it = hash.begin(), ito = other.hash.begin();
             it != hash.end() && ito != other.hash.end();) {
            if (*it < *ito)
                it++;
            else if (*it > *ito)
                ito++;
            else {
                it++;
                ito++;
                match++;
            }
        }
        return match >= 66;
    }

    void normalize(const scanner_orientation& orientation) {
        for (auto& beacon : beacons)
            beacon = beacon.rotate(orientation.rotation) + orientation.position;
        position = orientation.position;
    }
    bool overlaps(const scanner_s& other, unsigned threshold, scanner_orientation& o) const {
        if (!check_hash(other))
            return false;
        for (auto& rotation : rotations) {
            unordered_map<beacon_t, unsigned, hasher> cnts;
            for (auto& lhs : beacons)
                for (auto& rhs : other.beacons)
                    cnts[lhs - rhs.rotate(rotation)]++;
            for (auto& el : cnts)
                if (el.second >= threshold) {
                    o.position = el.first;
                    o.rotation[0] = rotation[0];
                    o.rotation[1] = rotation[1];
                    o.rotation[2] = rotation[2];
                    return true;
                }
        }
        return false;
    }
};

vector<scanner_s> scanners;

void normalize(void) {
    unordered_set<int> fixed;
    deque<int> queue;
    fixed.insert(0);
    queue.push_back(0);
    scanners[0].position = point_s{0, 0, 0};
    while (!queue.empty()) {
        auto tested = queue.front();
        queue.pop_front();
        for (unsigned other = 0; other < scanners.size(); other++) {
            if (fixed.find(unsigned(other)) != fixed.end())
                continue;
            scanner_orientation rot;
            if (!scanners[tested].overlaps(scanners[other], 12, rot))
                continue;
            scanners[other].normalize(rot);
            queue.push_back(other);
            fixed.insert(other);
        }
    }
}

const char* lines[] = {
#include "day19.txt"
};

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl;
    string line;
    scanner_s scan;
    scan.beacons.clear();
    for (auto& line : lines) {
        if (strlen(line) == 0) {
            scanners.push_back(scan);
            scan.beacons.clear();
            continue;
        }
        if (line[1] == '-')
            continue;
        beacon_t b;
        if (b.parse(line))
            scan.beacons.push_back(b);
    }
    scanners.push_back(scan);
    for (auto& scanner : scanners)
        scanner.generate_hash();
    normalize();
    unordered_set<beacon_t, hasher> unique_beacons;
    for (const auto& s : scanners)
        for (const auto& b : s.beacons)
            unique_beacons.insert(b);
    cout << "Part 1  - " << unique_beacons.size() << endl;
    int best = 0;
    for (auto lhs : scanners)
        for (auto rhs : scanners)
            best = max(best, (lhs.position - rhs.position).manhattan());
    cout << "Part 2  - " << best << endl
         << "Elapsed - " << (time_us_32() - start) / 1e6 << " s." << endl;
}
