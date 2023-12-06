#include "../lib/algo.hpp"

#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include <list>
#include <string>

TEST(all_of, odd) {
    std::vector<int> v{1, 3, 5, 7, 9};
    ASSERT_TRUE(algo::all_of(v.begin(), v.end(), [](int i) { return i % 2 == 1; }));
    ASSERT_FALSE(algo::all_of(v.begin(), v.end(), [](int i) { return i % 2 == 0; }));
}

TEST(all_of, even) {
    std::vector<int> v{2, 4, 6, 8, 10};
    ASSERT_FALSE(algo::all_of(v.begin(), v.end(), [](int i) { return i % 2 == 1; }));
    ASSERT_TRUE(algo::all_of(v.begin(), v.end(), [](int i) { return i % 2 == 0; }));
}

TEST(any_of, odd) {
    std::vector<int> v{2, 4, 6, 8, 10, 11};
    ASSERT_TRUE(algo::any_of(v.begin(), v.end(), [](int i) { return i % 2 == 1; }));
    ASSERT_TRUE(algo::any_of(v.begin(), v.end(), [](int i) { return i % 2 == 0; }));
    ASSERT_FALSE(algo::any_of(v.begin(), v.end(), [](int i) { return i <= 0 || i >= 12; }));
}

TEST(any_of, even) {
    std::vector<int> v{1, 3, 5, 7, 9, 10};
    ASSERT_TRUE(algo::any_of(v.begin(), v.end(), [](int i) { return i % 2 == 0; }));
    ASSERT_TRUE(algo::any_of(v.begin(), v.end(), [](int i) { return i % 2 == 1; }));
    ASSERT_FALSE(algo::any_of(v.begin(), v.end(), [](int i) { return i <= 0 || i >= 11; }));
}
TEST(none_of, odd) {
    std::vector<int> v{2, 4, 6, 8, 10};
    ASSERT_TRUE(algo::none_of(v.begin(), v.end(), [](int i) { return i % 2 == 1; }));
    ASSERT_FALSE(algo::none_of(v.begin(), v.end(), [](int i) { return i % 2 == 0; }));
}

TEST(none_of, even) {
    std::vector<int> v{1, 3, 5, 7, 9};
    ASSERT_TRUE(algo::none_of(v.begin(), v.end(), [](int i) { return i % 2 == 0; }));
    ASSERT_FALSE(algo::none_of(v.begin(), v.end(), [](int i) { return i % 2 == 1; }));
}

TEST(one_of, basic_test) {
    std::vector<int> v{1, 3, 5, 7, 9, 10};
    ASSERT_TRUE(algo::one_of(v.begin(), v.end(), [](int i) { return i == 5; }));
    ASSERT_TRUE(algo::one_of(v.begin(), v.end(), [](int i) { return i == 1 || i == 2; }));
    ASSERT_FALSE(algo::one_of(v.begin(), v.end(), [](int i) { return i == 11; }));
}

TEST(one_of, mod_10) {
    std::vector<int> v{10, 21, 1235, 6, 7, 7};
    ASSERT_TRUE(algo::is_sorted(v.begin(), v.end(), [](int l, int r) { return (l % 10) <= (r % 10); }));
    ASSERT_FALSE(algo::is_sorted(v.begin(), v.end(), [](int l, int r) { return (l % 10) < (r % 10); }));
    ASSERT_FALSE(algo::is_sorted(v.begin(), v.end(), [](int l, int r) { return l < r; }));
}

TEST(is_partitioned, even_odd) {
    std::vector<int> v{1, 3, 5, 2, 4, 6};
    ASSERT_TRUE(algo::is_partitioned(v.begin(), v.end(), [](int l) { return l % 2 == 0; }));
    ASSERT_TRUE(algo::is_partitioned(v.begin(), v.end(), [](int l) { return l % 2 == 1; }));
    ASSERT_FALSE(algo::is_partitioned(v.begin(), v.end(), [](int l) { return l % 3; }));
    ASSERT_TRUE(algo::is_partitioned(v.begin(), v.end(), [](int l) { return l == 1 || l == 3 || l == 5; }));
}

