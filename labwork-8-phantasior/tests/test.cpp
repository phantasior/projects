#include "../lib/StaticBuffer.hpp"
#include "../lib/DynamicBuffer.hpp"

#include <gtest/gtest.h>
#include <algorithm>

constexpr int size = 5;

TEST(StaticBuffer, Reserve) {
    StaticBuffer<int> v(size);
    ASSERT_EQ(v.capacity(), size);
    v.reserve(4 * size);
    ASSERT_EQ(v.capacity(), 4 * size);
}

TEST(StaticBuffer, Empty) {
    StaticBuffer<int> v(size);
    v.push_front(3);
    v.push_back(4);
    v.pop_back();
    v.pop_back();
    ASSERT_TRUE(v.empty());
}

TEST(StaticBuffer, PushBack) {
    StaticBuffer<int> v(size);
    for (int i = 0; i < size; ++i) {
        v.push_back(i);
    }

    auto it = v.begin();
    for (int i = 0; it != v.end(); ++it, ++i) {
        ASSERT_EQ(*it, i);
    }
}

TEST(StaticBuffer, CyclePushBack) {
    StaticBuffer<int> v(size);
    for (int i = 0; i < 2 * size; ++i) {
        v.push_back(i);
    }

    auto it = v.begin();
    for (int i = 0; it != v.end(); ++it, ++i) {
        ASSERT_EQ(*it, i + size);
    }
}

TEST(StaticBuffer, CyclePopBack) {
    StaticBuffer<int> v(size);
    for (int i = 0; i < 2 * size; ++i) {
        v.push_back(i);
    }

    v.pop_back();
    ASSERT_EQ(v.size(), size - 1);

    auto it = v.begin();
    for (int i = 0; it != v.end(); ++it, ++i) {
        ASSERT_EQ(*it, i + size);
    }
}

TEST(StaticBuffer, CyclePushFront) {
    StaticBuffer<int> v(size);
    for (int i = 0; i < size; ++i) {
        v.push_front(i);
    }

    auto it = v.begin();
    for (int i = 0; it != v.end(); ++it, ++i) {
        ASSERT_EQ(*it, size - 1 - i);
    }
}

TEST(StaticBuffer, CyclePopFront) {
    StaticBuffer<int> v(size);
    for (int i = 0; i < 2 * size; ++i) {
        v.push_front(i + 1);
    }

    v.pop_front();

    ASSERT_EQ(v.size(), size - 1);
    auto it = v.begin();
    for (int i = 2 * size - 1; it != v.end(); ++it, --i) {
        ASSERT_EQ(*it, i);
    }
}

TEST(StaticBuffer, AssignmentConstructor) {
    StaticBuffer<int> v(size);
    for (int i = 0; i < size; ++i) {
        v.push_back(i + 1);
    }

    StaticBuffer<int> u;
    u = v;
    *v.begin() = 13;

    auto it = u.begin();
    for (int i = 1; it != u.end(); ++it, ++i) {
        ASSERT_EQ(*it, i);
    }
}

TEST(StaticBuffer, Swap) {
    StaticBuffer<int> v(size);
    for (int i = 0; i < size; ++i) {
        v.push_back(i + 1);
    }

    int ptr = size;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        ASSERT_EQ(*it, ptr--);
    }
}

TEST(StaticBuffer, Resize) {
    StaticBuffer<int> v(3);
    for (int i = 0; i < 3; ++i) {
        v.push_back(i + 1);
    }

    v.resize(5);
    ASSERT_EQ(v.size(), 5);
    v.push_back(13);
    ASSERT_EQ(v.size(), 5);
    
    int expected[5] = {2, 3, 0, 0, 13};
    auto it = v.begin();
    for (int i = 0; it != v.end(); ++it, ++i) {
        ASSERT_EQ(*it, expected[i]);
    }   
}

TEST(DynamicBuffer, PushBackWithoutExtending) {
    DynamicBuffer<int> v(size);
    for (int i = 0; i < size; ++i) {
        v.push_back(i);
    }

    auto it = v.begin();
    for (int i = 0; it != v.end(); ++it, ++i) {
        ASSERT_EQ(*it, i);
    }
}

TEST(DynamicBuffer, PushBack) {
    DynamicBuffer<int> v(size);
    for (int i = 0; i < 2 * size; ++i) {
        v.push_back(i);
    }

    auto it = v.begin();
    for (int i = 0; it != v.end(); ++it, ++i) {
        ASSERT_EQ(*it, i);
    }
}

TEST(DynamicBuffer, PopBack) {
    DynamicBuffer<int> v(size);
    for (int i = 0; i < 2 * size; ++i) {
        v.push_back(i);
    }

    v.pop_back();
    ASSERT_EQ(v.size(), 2 * size - 1);

    auto it = v.begin();
    for (int i = 0; it != v.end(); ++it, ++i) {
        ASSERT_EQ(*it, i);
    }
}

TEST(DynamicBuffer, PushFront) {
    DynamicBuffer<int> v(size);
    for (int i = 0; i < size; ++i) {
        v.push_front(i);
    }

    auto it = v.begin();
    for (int i = 0; it != v.end(); ++it, ++i) {
        ASSERT_EQ(*it, 4 - i);
    }
}


TEST(RandomAccessIterator, BasicOperations) {
    DynamicBuffer<int> v(size);
    for (int i = 0; i < 2 * size; ++i) {
        v.push_back(2 * size - i);
    }
    auto begin = v.begin();
    auto end = v.end();

    ASSERT_EQ(*begin, 10);
    ASSERT_EQ(*(end - 1), 1);
    ASSERT_EQ(end - begin, 10);
    ASSERT_EQ(*(begin + 3), 7);
    ASSERT_EQ(*(end - 4), 4);

    ASSERT_TRUE(begin < end);
    ASSERT_FALSE(begin > end);
    ASSERT_FALSE(begin < begin);
    ASSERT_FALSE(begin > begin);

    ASSERT_TRUE(begin <= begin);
    ASSERT_TRUE(begin >= begin);
}

TEST(Algorithms, Sort) {
    DynamicBuffer<int> v(size);
    for (int i = 1; i <= 2 * size; ++i) {
        v.push_back(i);
    }

    std::sort(v.begin(), v.end(), std::greater<int>());
    auto begin  = v.begin();
    for (int i = 2 * size; i > 0; --i, begin++) {
        ASSERT_EQ(*begin, i);
    }
}

TEST(Algorithms, Transform) {
    DynamicBuffer<int> v(size);
    for (int i = 1; i <= 2 * size; ++i) {
        v.push_back(i);
    }

    std::transform(v.begin(), v.end(), v.begin(), [](int k) { return k * k; });

    auto begin = v.begin();
    for (int i = 1; i <= 2 * size; i++, begin++) {
        ASSERT_EQ(*begin, i * i);
    }
}