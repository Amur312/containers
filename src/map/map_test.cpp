
#include "../tree/RedBlackTree.h"
#include "s21_map.h"
#include <gtest/gtest.h>

namespace s21 {
TEST(MapTest, EmptyMap) {
  s21::map<int, std::string> m;
  EXPECT_TRUE(m.empty());
  EXPECT_EQ(m.size(), 0);
}

TEST(MapTest, InsertSingleElement) {
  s21::map<int, std::string> m;
  m.insert(std::make_pair(1, "one"));
  EXPECT_FALSE(m.empty());
  EXPECT_EQ(m.size(), 1);
  EXPECT_EQ(m[1], "one");
}

TEST(MapTest, InsertAndRetrieveMultipleElements) {
  s21::map<int, std::string> m;
  m.insert(std::make_pair(1, "one"));
  m.insert(std::make_pair(2, "two"));
  EXPECT_EQ(m.size(), 2);
  EXPECT_EQ(m[1], "one");
  EXPECT_EQ(m[2], "two");
}

TEST(MapTest, CopyConstructor) {
  s21::map<int, std::string> m1;
  m1[1] = "one";
  s21::map<int, std::string> m2(m1);
  EXPECT_EQ(m1[1], m2[1]);
}

TEST(MapTest, AtNonExistentKey) {
  s21::map<int, std::string> m;
  EXPECT_THROW(m.at(1), std::out_of_range);
}

TEST(MapTest, InsertOrAssign) {
  s21::map<int, std::string> m;
  m.insert_or_assign(1, "one");
  EXPECT_EQ(m[1], "one");
  m.insert_or_assign(1, "new_one");
  EXPECT_EQ(m[1], "new_one");
}

TEST(MapTest, EraseElement) {
  s21::map<int, std::string> m;
  m.insert(std::make_pair(1, "one"));
  auto it = m.begin();
  m.erase(it);
  EXPECT_TRUE(m.empty());
  EXPECT_EQ(m.size(), 0);
}

TEST(MapTest, MoveConstructor) {
  s21::map<int, std::string> m1;
  m1[1] = "one";
  s21::map<int, std::string> m2(std::move(m1));
  EXPECT_EQ(m2[1], "one");
  EXPECT_TRUE(m1.empty());
}
TEST(MapTest, SwapMaps) {
  s21::map<int, std::string> m1;
  m1[1] = "one";
  s21::map<int, std::string> m2;
  m2[2] = "two";
  m1.swap(m2);
  EXPECT_EQ(m1[2], "two");
  EXPECT_EQ(m2[1], "one");
}
TEST(MapTest, MergeMaps) {
  s21::map<int, std::string> m1;
  m1[1] = "one";
  s21::map<int, std::string> m2;
  m2[2] = "two";
  m1.merge(m2);
  EXPECT_EQ(m1[1], "one");
  EXPECT_EQ(m1[2], "two");
  EXPECT_TRUE(m2.empty());
}


TEST(MapTest, IteratorTraversal) {
  s21::map<int, std::string> m;
  m.insert(std::make_pair(1, "one"));
  m.insert(std::make_pair(2, "two"));
  m.insert(std::make_pair(3, "three"));

  auto it = m.begin();
  EXPECT_EQ((*it).first, 1);
  EXPECT_EQ((*it).second, "one");
  ++it;
  EXPECT_EQ((*it).first, 2);
  EXPECT_EQ((*it).second, "two");
  ++it;
  EXPECT_EQ((*it).first, 3);
  EXPECT_EQ((*it).second, "three");
}



TEST(RedBlackTreeTest, UpdateExtremes) {
  s21::RedBlackTree<int, std::less<int>> tree;

  tree.Insert(10);
  tree.Insert(5);
  tree.Insert(20);

  // Используйте правильное имя поля
  EXPECT_EQ(tree.GetLeftmostNode()->key_, 5);
  EXPECT_EQ(tree.GetRightmostNode()->key_, 20);

  tree.Insert(1);
  tree.Insert(25);

  EXPECT_EQ(tree.GetLeftmostNode()->key_, 1);
  EXPECT_EQ(tree.GetRightmostNode()->key_, 25);
}










int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
} // namespace s21