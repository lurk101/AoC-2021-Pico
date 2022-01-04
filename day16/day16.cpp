#define title "--- Day 16: Packet Decoder ---"
#include <iostream>
#include <list>
#include <sstream>
using namespace std;

#include "pico/stdlib.h"

unsigned n_bits = 0;
unsigned bits;
unsigned version_sum;
list<uint64_t> stack;
stringstream ss;

unsigned read_32_bits(void) {
    unsigned r = 0;
    for (unsigned i = 0; i < 8; i++) {
        char c;
        ss >> c;
        if (ss.eof())
            c = 0;
        else if (c <= '9')
            c -= '0';
        else
            c = c - 'A' + 10;
        r = (r << 4) | c;
    }
    n_bits = 32;
    return r;
}

unsigned get_bits(unsigned n) {
    static union {
        uint64_t u64;
        uint32_t u32[2];
    } b;
    b.u32[1] = 0;
    while (n--) {
        if (n_bits == 0)
            b.u32[0] = read_32_bits();
        b.u64 <<= 1;
        n_bits--;
    }
    return b.u32[1];
}

unsigned packet(void) {
    auto v = get_bits(3);
    version_sum += v;
    auto type_id = get_bits(3);
    unsigned bit_count = 6;
    switch (type_id) {
    case 4: {
        uint64_t l = 0;
        unsigned five;
        do {
            l <<= 4;
            five = get_bits(5);
            bit_count += 5;
            l |= five & 0xf;
        } while (five & 0x10);
        stack.push_front(l);
        break;
    }
    default: {
        unsigned len, operands = 0, mode = get_bits(1);
        bit_count += 1;
        switch (mode) {
        case 0:
            len = get_bits(15);
            bit_count += 15 + len;
            while (len) {
                len -= packet();
                operands++;
            }
            break;
        case 1:
            len = get_bits(11);
            operands = len;
            bit_count += 11;
            while (len--)
                bit_count += packet();
            break;
        }
        list<uint64_t> local;
        while (operands--) {
            local.push_front(stack.front());
            stack.pop_front();
        }
        uint64_t v1 = local.front();
        local.pop_front();
        while (local.size()) {
            uint64_t v2 = local.front();
            local.pop_front();
            switch (type_id) {
            case 0:
                v1 = v1 + v2;
                break;
            case 1:
                v1 = v1 * v2;
                break;
            case 2:
                if (v2 < v1)
                    v1 = v2;
                break;
            case 3:
                if (v2 > v1)
                    v1 = v2;
                break;
            case 5:
                v1 = v1 > v2 ? 1 : 0;
                break;
            case 6:
                v1 = v1 < v2 ? 1 : 0;
                break;
            case 7:
                v1 = v1 == v2 ? 1 : 0;
                break;
            default:
                abort();
            }
        }
        stack.push_front(v1);
        break;
    }
    }
    return bit_count;
}

const string line =
#include "day16.txt"
    ;

int main() {
    auto start = time_us_32();
    stdio_init_all();
    ss = stringstream(line);
    version_sum = 0;
    packet();
    cout << title << endl
         << "Part 1 - " << version_sum << endl
         << "Part 2 - " << stack.front() << endl
         << "Elapsed - " << (time_us_32() - start) / 1000.0 << " ms." << endl;
}
