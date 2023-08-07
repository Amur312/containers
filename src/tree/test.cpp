#include "RedBlackTree.h"
#include <gtest/gtest.h>

//TEST(RedBlackTreeTest, InsertAndSize) {
//  s21::RedBlackTree<int> tree;
//  EXPECT_TRUE(tree.empty());
//  EXPECT_EQ(tree.size(), 0);
//
//  tree.insert(5);
//  EXPECT_FALSE(tree.empty());
//  EXPECT_EQ(tree.size(), 1);
//
//  tree.insert(10);
//  EXPECT_EQ(tree.size(), 2);
//}

//TEST(RedBlackTreeTest, Find) {
//  s21::RedBlackTree<int> tree;
//  tree.insert(5);
//  tree.insert(10);
//
//  auto it = tree.find(5);
//  EXPECT_NE(it, tree.cEnd());
//  EXPECT_EQ(*it, 5);
//
//  it = tree.find(15);
//  EXPECT_EQ(it, tree.cEnd());
//}
//
//TEST(RedBlackTreeTest, Erase) {
//  s21::RedBlackTree<int> tree;
//  tree.insert(5);
//  tree.insert(10);
//
//  auto it = tree.find(5);
//  tree.erase(it);
//  EXPECT_EQ(tree.size(), 1);
//
//  it = tree.find(5);
//  EXPECT_EQ(it, tree.cEnd());
//}
//
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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
