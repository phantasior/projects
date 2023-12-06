#pragma once

#include <type_traits>
#include <iostream>
#include <utility>
#include <iterator>
#include <algorithm>
#include <exception>
#include <tuple>

namespace algo {

    template<typename... Containers>
    class UltimateZipper {
    public:
        UltimateZipper(const Containers&... containers)
                : min_size(std::min({containers.size()...}))
                , iterators_(std::make_tuple<typename Containers::const_iterator...>(containers.cbegin()...))
        {}

        std::tuple<typename Containers::value_type...> operator*() {
            return GetValues(std::make_index_sequence<sizeof...(Containers)>());
        }

        UltimateZipper& operator++() {
            cur_iteration++;
            std::apply([](auto&&... args) { (( args++ ), ...); }, iterators_);
            return *this;
        }

        UltimateZipper operator++(int) {
            auto copy = *this;
            ++(*this);
            return copy;
        }

        bool operator!=(const UltimateZipper& other) const {
            return cur_iteration != min_size;
        }

        UltimateZipper begin() const {
            return *this;
        }

        UltimateZipper end() const {
            return *this;
        }
    private:
        template<size_t... indexes>
        std::tuple<typename Containers::value_type...> GetValues(std::index_sequence<indexes...>) {
            return {*std::get<indexes>(iterators_)..., };
        }

        std::tuple<typename Containers::const_iterator...> iterators_;
        std::size_t cur_iteration = 0;
        std::size_t min_size;
    };

    template<typename First, typename Second>
    class ZipGenerator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::pair<typename First::value_type, typename Second::value_type>;
        using reference = value_type&;
        using pointer = value_type*;
        using difference_type = std::ptrdiff_t;

        value_type operator*() const {
            return {*first_cur_, *second_cur_};
        }

        ZipGenerator& operator++() {
            first_cur_++;
            second_cur_++;
            return *this;
        }

        ZipGenerator operator++(int) {
            ZipGenerator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const ZipGenerator& other) const {
            return first_cur_ == first_end_ && second_cur_ == second_end_;
        }

        bool operator!=(const ZipGenerator& other) const {
            return first_cur_ != first_end_ && second_cur_ != second_end_;
        }

    public:

        ZipGenerator(const First& first, const Second& second)
            : first_cur_(first.cbegin())
            , first_end_(first.cend())
            , second_cur_(second.cbegin())
            , second_end_(second.cend())
        {}

        ZipGenerator begin() const {
            return *this;
        }

        ZipGenerator end() const {
            return *this;
        }

    private:
        typename First::const_iterator first_cur_;
        typename Second::const_iterator second_cur_;
        const typename First::const_iterator first_end_;
        const typename Second::const_iterator second_end_;
    };

    template<typename First, typename Second>
    auto zip(const First& first, const Second& second) {
        return ZipGenerator(first, second);
    }

    template<typename T, typename Step>
    class RangeGenerator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;

        T& operator*() {
            return cur_;
        }

        T* operator->() const {
            return &cur_;
        }

        RangeGenerator& operator++() {
            cur_ = cur_ + step_;
            return *this;
        }

        RangeGenerator operator++(int) {
            RangeGenerator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const RangeGenerator& other) const {
            return step_ > 0 ? cur_ >= other.cur_ : cur_ <= other.cur_;
        }

        bool operator!=(const RangeGenerator& other) const {
            return step_ > 0 ? cur_ < end_ : cur_ > end_;
        }

        RangeGenerator(const T& begin, const T& end, const Step& step)
            : cur_(begin)
            , end_(end)
            , step_(step)
        {
            if ( (begin < end && step < 0) || (begin > end && step > 0) || step == 0) {
                throw std::invalid_argument("Invalid range");
            }
        }

        RangeGenerator begin() const {
            return *this;
        }

        RangeGenerator end() const {
            return *this;
        }

    private:
        T cur_;
        const T end_;
        Step step_;
    };

    template<typename T>
    auto range(const T& start, const T& end) {
        return RangeGenerator<T, int>(start, end, 1);
    }

    template<typename T>
    auto range(const T& end) {
        return RangeGenerator<T, int>(T(), end, 1);
    }

    template<typename T, typename Step>
    auto range(const T& start, const T& end, Step step) {
        return RangeGenerator<T, Step>(start, end, step);
    }


    template<typename InputIt, typename Func>
    inline constexpr bool all_of(InputIt first, InputIt last, Func func) {
        for (; first != last; ++first) {
            if (!func(*first)) {
                return false;
            }
        }

        return true;
    }

    template<typename InputIt, typename Func>
    inline constexpr bool any_of(InputIt first, InputIt last, Func func) {
        for (; first != last; ++first) {
            if (func(*first)) {
                return true;
            }
        }

        return false;
    }

    template<typename InputIt, typename Func>
    inline constexpr bool none_of(InputIt first, InputIt last, Func func) {
        for (; first != last; ++first) {
            if (func(*first)) {
                return false;
            }
        }

        return true;
    }

    template<typename InputIt, typename Func>
    inline constexpr bool one_of(InputIt first, InputIt last, Func func) {
        bool found = false;
        for (; first != last; ++first) {
            if (func(*first)) {
                if (found) {
                    return false;
                }
                {
                    found = true;
                }
            }
        }

        return found;
    }

    template<typename InputIt, typename Func>
    inline constexpr bool is_sorted(InputIt first, InputIt last, Func func) {
        InputIt next = first;
        next++;

        for (; first != last && next != last; ++first, ++next) {
            if (!func(*first, *next)) {
                return false;
            }
        }

        return true;
    }

    template<typename InputIt, typename Func>
    inline constexpr bool is_partitioned(InputIt first, InputIt last, Func func) {
        if (first == last) return true;
        bool changed = false;
        bool satisfy = func(*first);
        first++;

        for (; first != last; ++first) {
            if (!changed && func(*first) != satisfy) {
                changed = true;
            } else if (changed && func(*first) == satisfy) {
                return false;
            }
        }

        return true;
    }

    template<typename InputIt, typename T>
    inline constexpr InputIt find_not(InputIt first, InputIt last, const T &val) {
        for (; first != last; ++first) {
            if (*first != val) {
                return first;
            }
        }

        return last;
    }

    template<typename InputIt, typename T>
    inline constexpr InputIt find_backward(InputIt first, InputIt last, const T &val) {
        InputIt res = last;
        for (; first != last; first++) {
            if (*first == val) {
                res = first;
            }
        }

        return res;
    }


    template<typename InputIt, typename Func>
    inline constexpr bool is_palindrome(InputIt first, InputIt last, Func func) {
        if (first == last) {
            return true;
        }

        last--;
        while (first < last) {
            if (!func(*first, *last)) {
                return false;
            }

            first++;
            last--;
        }

        return true;
    }

}; // namespacet