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
  EXPECT_TRUE(s1.contains(4));
  EXPECT_TRUE(s2.empty());
}

TEST(SetTest, Swap) {
  s21::set<int> s1{1, 2, 3};
  s21::set<int> s2{4, 5, 6};
  s1.swap(s2);
  EXPECT_EQ(s1.size(), 3);
  EXPECT_EQ(*s1.find(4), 4);
  EXPECT_EQ(s2.size(), 3);
  EXPECT_EQ(*s2.find(1), 1);
}

TEST(SetTest, Clear) {
  s21::set<int> s{1, 2, 3};
  s.clear();
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(SetTest, Emplace) {
  s21::set<int> s;
  auto results = s.emplace(42);
  EXPECT_EQ(results.size(), 1);
  EXPECT_TRUE(results[0].second);
  EXPECT_EQ(*results[0].first, 42);
  EXPECT_EQ(s.size(), 1);
}

TEST(SetTest, MergeEmptyTrees) {
  s21::set<int> s1;
  s21::set<int> s2;
  s1.merge(s2);
  EXPECT_TRUE(s1.empty());
  EXPECT_TRUE(s2.empty());
}

TEST(SetTest, MergeWithSelf) {
  s21::set<int> s1{1, 2, 3};
  s1.merge(s1);
  EXPECT_EQ(s1.size(), 3);
}

TEST(SetTest, ContainsMethod) {
  s21::set<int> s{1, 2, 3};
  EXPECT_TRUE(s.contains(2));
  EXPECT_FALSE(s.contains(4));
}

TEST(SetTest, MaxSizeMethod) {
  s21::set<int> s;
  EXPECT_GT(s.max_size(), 0);
}

TEST(SetTest, CopyAssignment) {
  s21::set<int> original{1, 2, 3};
  s21::set<int> copy;
  copy = original;
  EXPECT_EQ(copy.size(), 3);
  EXPECT_EQ(*copy.find(2), 2);
}

TEST(SetTest, MoveAssignment) {
  s21::set<int> original{1, 2, 3};
  s21::set<int> moved;
  moved = std::move(original);
  EXPECT_EQ(moved.size(), 3);
  EXPECT_TRUE(original.empty());
}
TEST(SetTest, BeginEndIterators) {
  s21::set<int> s{1, 2, 3};
  auto it = s.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
  ++it;
  EXPECT_EQ(it, s.end());
}

TEST(SetTest, EmplaceWithDuplicate) {
  s21::set<int> s;
  auto results1 = s.emplace(42);
  auto results2 = s.emplace(42);
  EXPECT_EQ(results1.size(), 1);
  EXPECT_TRUE(results1[0].second);
  EXPECT_EQ(results2.size(), 1);
  EXPECT_FALSE(results2[0].second);
  EXPECT_EQ(s.size(), 1);
}

TEST(SetTest, MergeWithEmptySet) {
  s21::set<int> s1{1, 2, 3};
  s21::set<int> s2;
  s1.merge(s2);
  EXPECT_EQ(s1.size(), 3);
  EXPECT_TRUE(s2.empty());
}

TEST(SetTest, CompareSets) {
  s21::set<int> s1{1, 2, 3};
  s21::set<int> s2{1, 2, 3};
  s21::set<int> s3{4, 5, 6};
  EXPECT_TRUE(s1 == s2);
  EXPECT_FALSE(s1 == s3);
}

TEST(SetTest, FindNonExistentElement) {
  s21::set<int> s{1, 2, 3};
  EXPECT_EQ(s.find(42), s.end());
}
TEST(SetTest, CountMethod) {
  s21::set<int> my_set;
  my_set.insert(5);
  my_set.insert(10);
  EXPECT_EQ(my_set.count(5), 1);
  EXPECT_EQ(my_set.count(3), 0);
}
TEST(SetTest, InequalityOperator) {
  s21::set<int> set1 = {1, 2, 3};
  s21::set<int> set2 = {1, 2, 3};
  s21::set<int> set3 = {1, 2, 4};

  EXPECT_FALSE(set1 != set2);
  EXPECT_TRUE(set1 != set3);
}

TEST(SetTest, EraseByKey) {
  s21::set<int> set1 = {1, 2, 3};

  EXPECT_EQ(set1.erase(2), 1);
  EXPECT_EQ(set1.count(2), 0);

  EXPECT_EQ(set1.erase(4), 0);
  EXPECT_EQ(set1.count(4), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
