#define title "--- Day 12: Passage Pathing ---"
#include <iostream>
#include <list>
#include <map>
#include <sstream>
using namespace std;

#include "pico/stdlib.h"

typedef struct {
    list<string> to;
    uint8_t visit;
} cave;
map<string, cave*> caves;

void connect(string from, string to) {
    auto c = caves.find(from);
    if (c != caves.end())
        c->second->to.push_back(to);
    else {
        cave* c2 = new cave;
        c2->visit = false;
        c2->to.push_back(to);
        caves.insert(pair<string, cave*>(from, c2));
    };
}

const string lines[] = {
#include "day12.txt"
};

void load(void) {
    for (auto c : caves)
        delete &c.second->to;
    caves.clear();
    string from, to;
    for (auto& line : lines) {
        from = line;
        to = from.substr(from.find('-') + 1);
        from = from.substr(0, from.find('-'));
        connect(from, to);
        connect(to, from);
    }
}

unsigned paths = 0;
bool two_done = false;

void search(list<string>& tos, int part) {
    for (auto& to : tos) {
        if (to == "end") {
            paths++;
            continue;
        }
        if (to == "start")
            continue;
        auto c = caves.find(to);
        bool done = false;
        if (islower(to[0])) {
            if (c->second->visit) {
                if (two_done || (part == 1))
                    continue;
                two_done = true;
                done = true;
            }
        }
        c->second->visit++;
        search(c->second->to, part);
        c->second->visit--;
        if (done)
            two_done = false;
    }
}

unsigned part(int part) {
    load();
    paths = 0;
    two_done = false;
    auto c = caves.find("start");
    search(c->second->to, part);
    return paths;
}

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl
         << "Part 1 - " << part(1) << endl
         << "Part 2 - " << part(2) << endl
         << "Elapsed - " << (time_us_32() - start) / 1e6 << " s." << endl;
}
