#include "../lib/StaticBuffer.hpp"
#include "../lib/DynamicBuffer.hpp"

#include <iostream>
#include <algorithm>
#include <vector>

template<typename T>
class MyAllocator {
public:
    using value_type = T;

    T* allocate(size_t n) const {
        return reinterpret_cast<T*>(::operator new(n * sizeof(T)));
    }

    template<typename... Args>
    void construct(T* ptr, const Args&... args) {
        new(ptr) T(args...);
    }

    void destroy(T* ptr) {
        ptr->~T();
    }

    void deallocate(T* ptr, size_t) {
        operator delete(ptr);
    }
};

auto print = [](const int& n) { std::cout << n << ' '; };

int main() {
    using std::endl;
    using std::cout;
    StaticBuffer<int> buf(5);
    for (int i = 0; i < 20; ++i) {
        if (i % 2 == 0) {
            buf.push_back(i);
        } else {
            buf.push_front(i);
        }
    }

    cout << buf;
}