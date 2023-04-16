// STATISTIC:
// ---------
// 1. Vector
// operation: push_back 16384 times
// With PoolAllocator: 80ms           // not bad
// Without: 140ms                    //
//
// 2. Map<int, int>
// opeartion: insert 16384 times
// with PoolAllocator: ~1000ms       // cool
// without PoolAllocator: ~8000ms   //
//
// 3. List<int>
// operation: push_back 16384 times
// with PoolAllocator: ~100ms        // nice
// without PoolAllocator: ~3000ms   //
//
// 4. Just allocations
// PoolAllocator: ~100ms
// std::Allocator: ~4000ms

// P.S. implied that pools were allocated in compile time


#include "../lib/allocator.hpp"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>

#include <vector>
#include <map>
#include <list>

using std::endl;
using std::cout;

template<typename T>
size_t get_duration(T func) {
    auto start = std::chrono::high_resolution_clock::now();
    
    func();

    auto stop = std::chrono::high_resolution_clock::now();
    return duration_cast<std::chrono::microseconds>(stop - start).count();
}

void TestVector() {
    std::cout << "Testing Vector<int>:\n";
    std::vector<Pool> pools;
    pools.emplace_back(1, 1);
    pools.emplace_back(1, 2);
    pools.emplace_back(1, 4);
    pools.emplace_back(1, 8);
    pools.emplace_back(1, 16);
    pools.emplace_back(1, 32);
    pools.emplace_back(1, 64);
    pools.emplace_back(1, 128);
    pools.emplace_back(1, 256);
    pools.emplace_back(1, 512);
    pools.emplace_back(1, 1024);
    pools.emplace_back(1, 2048);
    pools.emplace_back(1, 4096);
    pools.emplace_back(1, 8192);
    pools.emplace_back(1, 16384);
    pools.emplace_back(1, 32768);
    pools.emplace_back(1, 65536);
    PoolAllocator<int> allocator(pools);

    std::vector<int, PoolAllocator<int>> v1(allocator);
    size_t time = get_duration([&]() {
        for (size_t i = 0; i < 16384; ++i) {
            v1.push_back(i);
        }
    });
    std::cout << "Vector with PoolAllocator: " << time << endl;;
    std::vector<int> v2;
    time = get_duration([&]() {
        for (size_t i = 0; i < 16384; ++i) {
            v2.push_back(i);
        }
    });
    cout << "Vector without PoolAllocator: " << time << endl;
}

void TestMap() {
    std::cout << "Testing Map<int, int>:\n";
    std::vector<Pool> pools;
    pools.emplace_back(16384, 40);
    PoolAllocator<std::pair<const int, int>> allocator(pools);
    size_t time = get_duration([&]() {
        std::map<int, int, std::less<int>, PoolAllocator<std::pair<const int, int>>> m1(std::less<int>(), allocator);
        for (size_t i = 0; i < 16384; ++i) {
            m1.insert({i, i});
        }
    });
    std::cout << "Map with PoolAllocator: " << time << endl;;
    time = get_duration([]() {
        std::map<int, int> m2;
        for (size_t i = 0; i < 16384; ++i) {
            m2.insert({i, i});
        }
    });
    cout << "Map without PoolAllocator: " << time << endl;
}

void TestList() {
    std::cout << "Testing List<int>:\n";
    std::vector<Pool> pools;
    pools.emplace_back(16384, 24);
    PoolAllocator<int> allocator(pools);

    size_t time = get_duration([&]() {
        std::list<int, PoolAllocator<int>> l(allocator);
        for (size_t i = 0; i < 16384; ++i) {
            l.push_back(i);
        }
    });
    std::cout << "List with PoolAllocator: " << time << endl;;
    time = get_duration([]() {
        std::list<int> l;
        for (size_t i = 0; i < 16384; ++i) {
            l.push_back(i);
        }
    });
    cout << "List without PoolAllocator: " << time << endl;
}

void TestAllocates() {
    std::cout << "Testing just allocations:\n";
    std::vector<Pool> pools;
    pools.emplace_back(1000, 1);
    pools.emplace_back(1000, 2);
    pools.emplace_back(1000, 4);
    pools.emplace_back(1000, 8);
    pools.emplace_back(1000, 16);
    pools.emplace_back(1000, 32);
    pools.emplace_back(1000, 64);
    pools.emplace_back(1000, 128);
    pools.emplace_back(1000, 256);
    pools.emplace_back(1000, 512);
    pools.emplace_back(1000, 1024);
    pools.emplace_back(1000, 2048);
    pools.emplace_back(1000, 4096);
    pools.emplace_back(1000, 8192);
    pools.emplace_back(1000, 16384);
    pools.emplace_back(1000, 32768);
    pools.emplace_back(1000, 65536);
    PoolAllocator<int> pool_alloc(pools);

    std::cout << "With PoolAllocator: " << time << endl;;
    size_t time = get_duration([&]() {
        for (size_t i = 1; i <= 100; ++i) {
            auto ptr = pool_alloc.allocate(i);
            pool_alloc.deallocate(ptr, i);
        }
    });

    std::cout << "Without PoolAllocator: " << time << endl;;
    std::allocator<int> std_alloc;
    time = get_duration([&]() {
        for (size_t i = 1; i <= 100; ++i) {
            auto ptr = std_alloc.allocate(i);
            std_alloc.deallocate(ptr, i);
        }
    });
}


int main() {
    TestVector();
    TestMap();
    TestList();
    TestAllocates();
}