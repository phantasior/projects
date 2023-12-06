#pragma once

#include "Buffer.hpp"

template<typename T, typename Allocator = std::allocator<T>>
class DynamicBuffer : public Buffer<T, Allocator> {
public:
    using Buffer<T, Allocator>::begin_;
    using Buffer<T, Allocator>::end_;
    using Buffer<T, Allocator>::arr_;
    using Buffer<T, Allocator>::capacity_;
    using Buffer<T, Allocator>::allocator_;
    using typename Buffer<T, Allocator>::allocator_traits;

    DynamicBuffer() : Buffer<T, Allocator>() {}
    DynamicBuffer(const DynamicBuffer<T, Allocator>& other) : Buffer<T, Allocator>(other) {}
    DynamicBuffer(size_t size) : Buffer<T, Allocator>(size) {}

    // keeps invariant -> [last_element + 1 != begin_]
    void push_back(const T& element) {
        if (arr_ == nullptr) {
            this->reserve(1);
        }

        if (this->size() + 1 == capacity_) {
            this->reserve(2 * capacity_);
        }

        allocator_traits::construct(allocator_, end_, element);
        end_++;
    }

    void pop_back() {
        if (this->empty()) return;
        if (end_ == arr_) {
            end_ = arr_ + capacity_;
        }

        end_--;
    }

    void push_front(const T& element) {
        if (arr_ == nullptr) {
            this->reserve(1);
        }

        if (this->size() + 1 == capacity_) {
            this->reserve(2 * capacity_);
        }

        if (begin_ == arr_) {
            begin_ = arr_ + capacity_;
        }

        allocator_traits::construct(allocator_, begin_ - 1, element);
        --begin_;
    }

    void pop_front() {
        if (begin_ == end_) return;
        if (begin_ == arr_ + capacity_ - 1) {
            begin_ = arr_;
        } else {
            ++begin_;
        }
    }
};

