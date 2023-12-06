#pragma once
#include <cinttypes>
#include <iostream>
#include <bitset>

struct uint2022_t {
    static const uint16_t N = 2022;
    std::bitset<N> bits;

    uint2022_t() {
        for (uint16_t i = 0; i < N; ++i) {
            bits[i] = 0;
        }
    }

    uint16_t get_size() const {
        for (int16_t i = N - 1; i >= 0; --i) {
            if (bits[i] == 0) continue;
            return i + 1;
        }

        return 0;
    }

    void print() const {
        bool is_first = true;
        for (int32_t i = N - 1; i >= 0; --i) {
            if (is_first && bits[i] == 0) continue;
            is_first = false;
            std::cout << bits[i];
        }

        std::cout << '\n';
    }
};

static_assert(sizeof(uint2022_t) <= 300, "Size of uint2022_t must be no higher than 300 bytes");

uint2022_t from_uint(uint32_t i);

uint2022_t from_string(const char* buff);

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs);

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs);

bool operator>=(const uint2022_t& lhs, const uint2022_t& rhs);

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value);