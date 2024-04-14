#include <algorithm>
#include <type_traits>
#include <iostream>
#include <memory>

const size_t kBitInByte = 8;

template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type divUp(T x, T y) {
    return (x + y - 1) / y;
}

void print(const void* raw_ptr, size_t size_in_bits) {
    std::cout << '[';

    const uint8_t* ptr = static_cast<const uint8_t*>(raw_ptr);
    for (size_t i = 0; i < size_in_bits / kBitInByte; ++i) {
        std::cout << std::bitset<kBitInByte>(ptr[i]);
        std::cout << (size_in_bits % kBitInByte != 0 ? "|" : "");
    }

    for (size_t i = 0; i < size_in_bits % kBitInByte; ++i) {
        std::cout << ((ptr[size_in_bits / kBitInByte] >> i) & 1);
    }

    std::cout << "]\n";
}


void invertBits(void* raw_mem, size_t len, auto&& should_be_reversed) {
    auto invertBit = [&](std::byte& byte, size_t bit) -> void {
        byte ^= static_cast<std::byte>(1 << bit);
    };

    std::byte* mem = static_cast<std::byte*>(raw_mem);
    for (size_t byte_cnt = 0; byte_cnt < divUp(len, kBitInByte); ++byte_cnt) {
        std::byte byte = *(mem + byte_cnt);

        for (size_t bit_cnt = 0; bit_cnt < std::min(8ul, len - byte_cnt * kBitInByte); ++bit_cnt) {
            if (std::forward<decltype(should_be_reversed)>(should_be_reversed)(byte_cnt * kBitInByte + kBitInByte - bit_cnt - 1)) {
                invertBit(byte, bit_cnt);
            }
        }

        *(mem + byte_cnt) = byte;
    }
}


void test(size_t size_in_bits, auto&& should_be_reversed, const std::string& test_message) {
    std::cout << "Test: " << test_message << '\n';
    auto ptr = std::make_unique<uint8_t[]>(divUp(size_in_bits, kBitInByte));

    std::cout << "Before: "; 
    print(ptr.get(), size_in_bits);

    invertBits(static_cast<void*>(ptr.get()), size_in_bits, std::forward<decltype(should_be_reversed)>(should_be_reversed));

    std::cout << "After:  ";
    print(ptr.get(), size_in_bits);
    std::cout << "-------------------------\n";
}

int main() {
    test(32, [](size_t bit) -> bool { return true; }, "reverse all bits");
    test(32, [](size_t bit) -> bool { return bit % 2 == 1; }, "reverse odd bits");
    test(32, [](size_t bit) -> bool { return bit % 2 == 0; }, "reverse even bits");
    test(7, [](size_t bit) -> bool { return bit % 2 == 0; }, "bits number % 8 != 0");
    test(50, [](size_t bit) -> bool { return bit % 2 == 0; }, "bits number % 8 != 0");
    test(81, [](size_t bit) -> bool { return bit % 2 == 0; }, "bits number % 8 != 0");
    test(49, [](size_t bit) -> bool { return bit % 2 == 0; }, "bits number % 8 != 0");
}


