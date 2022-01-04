#define title "--- Day 18: Snailfish ---"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <optional>
#include <sstream>
#include <vector>
using namespace std;

#include "pico/stdlib.h"

typedef uint64_t binary;

struct snum {
    snum() = default;
    snum(snum&&) = default;
    snum& operator=(snum&&) = default;
    snum(binary value) : m_left(nullptr), m_right(nullptr), m_val(value) {}
    snum(const snum&);
    snum& operator=(const snum&);
    operator binary() const;

  private:
    bool is_leaf() const { return m_left == nullptr && m_right == nullptr; }
    void reduce();
    bool split();
    bool explode();
    bool search(uint32_t depth, optional<binary*>& last_value, optional<binary>& oflow);
    friend istream& operator>>(istream&, snum&);
    friend snum operator+(const snum& left, const snum& right);

    unique_ptr<snum> m_left;
    unique_ptr<snum> m_right;
    binary m_val;
};

snum::snum(const snum& b) : m_left(nullptr), m_right(nullptr), m_val(b.m_val) {
    if (b.is_leaf())
        return;
    m_left = make_unique<snum>(*b.m_left);
    m_right = make_unique<snum>(*b.m_right);
}

snum& snum::operator=(const snum& b) {
    if (this == &b)
        return *this;
    if (b.is_leaf()) {
        m_val = b.m_val;
        m_left = nullptr;
        m_right = nullptr;
        return *this;
    }
    m_left = make_unique<snum>(*b.m_left);
    m_right = make_unique<snum>(*b.m_right);
    return *this;
}

istream& operator>>(istream& s, snum& n) {
    char c;
    if (!(s >> c))
        return s;
    n.m_left = make_unique<snum>();
    if (s.peek() == '[')
        s >> *n.m_left;
    else
        s >> n.m_left->m_val;
    s >> c;
    n.m_right = make_unique<snum>();
    if (s.peek() == '[')
        s >> *n.m_right;
    else
        s >> n.m_right->m_val;
    s >> c;
    return s;
}

snum::operator binary() const {
    if (is_leaf())
        return m_val;
    return 3 * static_cast<binary>(*m_left) + 2 * static_cast<binary>(*m_right);
}

snum operator+(const snum& left, const snum& right) {
    snum r;
    r.m_left = make_unique<snum>(left);
    r.m_right = make_unique<snum>(right);
    r.reduce();
    return r;
}

void snum::reduce() {
    do
        while (explode())
            ;
    while (split());
}

bool snum::split() {
    if (!is_leaf()) {
        if (m_left->split())
            return true;
        return m_right->split();
    }
    if (m_val < 10)
        return false;
    m_left = make_unique<snum>(m_val / 2);
    m_right = make_unique<snum>((m_val + 1) / 2);
    return true;
}

bool snum::explode() {
    optional<binary> oflow;
    optional<binary*> last_value;
    return search(0, last_value, oflow) || oflow.has_value();
}

bool snum::search(uint32_t depth, optional<binary*>& last_value, optional<binary>& oflow) {
    if (is_leaf()) {
        if (oflow.has_value()) {
            m_val += oflow.value();
            oflow.reset();
            return true;
        }
        last_value = &m_val;
        return false;
    }
    if (depth < 4 || oflow.has_value() || !m_left->is_leaf() || !m_right->is_leaf())
        return m_left->search(depth + 1, last_value, oflow) ||
               m_right->search(depth + 1, last_value, oflow);
    if (last_value.has_value())
        (*last_value.value()) += m_left->m_val;
    oflow = m_right->m_val;
    m_left = nullptr;
    m_right = nullptr;
    m_val = 0;
    return false;
}

const string lines[] = {
#include "day18.txt"
};

int main() {
    auto start = time_us_32();
    stdio_init_all();
    cout << title << endl;
    vector<snum> ns;
    snum n;
    for (auto& line : lines) {
        stringstream ss(line);
        ss >> n;
        ns.push_back(n);
    }
    snum sum = accumulate(ns.begin() + 1, ns.end(), ns[0], plus<>());
    cout << "Part 1  - " << static_cast<binary>(sum) << endl;
    binary best = 0;
    for (size_t i = 0; i < ns.size(); i++)
        for (size_t j = i + 1; j < ns.size(); j++)
            best =
                max({best, static_cast<binary>(ns[i] + ns[j]), static_cast<binary>(ns[j] + ns[i])});
    cout << "Part 2  - " << best << endl
         << "Elapsed - " << (time_us_32() - start) / 1e6 << " s." << endl;
}
