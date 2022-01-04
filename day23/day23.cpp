#define title "--- Day 23: Amphipod ---"
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

#include "pico/stdlib.h"

const unsigned color_cost[4] = {1, 10, 100, 1000};

struct room_s {
    room_s(char who, vector<char> data) {
        preferred_occupant = who - 'A';
        for (auto& o : data)
            occupants.push_back(o - 'A');
        n_occupants = occupants.size();
        auto it = occupants.begin();
        for (; it != occupants.end(); it++)
            if (*it != preferred_occupant)
                break;
        anchored = it - occupants.begin();
    }

    bool finished() const { return anchored == occupants.size(); }

    bool can_accept() const { return anchored == n_occupants && n_occupants != occupants.size(); }

    bool has_visitors() const {
        return anchored != occupants.size() && anchored != n_occupants && n_occupants != 0;
    }

    int8_t top() const { return occupants[n_occupants - 1]; }

    unsigned pop() {
        unsigned cost =
            (occupants.size() - n_occupants + 1) * color_cost[occupants[n_occupants - 1]];
        occupants[n_occupants - 1] = -1;
        n_occupants--;
        return cost;
    }

    unsigned push(int8_t who) {
        unsigned cost = (occupants.size() - n_occupants) * color_cost[who];
        occupants[n_occupants] = who;
        n_occupants++;
        anchored++;
        return cost;
    }

    int8_t preferred_occupant;
    vector<int8_t> occupants;
    unsigned n_occupants;
    unsigned anchored;
};

unsigned distance(unsigned from, unsigned to) {
    if (to > from)
        return to - from;
    else
        return from - to;
}

struct cave_s {
    int8_t hallway[11];
    vector<room_s> rooms;

    unsigned move_to_room() {
        for (unsigned i = 0; i < 11; i++) {
            int8_t who = hallway[i];
            if (who == -1)
                continue;
            if (!rooms[who].can_accept())
                continue;
            unsigned dest = 2 + 2 * who;
            if (occupied_spaces(i, dest) > 1)
                continue;
            unsigned cost = rooms[who].push(who);
            hallway[i] = -1;
            return cost + color_cost[who] * distance(i, dest);
        }
        return 0;
    }

    unsigned room_to_room() {
        for (unsigned h = 0; h < rooms.size(); h++) {
            if (!rooms[h].has_visitors())
                continue;
            int8_t who = rooms[h].top();
            if (!rooms[who].can_accept())
                continue;
            if (occupied_spaces(2 * h + 2, 2 * who + 2) != 0)
                continue;
            unsigned cost = rooms[h].pop();
            cost += rooms[who].push(who);
            cost += distance(h, who) * 2 * color_cost[who];
            return cost;
        }
        return 0;
    }

    unsigned occupied_spaces(unsigned from, unsigned to) const {
        auto b = min(from, to);
        auto e = max(from, to) + 1;
        unsigned n = 0;
        for (auto i = b; i != e; i++)
            if (hallway[i] != -1)
                n++;
        return n;
    }

    bool solved() const {
        for (auto& room : rooms)
            if (!room.finished())
                return false;
        return true;
    }

    unsigned hallway_move(unsigned room, unsigned dest) {
        if (!rooms[room].has_visitors())
            return 0;
        if (dest % 2 == 0 && dest != 0 && dest != 10)
            return 0;
        unsigned source = room * 2 + 2;
        if (occupied_spaces(source, dest) != 0)
            return 0;

        int8_t who = rooms[room].top();
        unsigned cost = distance(source, dest) * color_cost[who];
        cost += rooms[room].pop();
        hallway[dest] = who;

        return cost;
    }
};

unsigned search(cave_s cave) {
    unsigned cost = 0;
    while (true) {
        unsigned greedy = cave.move_to_room() + cave.room_to_room();
        if (greedy == 0)
            break;
        cost += greedy;
    }
    if (cave.solved())
        return cost;
    unsigned best = numeric_limits<unsigned>::max();
    for (unsigned h = 0; h < cave.rooms.size(); h++) {
        if (!cave.rooms[h].has_visitors())
            continue;
        for (unsigned i = 0; i < 11; i++) {
            if (i % 2 == 0 && i != 0 && i != 10)
                continue;
            if (cave.occupied_spaces(h * 2 + 2, i) != 0)
                continue;
            cave_s copy = cave;
            auto move_cost = copy.hallway_move(h, i);
            if (!move_cost)
                continue;
            auto recursion = search(move(copy));
            if (!recursion)
                continue;
            best = min(best, move_cost + recursion);
        }
    }
    if (best == UINT_MAX)
        return 0;
    return best + cost;
}

const string lines[] = {
#include "day23.txt"
};

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl;
    cave_s c;
    for (int h = 0; h < 11; h++)
        c.hallway[h] = -1;
    room_s room('A', {lines[3][3], lines[2][3]});
    c.rooms.push_back(room);
    room = room_s('B', {lines[3][5], lines[2][5]});
    c.rooms.push_back(room);
    room = room_s('C', {lines[3][7], lines[2][7]});
    c.rooms.push_back(room);
    room = room_s('D', {lines[3][9], lines[2][9]});
    c.rooms.push_back(room);
    cout << "Part 1  - " << search(c) << endl;
    c.rooms.clear();
    for (int h = 0; h < 11; h++)
        c.hallway[h] = -1;
    room = room_s('A', {lines[3][3], 'D', 'D', lines[2][3]});
    c.rooms.push_back(room);
    room = room_s('B', {lines[3][5], 'B', 'C', lines[2][5]});
    c.rooms.push_back(room);
    room = room_s('C', {lines[3][7], 'A', 'B', lines[2][7]});
    c.rooms.push_back(room);
    room = room_s('D', {lines[3][9], 'C', 'A', lines[2][9]});
    c.rooms.push_back(room);
    cout << "Part 2  - " << search(c) << endl
         << "Elapsed - " << (time_us_32() - start) / 1e6 << " s." << endl;
}
