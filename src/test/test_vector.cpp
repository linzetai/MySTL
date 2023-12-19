#include <gtest/gtest.h>
#include "../vector.h"

TEST(VectorTest, Initialize) {
    vector<int> v;
    EXPECT_EQ(0, v.size());
    EXPECT_EQ(8, v.capacity());
}

TEST(VectorTest, PushBack) {
    vector<int> v;
    EXPECT_TRUE(v.empty());
    v.push_back(1);
    EXPECT_EQ(1, v.size());
    EXPECT_FALSE(v.empty());
}

TEST(VectorTest, ElementAccess) {
    vector<int> v;
    for (size_t i = 0; i < 10; ++i) {
        v.push_back(i);
        EXPECT_EQ(i, v.at(i));
        EXPECT_EQ(i, v[i]);
    }
    EXPECT_EQ(v.front(), 0);
    EXPECT_EQ(v.back(), 9);
    EXPECT_EQ(*v.data(), 0);
}

TEST(VectorTest, Reserve) {
    vector<int> v;
    v.reserve(24);
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 24);
}

TEST(VectorTest, Resize) {
    vector<int> v;
    for (size_t i = 0; i < 3; ++i)
        v.push_back(i);
    v.resize(5);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[1], 1);
    EXPECT_EQ(v[2], 2);
    EXPECT_EQ(v[3], 0);
    EXPECT_EQ(v[4], 0);

    v.resize(7, 2);
    EXPECT_EQ(v[5], 2);
    EXPECT_EQ(v[6], 2);
}

TEST(VectorTest, Iterators) {
    vector<int> v;
    EXPECT_EQ(v.begin(), v.end());
    EXPECT_EQ(v.cbegin(), v.cend());
    EXPECT_EQ(v.rbegin(), v.rend());
    EXPECT_EQ(v.crbegin(), v.crend());
    for (size_t i = 0; i < 10; ++i)
        v.push_back(i);
    int i = 0;
    for (auto iter = v.begin(); iter != v.end(); ++iter, ++i)
        EXPECT_EQ(*iter, i);
    i = 0;
    for (auto iter = v.begin(); iter != v.end(); iter++, ++i)
        EXPECT_EQ(*iter, i);
    i = 0;
    for (auto iter = v.cbegin(); iter != v.cend(); iter++, ++i)
        EXPECT_EQ(*iter, i);
    i = 9;
    for (auto iter = v.rbegin(); iter != v.rend(); ++iter, --i)
        EXPECT_EQ(*iter, i);
    i = 9;
    for (auto iter = v.rbegin(); iter != v.rend(); iter++, --i)
        EXPECT_EQ(*iter, i);
    i = 9;
    for (auto iter = v.crbegin(); iter != v.crend(); iter++, --i)
        EXPECT_EQ(*iter, i);
}

TEST(VectorTest, PopBack) {
    vector<int> v;
    v.push_back(0);
    v.push_back(1);
    EXPECT_EQ(v.size(), 2);
    v.pop_back();
    EXPECT_EQ(v.size(), 1);
    EXPECT_ANY_THROW(v.at(1));
}

TEST(VectorTest, EmplaceBack) {
    struct _Test {
        int a;
        double b;
        char c;
        _Test(int a, double b, char c) : a(a), b(b), c(c){};
        _Test() = default;
    };
    vector<_Test> v;
    v.emplace_back(1, 2, 3);
    EXPECT_EQ(v[0].a, 1);
    EXPECT_EQ(v[0].b, 2);
    EXPECT_EQ(v[0].c, 3);
}

TEST(VectorTest, Insert) {
    vector<int> v;
    for (size_t i = 0; i < 6; ++i)
        v.push_back(i);
    v.insert(1, 2, 3);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[3], 1);
}