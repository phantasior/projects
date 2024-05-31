#pragma once

#include "pool.hpp"

#include <cstdio>
#include <cstdlib>
#include <vector>

template<typename T>
class PoolAllocator {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using is_always_equal = std::false_type;

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    
    template<typename U> 
    struct rebind {
        using other = PoolAllocator<U>; // deprecated
    };

    PoolAllocator() = delete;

    PoolAllocator(std::vector<Pool>& pools) : pools_(&pools) { std::sort(pools_->begin(), pools_->end(),
            [](const Pool& lhs, const Pool& rhs) { return lhs.chunk_size() < rhs.chunk_size(); }); }

    PoolAllocator(const PoolAllocator& other) noexcept { pools_ = other.pools_; }        

    template<typename U>
    PoolAllocator(const PoolAllocator<U>& other) noexcept { pools_ = other.pools(); }

    PoolAllocator(PoolAllocator&& other) noexcept : pools_(std::exchange(other.pools_, nullptr)) {}

    PoolAllocator& operator=(const PoolAllocator& other) noexcept { pools_ = other.pools_; return *this; }

    PoolAllocator& operator=(PoolAllocator&& other) noexcept { pools_ = std::exchange(other.pools_, nullptr); return *this; }

    ~PoolAllocator() = default;

    [[nodiscard]] T* allocate(size_t n) {
        const size_t need = n * sizeof(T);
        for (Pool& pool : *pools_) {
            if (!pool.fit(need)) continue;
            T* mem = pool.allocate<T>();
            report(mem, n);
            return mem;
        }

        throw std::bad_alloc();
    }

    void deallocate(T* ptr, size_t n) {
        for (Pool& pool : *pools_) {
            if (!pool.own(ptr)) continue;
            report(ptr, n, 0);
            pool.deallocate(ptr);
            return;
        }
    }

    bool operator==(const PoolAllocator& other) const { return pools_ == other.pools_; }

    bool operator!=(const PoolAllocator& other) const { return pools_ != other.pools_; }

    size_type max_size() const { return pools_->back().chunk_size(); }

    std::vector<Pool>* pools() const { return pools_; }

private:
    void report(T* p, std::size_t n, bool alloc = true) const {
        if (!DEBUG) return;
        std::cout << "[ALLOCATOR] " << (alloc ? "Alloc: " : "Dealloc: ") << sizeof(T) * n
                  << " bytes at " << std::hex << std::showbase
                  << reinterpret_cast<void*>(p) << std::dec << '\n';
    }

    std::vector<Pool>* pools_;
};
