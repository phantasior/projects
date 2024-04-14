#include <algorithm>
#include <type_traits>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

const size_t kBitInByte = 8;

class Timer {
public:
    Timer() {
        start = std::chrono::steady_clock::now();
    }

    ~Timer() {
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "Time elapsed: " << diff.count() << " seconds" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> start;
};

template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type divUp(T x, T y) {
    return (x + y - 1) / y;
}

template<std::size_t N>
void reverse(std::bitset<N> &b) {
    for(std::size_t i = 0; i < N / 2; ++i) {
        std::swap(b[i], b[N - i - 1]);
    }
}

void print(const void* raw_ptr, size_t size_in_bits) {
    std::cout << '[';

    const uint8_t* ptr = static_cast<const uint8_t*>(raw_ptr);
    for (size_t i = 0; i < size_in_bits / kBitInByte; ++i) {
        auto bs = std::bitset<kBitInByte>(ptr[i]);
        reverse(bs);
        std::cout << bs;
        std::cout << (size_in_bits % kBitInByte == 0 && i == size_in_bits / kBitInByte - 1 ? "" : "|");
    }

    for (size_t i = 0; i < size_in_bits % kBitInByte; ++i) {
        std::cout << ((ptr[size_in_bits / kBitInByte] >> i) & 1);
    }

    std::cout << "]\n";
}


void invertBits(void* raw_mem, size_t len, auto&& should_be_reversed) {
    static_assert(std::is_invocable_r_v<bool, decltype(should_be_reversed), size_t>);

    auto invertBit = [&](uint8_t& byte, size_t bit) -> void {
        byte ^= 1 << bit;
    };

    std::uint8_t* mem = static_cast<uint8_t*>(raw_mem);
    for (size_t byte_cnt = 0; byte_cnt < divUp(len, kBitInByte); ++byte_cnt) {
        uint8_t byte = *(mem + byte_cnt);

        for (size_t bit_cnt = 0; bit_cnt < std::min(8ul, len - byte_cnt * kBitInByte); ++bit_cnt) {
            size_t cur_bit = byte_cnt * kBitInByte + bit_cnt;
            if (std::forward<decltype(should_be_reversed)>(should_be_reversed)(cur_bit)) {
                invertBit(byte, bit_cnt);
            }
        }

        *(mem + byte_cnt) = byte;
    }
}

void solve(void* mem, size_t len, auto&& should_be_reversed) {
    if (len == 0) {
        return;
    }

    const size_t min_job_size = 1000;
    const size_t hardware_threads = std::thread::hardware_concurrency();
    const size_t max_threads = hardware_threads == 0 ? 2 : hardware_threads;
    const size_t num_threads = std::min(max_threads, divUp(len, min_job_size));

    std::vector<std::thread> threads;
    threads.reserve(num_threads - 1);
    uint8_t* first = static_cast<uint8_t*>(mem);
    const size_t block_size = len / num_threads;
    for (size_t i = 0; i < num_threads - 1; ++i) {
        threads.emplace_back(invertBits<bool(*)(size_t)>, static_cast<void*>(first), block_size, std::forward<decltype(should_be_reversed)>(should_be_reversed));
        first += block_size / kBitInByte;
    }

    const size_t lastPartLen = len/8 + static_cast<uint8_t*>(mem) - first;
    invertBits(first, lastPartLen, std::forward<decltype(should_be_reversed)>(should_be_reversed));

    for (auto& t : threads) {
        t.join();
    }
}

void test(size_t size_in_bits, auto&& should_be_reversed, const std::string& test_message) {
    std::cout << "Test: " << test_message << '\n';
    std::cout << "There is " << size_in_bits << " bits\n";
    auto ptr = std::make_unique<uint8_t[]>(divUp(size_in_bits, kBitInByte));

    // std::cout << "Before: "; 
    // print(ptr.get(), size_in_bits);

    std::cout << "Multithread solition:\n";

    {
        Timer timer;
        solve(ptr.get(), size_in_bits, std::forward<decltype(should_be_reversed)>(should_be_reversed));
    }

    std::cout << "Singlethread solution:\n";

    {
        Timer timer;
        invertBits(ptr.get(), size_in_bits, std::forward<decltype(should_be_reversed)>(should_be_reversed));
    }   

    // std::cout << "After:  ";
    // print(ptr.get(), size_in_bits);

    std::cout << "-------------------------\n";
}

int main() {
    test(100000000, [](size_t bit) -> bool { return bit % 10 == 3; }, "do not reverse at all");
    test(100000,    [](size_t bit) -> bool { return true; },          "reverse all bits");
    test(3200,      [](size_t bit) -> bool { return bit % 2 == 1; },  "reverse odd bits");
    test(4000,      [](size_t bit) -> bool { return bit % 2 == 0; },  "reverse even bits");
    test(15,        [](size_t bit) -> bool { return bit % 2 == 0; },  "reverse even bits");
    test(2222,      [](size_t bit) -> bool { return bit % 2 == 0; },  "reverse even bits");
    test(1234,      [](size_t bit) -> bool { return bit % 2 == 0; },  "reverse even bits");
}