// So struct uint2022_t represents my very long long int
// inside of struct i have std::bitsts<2022> that just contains my number

// When i need to do functions: [from_string, operator<<] i create
// additional array uint32_t[75] that will containt decimal representation
// of my number. This way saves memory (about 20-30 bytes).

#include "number.h"
#include "consts.h"
#include "utils.h"

#include <iostream>
#include <cinttypes>
#include <vector>

uint2022_t from_uint(uint32_t i) {
    uint2022_t n;

    uint16_t pos = 0;
    while (i > 0) {
        n.bits[pos] = i % BINARY_BASE;
        i >>= 1;
        pos++;
    }

    return n;
}

uint2022_t from_string(const char* buff) {
    // Algorithm:
    // 
    // 1. From char* we will get decimal[] that just
    // contains our number (from buff) in decimal
    // 2. We will count bin_pow(decimal) = min({2^k}) where 2^k >= decimal;
    // Now we always decreasing k > 0 - pow;
    // If currently bin_pow <= decimal than decimal -= bin_pow
    // and current bit of our result is 1 n[curr] = 1;

    uint2022_t n;

    std::vector<uint32_t> decimal(DECIMAL_ARRAY_SIZE, 0);
    uint16_t buff_size = strlen(buff);


    uint32_t first_nums = 0;
    for (uint8_t i = 0; i < (buff_size % NUMBER_OF_DIGITS_IN_PLACE); ++i) {
        first_nums *= 10;
        first_nums += (buff[i] - '0');
    }

    decimal[buff_size / NUMBER_OF_DIGITS_IN_PLACE] = first_nums;


    uint16_t cur_ptr = (buff_size % NUMBER_OF_DIGITS_IN_PLACE);
    while (cur_ptr < buff_size) {
        uint32_t cur_num = 0;

        for (uint16_t i = cur_ptr; i < cur_ptr + NUMBER_OF_DIGITS_IN_PLACE; ++i) {
            cur_num *= 10;
            cur_num += (buff[i] - '0');
        }

        decimal[(buff_size / NUMBER_OF_DIGITS_IN_PLACE) - (cur_ptr / NUMBER_OF_DIGITS_IN_PLACE) - 1] = cur_num;
        cur_ptr += NUMBER_OF_DIGITS_IN_PLACE;
    }


    // Now in tmp[] we have our decimal number from buffer

    std::vector<uint32_t> decimal_power_of_two(DECIMAL_ARRAY_SIZE, 0);
    decimal_power_of_two[0] = 1;

    uint16_t pow = 0;
    while (decimal_power_of_two <= decimal) {
        pow++;
        DoubleDecimal(decimal_power_of_two);
    }
    
    while (pow > 0) {
        HalveDecimal(decimal_power_of_two);

        if (decimal_power_of_two <= decimal) {
            n.bits[pow - 1] = 1;
            SubstractDecimal(decimal, decimal_power_of_two);
        }

        pow--;
    }

    return n;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t n;

    uint16_t ptr = 0;
    uint8_t carry = 0;
    uint16_t N = std::max(lhs.get_size(), rhs.get_size()) + 1;  

    while (ptr < N || carry) {
        n.bits[ptr] = (lhs.bits[ptr] ^ rhs.bits[ptr] ^ carry);
        carry = (
                (lhs.bits[ptr] & rhs.bits[ptr]) | 
                (lhs.bits[ptr] & carry) | 
                (rhs.bits[ptr] & carry)
            );

        ptr++;
    }



    return n;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t n;

    int8_t carry = 0;
    for (uint16_t i = 0; i < lhs.get_size() + 1; ++i) {
        if (lhs.bits[i] < rhs.bits[i] + carry) {
            n.bits[i] = BINARY_BASE + lhs.bits[i] - rhs.bits[i] - carry;
            carry = 1;
        } else {
            n.bits[i] = lhs.bits[i] - rhs.bits[i] - carry;
            carry = 0;
        }
    }

    return n;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t n;

    uint2022_t multiplier = rhs;
    uint16_t size = rhs.get_size();

    for (int16_t i = 0; i < lhs.get_size(); ++i) {
        if (lhs.bits[i]) {
            n = n + multiplier;
        }

        ShiftBits(multiplier);
    }

    return n;
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t n;
    uint2022_t cur_value;

    for (int16_t i = lhs.get_size() - 1; i >= 0; --i) {
        ShiftBits(cur_value);
        cur_value.bits[0] = lhs.bits[i];

        if (cur_value >= rhs) {
            n.bits[i] = 1;
            cur_value = cur_value - rhs;
        }
    }

    return n;
}

bool operator>=(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (int16_t i = 2022 - 1; i >= 0; --i) {
        if (lhs.bits[i] != rhs.bits[i]) {
            return lhs.bits[i] >= rhs.bits[i];
        }
    }

    return true;
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (int32_t i = lhs.N; i >= 0; --i) {
        if (rhs.bits[i] != lhs.bits[i]) {
            return false;
        }
    }

    return true;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
    // Algorithm:
    // We have std::bitset, if currentrly
    // bitset[i] == 1 than
    // res += current decimal_power_of_two;

    std::vector<uint32_t> decimal(DECIMAL_ARRAY_SIZE, 0); 
    std::vector<uint32_t> decimal_power_of_two(DECIMAL_ARRAY_SIZE, 0);
    decimal_power_of_two[0] = 1;

    uint16_t pow = 0;
    for (int i = value.get_size(); i >= 0; --i) {
        if (value.bits[pow]) {
            AddDecimal(decimal, decimal_power_of_two);
        }

        DoubleDecimal(decimal_power_of_two);
        pow++;
    }

    Print(stream, decimal);

    return stream;
}