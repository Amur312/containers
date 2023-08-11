#include "../tree/RedBlackTree.h"
#include "s21_set.h"
#include <gtest/gtest.h>

TEST(SetTest, DefaultConstructor) {
  s21::set<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(SetTest, InitializerListConstructor) {
  s21::set<int> s{1, 2, 3, 4};
  EXPECT_FALSE(s.empty());
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(*s.find(1), 1);
}

TEST(SetTest, CopyConstructor) {
  s21::set<int> original{1, 2, 3};
  s21::set<int> copy(original);
  EXPECT_EQ(copy.size(), 3);
  EXPECT_EQ(*copy.find(2), 2);
}

TEST(SetTest, MoveConstructor) {
  s21::set<int> original{1, 2, 3};
  s21::set<int> moved(std::move(original));
  EXPECT_EQ(moved.size(), 3);
  EXPECT_TRUE(original.empty());
}

TEST(SetTest, Insert) {
  s21::set<int> s;
  auto [it, success] = s.insert(42);
  EXPECT_TRUE(success);
  EXPECT_EQ(*it, 42);
  EXPECT_EQ(s.size(), 1);

  auto [it2, success2] = s.insert(42);
  EXPECT_FALSE(success2);
  EXPECT_EQ(s.size(), 1);
}

TEST(SetTest, Erase) {
  s21::set<int> s{1, 2, 3};
  auto it = s.find(2);
  s.erase(it);
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(s.find(2), s.end());
}

TEST(SetTest, Merge) {
  s21::set<int> s1{1, 2, 3};
  s21::set<int> s2{3, 4, 5};
  s1.merge(s2);
  EXPECT_EQ(s1.size(), 5);
  EXPECT_EQ(s2.size(), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
