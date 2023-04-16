#pragma once

#include <algorithm>
#include <cassert>
#include <climits>
#include <cstddef> 
#include <exception> 
#include <iterator>
#include <iostream>
#include <memory>

template<class T, class... Args>
constexpr T* construct_at( T* p, Args&&... args ) {
    return ::new (const_cast<void*>(static_cast<const volatile void*>(p)))
        T(std::forward<Args>(args)...);
}
 

template<typename InputIt>
InputIt UnInitializedCopy(InputIt first, InputIt last, InputIt dest) {
    using T = typename std::iterator_traits<InputIt>::value_type;
    auto curr = dest;
    try {
        for (; first != last; ++first, ++curr) {
            new (std::addressof(*curr)) T(*first);
        }

        return curr;
    } catch (...) {
        for (; dest != curr; ++dest) {
            dest->~T();
        }
        throw;
    }
}

template<typename T, typename Allocator = std::allocator<T>>
class Buffer;

template<typename T, typename Allocator = std::allocator<T>>
std::ostream& operator<<(std::ostream& out, Buffer<T, Allocator>& v);

template<typename T, typename Allocator>
class Buffer {
protected:
    T* begin_;
    T* end_;
    T* arr_;
    size_t capacity_;
    [[no_unique_adress]] Allocator allocator_;

    template<bool IsConst>
    class BaseIterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = value_type*;
        using reference         = value_type&;

        using ConditionalPtr    = std::conditional_t<IsConst, const pointer, pointer>;
        using ConditionalRef    = std::conditional_t<IsConst, const reference, reference>;
        using ConditionalBuf    = std::conditional_t<IsConst, const Buffer*, Buffer*>;

        BaseIterator() = delete;
        BaseIterator(pointer ptr, ConditionalBuf buf) : ptr_(ptr), buf_(buf) {}
        BaseIterator(const BaseIterator& other) : ptr_(other.ptr_), buf_(other.buf_) {}

        void swap(BaseIterator& other) {
            std::swap(ptr_, other.ptr_);
            std::swap(buf_, other.buf_);
        }

        BaseIterator& operator=(const BaseIterator& other) {
            BaseIterator(other).swap(*this);
            return *this;
        };

        ConditionalRef operator*() { return *ptr_; }
        ConditionalPtr operator->() { return ptr_; }

        BaseIterator operator++(int) {
            BaseIterator tmp = *this; 
            ++(*this);
            return tmp;

        }

        BaseIterator& operator++() { 
            if (++ptr_ == buf_->arr_ + buf_->capacity_) {
                ptr_ = buf_->arr_;
            }

            return *this; 
        }

        BaseIterator operator--(int) {
            BaseIterator tmp = *this; 
            --(*this);
            return tmp;

        }

        BaseIterator& operator--() {
            ptr_ = (ptr_ == buf_->arr_) ? (buf_->arr_ + buf_->capacity_ - 1) : (ptr_ - 1);
            return *this; 
        }

        BaseIterator operator+(int k) {
            pointer new_ptr = ptr_;
            k %= buf_->capacity_;
            if (new_ptr + k > buf_->arr_ + buf_->capacity_ - 1) {
                k -= buf_->arr_ + buf_->capacity_ - 1 - (new_ptr + k);
                new_ptr = buf_->arr_ + k;
            } else {
                new_ptr += k;
            }

            return BaseIterator(new_ptr, buf_);
        }

        BaseIterator operator-(int k) {
            pointer new_ptr = ptr_;
            k %= buf_->capacity_;
            if (new_ptr - k < buf_->arr_) {
                k -= new_ptr - buf_->arr_;
                new_ptr = buf_->arr_ + buf_->capacity_ - 1 - k;
            } else {
                new_ptr -= k;
            }

            return BaseIterator(new_ptr, buf_);
        }
        
        BaseIterator& operator+=(int k) {
            return *this = *this + k;
        }

        BaseIterator& operator-=(int k) {
            return *this = *this - k;
        }


        difference_type operator-(const BaseIterator& other) const {
            if (ptr_ < buf_->begin_ && other.ptr_ > buf_->begin_) {
                return (ptr_ - buf_->arr_) + (buf_->arr_ + buf_->capacity_ - 1 - other.ptr_);
            } else if (ptr_ > buf_->begin_ && other.ptr_ < buf_->begin_) {
                return -(other - *this);
            }

            return ptr_ - other.ptr_;
        }

        bool operator<(const BaseIterator& other) const {
            if (ptr_ < buf_->begin_ && other.ptr_ > buf_->begin_) {
                return false;
            } else if (ptr_ > buf_->begin_ && other.ptr_ < buf_->begin_) {
                return true;
            }

            return ptr_ < other.ptr_;
        }

        bool operator >=(const BaseIterator& other) const {
            return !(*this < other);
        }

        bool operator>(const BaseIterator& other) const {
            return other < *this;
        }

        bool operator<=(const BaseIterator& other) const {
            return !(*this > other);
        }

        bool operator==(const BaseIterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const BaseIterator& other) const { return ptr_ != other.ptr_; }

