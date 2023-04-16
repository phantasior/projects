#pragma once

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <limits>

const bool DEBUG = false;

class Pool {
private:
    struct Chunk {
        Chunk* next;
    };

public:
    Pool() = delete;
    Pool(const Pool& other) = delete;
    Pool& operator=(const Pool& other) = delete;

    Pool(Pool&& other) noexcept
        : alloc_(std::exchange(other.alloc_, nullptr))
        , head_(std::exchange(other.head_, nullptr))
        , chunks_num_(std::move(other.chunks_num_))
        , chunk_size_(std::move(other.chunk_size_))
    {}

    Pool& operator=(Pool&& other) noexcept {
        if (this == &other) return *this;
        delete[] alloc_;
        alloc_ = std::exchange(other.alloc_, nullptr);
        head_ = std::exchange(other.head_, nullptr);
        chunks_num_ = other.chunks_num_;
        chunk_size_ = other.chunk_size_;
        return *this;
    }

    constexpr Pool(size_t chunks_num, size_t chunk_size)
        : chunks_num_(chunks_num)
        , chunk_size_(std::max(chunk_size, 8ul)) {
        alloc_ = malloc(chunks_num_ * chunk_size_);
        if (!alloc_) {
            throw std::bad_alloc();
        }

        head_ = reinterpret_cast<Chunk*>(alloc_);
        Chunk* chunk = head_;
        for (size_t i = 0; i < chunks_num_ - 1; ++i) {
            chunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(chunk) + chunk_size_);
            chunk = chunk->next;
        }

        chunk->next = nullptr;
    }

    ~Pool() {
        delete[] alloc_;
    }

    size_t chunk_size() const { return chunk_size_; }

    template<typename T>
    [[nodiscard]] T* allocate() {
        if (head_ == nullptr) {
            throw std::bad_alloc();
        }
        
        T* mem = reinterpret_cast<T*>(head_);
        report(mem, chunk_size_);
        head_ = head_->next;
        return mem;
    }

    template<typename T>
    void deallocate(T* ptr) {
        report(ptr, chunk_size_, 0);
        Chunk* chunk = reinterpret_cast<Chunk*>(ptr);
        chunk->next = head_;
        head_ = chunk;
    }

    template<typename T>
    bool own(T* ptr) const {
        return alloc_ <= ptr && ptr <= static_cast<void*>(static_cast<char*>(alloc_) + (chunks_num_ * chunk_size_));
    }

    bool fit(size_t need) const {
        return head_ != nullptr && need <= chunk_size_;
    }

private:
    template<typename T>
    void report(T* p, std::size_t n, bool alloc = true) const {
        if (!DEBUG || p == nullptr) return;
        std::cout << "[POOL] " << (alloc ? "Alloc: " : "Dealloc: ") << n
                  << " bytes at " << std::hex << std::showbase
                  << reinterpret_cast<void*>(p) << std::dec << '\n';
    }

    void report(void* p, std::size_t n, bool alloc = true) const {
        if (!DEBUG || p == nullptr) return;
        std::cout << "[POOL] " << (alloc ? "Alloc: " : "Dealloc: ") << n << " bytes at " 
                  << std::hex << std::showbase << p << std::dec << '\n';
    }

    void* alloc_ = nullptr;
    Chunk* head_;
    size_t chunks_num_ = 0;
    size_t chunk_size_ = 0;
};