TEST(find_not, find_not_5) {
    std::vector<int> v{5, 5, 5, 2, 5, 5, 7};
    ASSERT_EQ(algo::find_not(v.begin(), v.end(), 5), std::find(v.begin(), v.end(), 2));
    ASSERT_EQ(algo::find_not(v.begin(), v.end(), 2), std::find(v.begin(), v.end(), 5));
}

TEST(find_backward, basic_test) {
    std::vector<int> v{1, 5, 3, 10, 7, 123, 321};
    ASSERT_EQ(algo::find_backward(v.begin(), v.end(), 10), std::find(v.begin(), v.end(), 10));
    ASSERT_EQ(algo::find_backward(v.begin(), v.end(), 321), std::find(v.begin(), v.end(), 321));
    ASSERT_EQ(algo::find_backward(v.begin(), v.end(), 1), std::find(v.begin(), v.end(), 1));
    ASSERT_EQ(algo::find_backward(v.begin(), v.end(), 13), std::find(v.begin(), v.end(), 13));
}

TEST(is_palindrome, string_test) {
    std::string s = "abcccccba";
    ASSERT_TRUE(algo::is_palindrome(s.begin(), s.end(), [](char a, char b) { return a == b; }));
    ASSERT_TRUE(algo::is_palindrome(s.begin() + 1, s.end() - 1, [](char a, char b) { return a == b; }));
    ASSERT_FALSE(algo::is_palindrome(s.begin() + 1, s.end(), [](char a, char b) { return a == b; }));
}

TEST(range, basiс_test) {
    int total = 0;
    for (int i : algo::range(1, 10, 2)) {
        for (int j : algo::range(5)) {
            total += i * j;
        }
    }

    ASSERT_EQ(total, 250);
}

TEST(range, range_from_vector) {
    std::vector<int> v{1,3,5,7,9};
    std::vector<int> got;
    std::vector<int> expected{1,5,9};
    for (auto i : algo::range(v.begin(), v.end(), 2)) {
        got.push_back(*i);
    }

    ASSERT_EQ(got, expected);
}

TEST(range, string_range) {
    std::vector<std::string> got;
    std::vector<std::string> expected{
        "abc",
        "abcd",
        "abcdd",
        "abcddd",
        "abcdddd"
    };

    for (auto i : algo::range(std::string("abc"), std::string("abcddddd"), 'd')) {
        got.push_back(i);
    }

    ASSERT_EQ(got, expected);
}

TEST(range, backward_range) {
    int cur = 100;
    for (auto i : algo::range(100, 10, -3)) {
        ASSERT_EQ(i, cur);
        cur -= 3;
    }
}

TEST(range, vector_from_range) {
    auto x = algo::range(10, 17);
    std::vector<int> v{x.begin(), x.end()};
    std::vector<int> expected{10, 11, 12, 13, 14, 15, 16};

    ASSERT_EQ(v, expected);
}

TEST(zip, vector_from_zip) {
    std::list<int> l = {1, 2, 3, 4, 5};
    std::vector<char> v = {'a', 'b', 'c', 'd'};

    auto x = algo::zip(l, v);
    std::vector<std::pair<int, char>> res{x.begin(), x.end()};
    std::vector<std::pair<int, char>> expected{
            {1, 'a'},
            {2, 'b'},
            {3, 'c'},
            {4, 'd'}
    };

    ASSERT_EQ(res, expected);
}

TEST(Ultimate_zipper, zipper_from_three_containers) {
    std::vector<char> v1 = {'a', 'b', 'c', 'd', 'e'};
    std::vector<int> v2 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<double> v3 = {1.1, 2.2, 3.3, 4.4, 123.3};
    auto zipped = algo::UltimateZipper(v1, v2, v3);
    std::vector<std::tuple<char, int, double>> got;
    std::vector<std::tuple<char, int, double>> expected = {
            {'a', 1, 1.1},
            {'b', 2, 2.2},
            {'c', 3, 3.3},
            {'d', 4, 4.4},
            {'e', 5, 123.3}
    };

    for(const auto& i: zipped) {
        got.emplace_back(i);
    }

    ASSERT_EQ(got, expected);
}



