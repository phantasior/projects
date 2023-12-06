#pragma once

#include "Buffer.hpp"

template<typename T, typename Allocator = std::allocator<T>>
class StaticBuffer : public Buffer<T, Allocator> {
public:
    using Buffer<T, Allocator>::begin_;
    using Buffer<T, Allocator>::end_;
    using Buffer<T, Allocator>::arr_;
    using Buffer<T, Allocator>::capacity_;
    using Buffer<T, Allocator>::allocator_;
    using typename Buffer<T, Allocator>::allocator_traits;

    StaticBuffer() : Buffer<T, Allocator>() {}
    StaticBuffer(const StaticBuffer<T, Allocator>& other) : Buffer<T, Allocator>(other) {}
    StaticBuffer(size_t size) : Buffer<T, Allocator>(size) {}

    // keeps invariant -> [last_element + 1 != begin_]
    void push_back(const T& element) {
        assert(capacity_ > 0);

        construct_at(end_, element); // invariant that here freed memory
        if (end_ == begin_ - 1 || (begin_ == arr_ && end_ == arr_ + capacity_ - 1)) {
            std::destroy_at(begin_);
            ++begin_;
            if (begin_ == arr_ + capacity_) {
                begin_ = arr_;
            }
        }

        end_ = end_ == arr_ + capacity_ - 1 ? arr_ : end_ + 1;        
    }

    void pop_back() {
        if (begin_ == end_) return;
        if (end_ == arr_) end_ = arr_ + capacity_;
        end_--;
    }

    void push_front(const T& element) {
        assert(capacity_ > 0);

        if (end_ == begin_ - 1 || (begin_ == arr_ && end_ == arr_ + capacity_ - 1)) {
            std::destroy_at(end_);
            if (end_ == arr_) {
                end_ = arr_ + capacity_ - 1;
            } else {
                --end_;
            }
        }

        if (begin_ == arr_) {
            begin_ = arr_ + capacity_;
        }

        begin_ = begin_ == arr_ ? arr_ + capacity_ - 1 : begin_ - 1;

        try {
            construct_at(begin_, element);
        } catch(...) {
            begin_ = begin_ == arr_ + capacity_ - 1 ? arr_ : begin_ + 1;
            throw;
        }
    }

    void pop_front() {
        if (begin_ == end_) return;
        begin_ = begin_ == arr_ + capacity_ - 1 ? arr_ : begin_ + 1;
    }
};

