#include <gtest/gtest.h>
#include "s21_list.h"

TEST(ListTest, PushBackAndFront) {
    s21::list<int> lst;
    lst.push_back(1);
    lst.push_front(2);
    EXPECT_EQ(lst.front(), 2);
    EXPECT_EQ(lst.back(), 1);
}

TEST(ListTest, PopBackAndFront) {
    s21::list<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.pop_front();
    EXPECT_EQ(lst.front(), 2);
    lst.pop_back();
    EXPECT_TRUE(lst.empty());
}

TEST(ListTest, Iterator) {
    s21::list<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    auto it = lst.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    --it;
    EXPECT_EQ(*it, 1);
}

TEST(ListTest, ReverseIterator) {
    s21::list<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    auto it = lst.rbegin();
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(*it, 2);
    --it;
    EXPECT_EQ(*it, 3);
}


