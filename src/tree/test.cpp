#include "RedBlackTree.h"
#include <gtest/gtest.h>

TEST(RedBlackTreeTest, InsertAndSize) {
  s21::RedBlackTree<int> tree;
  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(tree.size(), 0);
  tree.insert(5);
  EXPECT_FALSE(tree.empty());
  EXPECT_EQ(tree.size(), 1);

  tree.insert(10);
  EXPECT_EQ(tree.size(), 2);
}

TEST(RedBlackTreeTest, Find) {
  s21::RedBlackTree<int> tree;
  tree.insert(5);
  tree.insert(10);

  auto it = tree.find(5);
  EXPECT_NE(it, tree.cEnd());
  EXPECT_EQ(*it, 5);

  it = tree.find(15);
  EXPECT_EQ(it, tree.cEnd());
}

TEST(RedBlackTreeTest, Erase) {
  s21::RedBlackTree<int> tree;
  tree.insert(5);
  tree.insert(10);

  auto it = tree.find(5);
  tree.erase(it);
  EXPECT_EQ(tree.size(), 1);

  it = tree.find(5);
  EXPECT_EQ(it, tree.cEnd());
}

TEST(RedBlackTreeTest, Iterator) {
  s21::RedBlackTree<int> tree;
  tree.insert(5);
  tree.insert(10);

  auto it = tree.cBegin();
  EXPECT_EQ(*it, 5);
  ++it;
  EXPECT_EQ(*it, 10);
  --it;
  EXPECT_EQ(*it, 5);
}

TEST(RedBlackTreeTest, Merge) {
  s21::RedBlackTree<int> tree1;
  tree1.insert(5);
  tree1.insert(10);

  s21::RedBlackTree<int> tree2;
  tree2.insert(15);
  tree2.insert(20);

  tree1.merge(tree2);

  EXPECT_EQ(tree1.size(), 4);
  EXPECT_EQ(tree2.size(), 0);
  EXPECT_EQ(*tree1.find(5), 5);
  EXPECT_EQ(*tree1.find(10), 10);
  EXPECT_EQ(*tree1.find(15), 15);
  EXPECT_EQ(*tree1.find(20), 20);
}

TEST(RedBlackTreeTest, UpperBound) {
  s21::RedBlackTree<int> tree;
  tree.insert(5);
  tree.insert(10);
  tree.insert(15);

  auto it = tree.upperBound(7);
  EXPECT_NE(it, tree.cEnd());
  EXPECT_EQ(*it, 10);

  it = tree.upperBound(15);
  EXPECT_EQ(it, tree.cEnd());
}

TEST(RedBlackTreeTest, Clear) {
  s21::RedBlackTree<int> tree;
  tree.insert(5);
  tree.insert(10);
  tree.insert(15);

  EXPECT_EQ(tree.size(), 3);
  tree.clear();
  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(tree.size(), 0);

  auto it = tree.find(5);
  EXPECT_EQ(it, tree.cEnd());
}
TEST(RedBlackTreeTest, InsertUnique_UniqueKey) {
  s21::RedBlackTree<int> tree;
  tree.insert(5);

  auto result = tree.insertUnique(10);
  EXPECT_TRUE(result.second);
  EXPECT_EQ(*result.first, 10);
  EXPECT_EQ(tree.size(), 2);
}

TEST(RedBlackTreeTest, InsertUnique_NonUniqueKey) {
  s21::RedBlackTree<int> tree;
  tree.insert(5);
  tree.insert(10);

  auto result = tree.insertUnique(10);
  EXPECT_FALSE(result.second);
  EXPECT_EQ(*result.first, 10);
  EXPECT_EQ(tree.size(), 2);
}

TEST(RedBlackTreeTest, OrderedIteration) {
  s21::RedBlackTree<int> tree;
  for (int i = 0; i < 10; ++i) {
    tree.insert(i);
  }

  int expectedValue = 0;
  for (auto it = tree.cBegin(); it != tree.cEnd(); ++it) {
    EXPECT_EQ(*it, expectedValue);
    ++expectedValue;
  }
}

TEST(RedBlackTreeTest, InsertDuplicateKeys) {
  s21::RedBlackTree<int> tree;
  tree.insert(5);
  tree.insert(5);
  tree.insert(5);
  EXPECT_EQ(tree.size(), 3);
}
TEST(RedBlackTreeTest, InsertInReverseOrder) {
  s21::RedBlackTree<int> tree;
  for (int i = 9; i >= 0; --i) {
    tree.insert(i);
  }

  int expectedValue = 0;
  for (auto it = tree.cBegin(); it != tree.cEnd(); ++it) {
    EXPECT_EQ(*it, expectedValue);
    ++expectedValue;
  }
}

TEST(RedBlackTreeTest, EraseNonExistentKey) {
  s21::RedBlackTree<int> tree;
  tree.insert(5);
  tree.insert(10);

  auto it = tree.find(15);
  tree.erase(it);

  EXPECT_EQ(tree.size(), 2);
  EXPECT_EQ(*tree.find(5), 5);
  EXPECT_EQ(*tree.find(10), 10);
}

TEST(RedBlackTreeTest, MergeWithEmptyTree) {
  s21::RedBlackTree<int> tree1;
  tree1.insert(5);
  tree1.insert(10);

  s21::RedBlackTree<int> tree2;

  tree1.merge(tree2);

  EXPECT_EQ(tree1.size(), 2);
  EXPECT_EQ(tree2.size(), 0);
}

TEST(RedBlackTreeTest, LowerBound) {
  s21::RedBlackTree<int> tree;
  tree.insert(5);
  tree.insert(10);
  tree.insert(15);

  auto it = tree.lowerBound(7);
  EXPECT_NE(it, tree.cEnd());
  EXPECT_EQ(*it, 10);

  it = tree.lowerBound(5);
  EXPECT_EQ(*it, 5);
}

TEST(RedBlackTreeTest, EraseAndBalance) {
  s21::RedBlackTree<int> tree;
  tree.insert(5);
  tree.insert(10);
  tree.insert(15);
  tree.insert(20);
  tree.insert(25);
  auto it = tree.find(15);
  tree.erase(it);
  EXPECT_EQ(tree.size(), 4);
  EXPECT_EQ(*tree.find(5), 5);
  EXPECT_EQ(*tree.find(10), 10);
  EXPECT_EQ(*tree.find(20), 20);
  EXPECT_EQ(*tree.find(25), 25);

  it = tree.find(15);
  EXPECT_EQ(it, tree.cEnd());
}



int main(int argc, char **argv) {

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
