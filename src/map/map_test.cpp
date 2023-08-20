
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

TEST(MapTest, AssignmentOperator) {
  s21::map<int, std::string> m1;
  m1[1] = "one";
  s21::map<int, std::string> m2;
  m2 = m1;
  EXPECT_EQ(m2[1], "one");
}

TEST(MapTest, InitializerListConstructor) {
  s21::map<int, std::string> m{{1, "one"}, {2, "two"}};
  EXPECT_EQ(m[1], "one");
  EXPECT_EQ(m[2], "two");
  EXPECT_EQ(m.size(), 2);
}

TEST(MapTest, ContainsMethod) {
  s21::map<int, std::string> m;
  m[1] = "one";
  EXPECT_TRUE(m.contains(1));
  EXPECT_FALSE(m.contains(2));
}

TEST(MapTest, EmptyMapAfterClear) {
  s21::map<int, std::string> m;
  m.insert(std::make_pair(1, "one"));
  m.clear();
  EXPECT_TRUE(m.empty());
  EXPECT_EQ(m.size(), 0);
}
TEST(MapTest, InsertDuplicateKey) {
  s21::map<int, std::string> m;
  m.insert(std::make_pair(1, "one"));
  m.insert(std::make_pair(1, "duplicate"));
  EXPECT_EQ(m.size(), 1);
  EXPECT_EQ(m[1], "one");
}

TEST(MapTest, EraseSingleElement) {
  s21::map<int, std::string> m;
  m.insert(std::make_pair(1, "one"));
  m.erase(m.begin());
  EXPECT_TRUE(m.empty());
  EXPECT_EQ(m.size(), 0);
}

TEST(MapTest, MergeDuplicateKeys) {
  s21::map<int, std::string> m1{{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2{{2, "duplicate"}, {3, "three"}};
  m1.merge(m2);
  EXPECT_EQ(m1.size(), 3);
  EXPECT_EQ(m1[2], "two");
}
TEST(MapTest, EmplaceMethod) {
  s21::map<int, std::string> m;
  auto result = m.emplace(1, "one");
  EXPECT_TRUE(result.second);
  EXPECT_EQ(m.size(), 1);
  EXPECT_EQ(m[1], "one");

  result = m.emplace(1, "duplicate");
  EXPECT_FALSE(result.second);
  EXPECT_EQ(m.size(), 1);
  EXPECT_EQ(m[1], "one");
}

TEST(MapTest, InsertManyElements) {
  s21::map<int, std::string> m;
  std::vector<std::pair<int, std::string>> values = {
      {1, "one"}, {2, "two"}, {3, "three"}};
  m.insert_many(values.begin(), values.end());
  EXPECT_EQ(m.size(), 3);
  EXPECT_EQ(m[1], "one");
  EXPECT_EQ(m[2], "two");
  EXPECT_EQ(m[3], "three");
}

TEST(MapTest, FindNonExistingKeyWithConstMap) {
  const s21::map<int, std::string> m{{1, "one"}, {2, "two"}};
  auto it = m.find(3);
  EXPECT_EQ(it, m.end());
}
TEST(MapTest, CountMethod) {
  s21::map<int, std::string> m;
  m.insert(std::make_pair(1, "one"));
  EXPECT_EQ(m.count(1), 1);
  EXPECT_EQ(m.count(2), 0);
}
TEST(MapTest, EqualityOperator) {
  s21::map<int, std::string> m1{{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2{{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m3{{1, "one"}, {3, "three"}};
  EXPECT_TRUE(m1 == m2);
  EXPECT_FALSE(m1 == m3);
}
TEST(MapTest, InequalityOperator) {
  s21::map<int, std::string> m1{{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2{{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m3{{1, "one"}, {3, "three"}};
  EXPECT_FALSE(m1 != m2);
  EXPECT_TRUE(m1 != m3);
}
TEST(MapTest, MergeWithoutModifyingOriginal) {
  s21::map<int, std::string> m1{{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2{{3, "three"}};
  s21::map<int, std::string> m1_copy = m1;
  m1.merge(m2);
  EXPECT_EQ(m1.size(), 3);
  EXPECT_EQ(m1_copy.size(), 2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
} // namespace s21