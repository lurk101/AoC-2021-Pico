#define title "--- Day 21: Dirac Dice ---"
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <vector>
using namespace std;

#include "pico/stdlib.h"

unsigned roll_count = 0;

unsigned roll_3(void) {
    static unsigned roll = 1;
    roll_count += 3;
    if (roll < 98) {
        roll += 3;
        return roll + roll + roll - 6;
    } else {
        roll = roll - 97;
        if (roll == 1)
            return 98 + 99 + 100;
        else if (roll == 2)
            return 99 + 100 + 1;
        else
            return 100 + 1 + 2;
    }
}

unsigned p1_score = 0, p2_score = 0;
short p1_pos, p2_pos;

bool player1_part1(void) {
    p1_pos = (p1_pos + roll_3()) % 10;
    p1_score += p1_pos + 1;
    return p1_score >= 1000;
}

bool player2_part1(void) {
    p2_pos = (p2_pos + roll_3()) % 10;
    p2_score += p2_pos + 1;
    return p2_score >= 1000;
}

unsigned part1(void) {
    for (;;) {
        if (player1_part1())
            return p2_score * roll_count;
        if (player2_part1())
            return p1_score * roll_count;
    }
}

const short splits_by_score[10] = {0, 0, 0, 1, 3, 6, 7, 6, 3, 1};

uint64_t p1_wins = 0, p2_wins = 0;

void player2_part2(short p1_pos, short p2_pos, short p1_score, short p2_score, uint64_t instances);

void player1_part2(short p1_pos, short p2_pos, short p1_score, short p2_score, uint64_t instances) {
    for (int score = 3; score <= 9; score++) {
        short new_pos = (p1_pos + score) % 10;
        short new_score = p1_score + new_pos + 1;
        uint64_t new_instances = instances * splits_by_score[score];
        if (new_score >= 21)
            p1_wins += new_instances;
        else
            player2_part2(new_pos, p2_pos, new_score, p2_score, new_instances);
    }
}

void player2_part2(short p1_pos, short p2_pos, short p1_score, short p2_score, uint64_t instances) {
    for (int score = 3; score <= 9; score++) {
        short new_pos = (p2_pos + score) % 10;
        short new_score = p2_score + new_pos + 1;
        uint64_t new_instances = instances * splits_by_score[score];
        if (new_score >= 21)
            p2_wins += new_instances;
        else
            player1_part2(p1_pos, new_pos, p1_score, new_score, new_instances);
    }
}

uint64_t part2() {
    player1_part2(p1_pos, p2_pos, 0, 0, 1);
    return max(p1_wins, p2_wins);
}
const string lines[] = {
#include "day21.txt"
};

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl;
    string l = lines[0];
    stringstream ss(l);
    vector<string> results(istream_iterator<string>{ss}, istream_iterator<string>());
    p1_pos = stoi(results[4]);
    l = lines[1];
    ss = stringstream(l);
    results = vector<string>(istream_iterator<string>{ss}, istream_iterator<string>());
    p2_pos = stoi(results[4]);
    p1_pos = (p1_pos + 9) % 10;
    p2_pos = (p2_pos + 9) % 10;
    short p1_save = p1_pos, p2_save = p2_pos;
    cout << "Part 1  - " << part1() << endl;
    p1_pos = p1_save;
    p2_pos = p2_save;
    cout << "Part 2  - " << part2() << endl
         << "Elapsed - " << (time_us_32() - start) / 1e6 << " s." << endl;
}
