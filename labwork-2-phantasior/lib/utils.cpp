#include "consts.h"
#include "number.h"

#include <vector>
#include <iomanip>
#include <cinttypes>

bool operator<=(const std::vector<uint32_t>& lhs, const std::vector<uint32_t>& rhs) {
    for (int16_t i = DECIMAL_ARRAY_SIZE - 1; i >= 0; --i) {
        if (lhs[i] == rhs[i])
            continue;

        return lhs[i] <= rhs[i];
    }

    return true;
}

uint16_t GetDecimalLength(uint32_t i) {
    if (i == 0) return 1;

    uint16_t length = 0;
    while (i > 0) {
        length++;
        i /= 10;
    }

    return length;
}

void AddDecimal(std::vector<uint32_t>& lhs, const std::vector<uint32_t>& rhs) {
    uint8_t carry = 0;

    for (uint16_t i = 0; i < DECIMAL_ARRAY_SIZE; ++i) {
        lhs[i] += rhs[i] + carry;
        carry = lhs[i] >= DECIMAL_BASE;
        if (carry) {
            lhs[i] -= DECIMAL_BASE;
        }
    }
}

void SubstractDecimal(std::vector<uint32_t>& lhs, const std::vector<uint32_t>& rhs) {
    uint8_t carry = 0;
    for (uint16_t i = 0; i < DECIMAL_ARRAY_SIZE; ++i) {
        if (lhs[i] < rhs[i] + carry) {
            lhs[i] = DECIMAL_BASE + lhs[i] - rhs[i] - carry;
            carry = 1;
        } else {
            lhs[i] = lhs[i] - rhs[i] - carry;
            carry = 0;
        }
    }
}

void DoubleDecimal(std::vector<uint32_t>& decimal) {
    uint8_t carry = 0;
    for (uint16_t i = 0; i < DECIMAL_ARRAY_SIZE; ++i) {
        uint32_t cur = carry + decimal[i] * 2;
        decimal[i] = cur % DECIMAL_BASE;
        carry = cur / DECIMAL_BASE;
    }
}

void HalveDecimal(std::vector<uint32_t>& decimal) {
    uint8_t carry = 0;
    for (int16_t i = DECIMAL_ARRAY_SIZE - 1; i >= 0; --i) {
        uint64_t cur = carry * DECIMAL_BASE + decimal[i];
        decimal[i] = cur / 2;
        carry = cur % 2;
    }
}

void ShiftBits(uint2022_t& bs) {
    for (uint16_t i = 2022; i > 0; --i) {
        bs.bits[i] = bs.bits[i - 1];
    }

    bs.bits[0] = 0;
}

void Print(std::ostream& out, const std::vector<uint32_t>& decimal) {
    int16_t ptr = DECIMAL_ARRAY_SIZE - 1;
    while (ptr >= 0 && decimal[ptr] == 0) ptr--;

    if (ptr < 0) {
        out << 0;
        return;
    }

    out << decimal[ptr];
    while (ptr--) {
        out << std::setfill('0') << std::setw(9) << decimal[ptr];
    }
}