        T& operator[](size_t idx) {
            idx %= buf_->capacity_;
            if (idx + ptr_ >= buf_->arr + buf_->capacity_) {
                idx -= (buf_->arr_ + buf_->capacity_) - ptr_;
                return buf_->arr_ + idx;
            }

            return *(ptr_ + idx);
        }

    private:
    public:
        ConditionalPtr ptr_;
        ConditionalBuf buf_;
    };

    friend std::ostream& operator<< <>(std::ostream& out, Buffer<T, Allocator>& v);

public:
    using iterator = BaseIterator<false>;  
    using const_iterator = BaseIterator<true>;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using allocator_traits = typename std::allocator_traits<Allocator>;

    Buffer()
        : arr_(nullptr)
        , begin_(nullptr)
        , end_(nullptr)
        , capacity_(0)
    {}

    Buffer(const Buffer& other)
        : capacity_(other.capacity_)
    {
        reserve(other.capacity_);    
        std::uninitialized_copy(other.begin_, other.end_, arr_);
        begin_ = arr_;
        end_ = arr_ + other.size();
    }

    Buffer& operator=(const Buffer& other) {
        if (this == &other) return *this;

        if (arr_ != nullptr) {
            allocator_traits::deallocate(allocator_, arr_, capacity_);
            for (auto it = begin(); it != end(); ++it) {
                std::destroy_at(it.operator->());
            }
        }
        reserve(other.capacity_);
        std::uninitialized_copy(other.begin_, other.end_, arr_);
        begin_ = arr_;
        end_ = arr_ + other.size();

        return *this;
    };

    Buffer(size_t capacity) 
        : begin_(nullptr)
        , end_(nullptr)
        , arr_(nullptr)
        , capacity_(0) 
    { 
        reserve(capacity);
    }

    ~Buffer() {
        std::cout << "~Buffer\n";
        if (arr_ == nullptr) return;

        for (auto it = begin(); it != end(); ++it) {
            std::destroy_at(it.operator->());
        }

        allocator_traits::deallocate(allocator_, arr_, capacity_);
        arr_ = nullptr;
    }

    bool operator==(const Buffer& other) const { return this->size() == other.size() && std::equal(begin(), end(), other.begin()); };
    bool operator!=(const Buffer& other) const { return *this != other; }

    T& operator[](size_t id) { return arr_[id % capacity_]; }
    T& at(size_t id) { 
        if (id < 0 || id > capacity_) {
            throw std::out_of_range("Out of range in buffer");
        }
        return arr_[id]; }

    iterator begin() { return iterator(begin_, this); }
    iterator end() { return iterator(end_, this); }

    const_iterator cbegin() const { return const_iterator(begin_, this); }
    const_iterator cend() const { return const_iterator(end_, this); }

    reverse_iterator rbegin() { return std::reverse_iterator(end()); }
    reverse_iterator rend() { return std::reverse_iterator(begin()); }

    const_reverse_iterator crbegin() const { return  const_reverse_iterator(end()); }
    const_reverse_iterator crend() const { return  const_reverse_iterator(begin()); }

    void swap(Buffer& other) {
        using std::swap;
        swap(begin_, other.begin_);
        swap(end_, other.end_);
        swap(arr_, other.arr_);
        swap(capacity_, other.capacity_);
    }

    size_t size() const {
        if (arr_ == nullptr) return 0;
        return end_ < begin_ ? (end_ - arr_) + ((arr_ + capacity_) - begin_) : end_ - begin_; 
    }
    size_t capacity() const { return capacity_ - 1; }
    size_t max_size() const { return INT_MAX; }
    bool empty() const { return begin_ == end_; }

    void reserve(size_t n) { // -> i will reserve n + 1
        if (n < capacity_) return;

        T* new_arr = allocator_traits::allocate(allocator_, n + 1);
        if (arr_ != nullptr) {
            try {
                std::uninitialized_copy(begin(), end(), new_arr);
            } catch(...) {
                allocator_traits::deallocate(allocator_, new_arr, n + 1);
                throw;
            }
        }

        auto cur = begin_;
        for (auto cur = begin_; cur != end_; ++cur) {
            std::destroy_at(cur);
        }

        end_ = new_arr + size();
        begin_ = arr_ = new_arr;
        capacity_ = n + 1;
    }

    void resize(size_t n, const T& element = T()) {
        if (capacity_ < n) reserve(n);

        for (; end_ != arr_ + capacity_; ++end_) {
            construct_at(end_, element);
        }

        --end_; // to keep invariant
    }

    T& front()  {
        return *begin_;
    }

    const T& front() const {
        return *begin_;
    }

    T& back()  {
        return *(end_ - 1);
    }

    const T& back() const {
        return *(end_ - 1);
    }


};

template<typename T, typename Allocator>
std::ostream& operator<<(std::ostream& out, Buffer<T, Allocator>& v) {
    out << v.size() << '\n';
    for (auto i : v) {
        out << i << ' ';
    }

    return out;
}