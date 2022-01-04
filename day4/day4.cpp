#define title "--- Day 4: Giant Squid ---"
#include <bitset>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "pico/stdlib.h"

vector<unsigned> numbers;
typedef struct {
    int8_t square[25];
    bool bingo;
} board;

vector<board> boards, save_boards;
const uint8_t stamped = 0x80;

bool bingo(board& b, uint8_t sq) {
    for (int i = 0; i < 25; i++)
        if (b.square[i] == sq) {
            b.square[i] |= stamped;
            if (stamped & b.square[0] & b.square[1] & b.square[2] & b.square[3] & b.square[4])
                return true;
            if (stamped & b.square[5] & b.square[6] & b.square[7] & b.square[8] & b.square[9])
                return true;
            if (stamped & b.square[10] & b.square[11] & b.square[12] & b.square[13] & b.square[14])
                return true;
            if (stamped & b.square[15] & b.square[16] & b.square[17] & b.square[18] & b.square[19])
                return true;
            if (stamped & b.square[20] & b.square[21] & b.square[22] & b.square[23] & b.square[24])
                return true;
            if (stamped & b.square[0] & b.square[5] & b.square[10] & b.square[15] & b.square[20])
                return true;
            if (stamped & b.square[1] & b.square[6] & b.square[11] & b.square[16] & b.square[21])
                return true;
            if (stamped & b.square[2] & b.square[7] & b.square[12] & b.square[17] & b.square[22])
                return true;
            if (stamped & b.square[3] & b.square[8] & b.square[13] & b.square[18] & b.square[23])
                return true;
            if (stamped & b.square[4] & b.square[9] & b.square[14] & b.square[19] & b.square[24])
                return true;
        }
    return false;
}

int part1(void) {
    for (auto n : numbers)
        for (auto& b : boards)
            if (bingo(b, n)) {
                unsigned sum = 0;
                for (int i = 0; i < 25; i++)
                    if (!(b.square[i] & stamped))
                        sum += b.square[i];
                return sum * n;
            }
    return 0;
}

int part2(void) {
    boards = move(save_boards);
    board last_board;
    unsigned last_n;
    for (auto n : numbers)
        for (auto& b : boards)
            if (!b.bingo && bingo(b, n)) {
                b.bingo = true;
                last_board = b;
                last_n = n;
            }
    unsigned sum = 0;
    for (int i = 0; i < 25; i++)
        if (!(last_board.square[i] & stamped))
            sum += last_board.square[i];
    return sum * last_n;
}

const string lines[] = {
#include "day4.txt"
};

int main() {
    auto start = time_us_32();
    stdio_init_all();
    char c;
    unsigned n;
    numbers.clear();
    stringstream ss;
    for (int i = 0; i < sizeof(lines) / sizeof(lines[0]); i++)
        ss << lines[i] << ' ';
    for (;;) {
        ss >> n;
        numbers.push_back(n);
        if (ss.peek() != ',')
            break;
        ss >> c;
    }
    boards.clear();
    for (;;) {
        board b;
        b.bingo = false;
        for (int x = 0; x < 25; x++) {
            ss >> n;
            b.square[x] = n;
        }
        boards.push_back(b);
        if (ss.eof())
            break;
    }
    save_boards = boards;
    cout << title << endl
         << "Part 1 - " << part1() << endl
         << "Part 2 - " << part2() << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
