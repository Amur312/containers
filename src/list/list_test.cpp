#include "s21_list.h"
#include <gtest/gtest.h>

TEST(ListTest, PushBack) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(*list.cBegin(), 1);
  EXPECT_EQ(*(++list.cBegin()), 2);
  EXPECT_EQ(*(++ ++list.cBegin()), 3);
}

TEST(ListTest, PushFront) {
  s21::List<int> list;
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(*list.cBegin(), 3);
  EXPECT_EQ(*(++list.cBegin()), 2);
  EXPECT_EQ(*(++ ++list.cBegin()), 1);
}

TEST(ListTest, PopBack) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.pop_back();
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(*list.cBegin(), 1);
  EXPECT_EQ(*(++list.cBegin()), 2);
}

TEST(ListTest, PopFront) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.pop_front();
  EXPECT_EQ(list.size(), 2);
  EXPECT_EQ(*list.cBegin(), 2);
  EXPECT_EQ(*(++list.cBegin()), 3);
}

TEST(ListTest, Insert) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(3);
  list.push_back(4);
  auto it = ++list.cBegin();
  s21::List<int>::iterator mutable_it(it._node);
  list.insert(mutable_it, 2);
  EXPECT_EQ(list.size(), 4);
  EXPECT_EQ(*list.cBegin(), 1);
  EXPECT_EQ(*(++list.cBegin()), 2);
  EXPECT_EQ(*(++ ++list.cBegin()), 3);
  EXPECT_EQ(*(++ ++ ++list.cBegin()), 4);
}

TEST(ListTest, Merge) {
  s21::List<int> list1;
  list1.push_back(1);
  list1.push_back(3);
  list1.push_back(5);
  s21::List<int> list2;
  list2.push_back(2);
  list2.push_back(4);
  list2.push_back(6);
  list1.merge(list2);
  EXPECT_EQ(list1.size(), 6);
  EXPECT_EQ(*list1.cBegin(), 1);
  EXPECT_EQ(*(++list1.cBegin()), 2);
  EXPECT_EQ(*(++ ++list1.cBegin()), 3);
  EXPECT_EQ(*(++ ++ ++list1.cBegin()), 4);
  EXPECT_EQ(*(++ ++ ++ ++list1.cBegin()), 5);
  EXPECT_EQ(*(++ ++ ++ ++ ++list1.cBegin()), 6);
}

TEST(ListTest, Sort) {
  s21::List<int> list;
  list.push_back(3);
  list.push_back(1);
  list.push_back(2);
  list.sort();
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(*list.cBegin(), 1);
  EXPECT_EQ(*(++list.cBegin()), 2);
  EXPECT_EQ(*(++ ++list.cBegin()), 3);
}
TEST(ListTest, EmptyList) {
  s21::List<int> list;
  EXPECT_EQ(list.size(), 0);
  EXPECT_TRUE(list.empty());
}

TEST(ListTest, Reverse) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.reverse();
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(*list.cBegin(), 3);
  EXPECT_EQ(*(++list.cBegin()), 2);
  EXPECT_EQ(*(++ ++list.cBegin()), 1);
}

TEST(ListTest, Clear) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  list.clear();
  EXPECT_EQ(list.size(), 0);
  EXPECT_TRUE(list.empty());
}

TEST(ListTest, FrontAndBack) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  EXPECT_EQ(list.front(), 1);
  EXPECT_EQ(list.back(), 3);
}

TEST(ListTest, Swap) {
  s21::List<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  s21::List<int> list2;
  list2.push_back(4);
  list2.push_back(5);
  list2.push_back(6);
  list1.swap(list2);
  EXPECT_EQ(list1.size(), 3);
  EXPECT_EQ(*list1.cBegin(), 4);
  EXPECT_EQ(*(++list1.cBegin()), 5);
  EXPECT_EQ(*(++ ++list1.cBegin()), 6);
  EXPECT_EQ(list2.size(), 3);
  EXPECT_EQ(*list2.cBegin(), 1);
  EXPECT_EQ(*(++list2.cBegin()), 2);
  EXPECT_EQ(*(++ ++list2.cBegin()), 3);
}

TEST(ListTest, CopyList) {
  s21::List<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  s21::List<int> list2;
  list2.copyList(list1);
  EXPECT_EQ(list2.size(), 3);
  EXPECT_EQ(*list2.cBegin(), 1);
  EXPECT_EQ(*(++list2.cBegin()), 2);
  EXPECT_EQ(*(++ ++list2.cBegin()), 3);
}

TEST(ListTest, MoveList) {
  s21::List<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  s21::List<int> list2;
  list2.moveList(list1);
  EXPECT_EQ(list2.size(), 3);
  EXPECT_EQ(*list2.cBegin(), 1);
  EXPECT_EQ(*(++list2.cBegin()), 2);
  EXPECT_EQ(*(++ ++list2.cBegin()), 3);
  EXPECT_EQ(list1.size(), 0);
}

TEST(ListTest, MaxSize) {
  s21::List<int> list;
  EXPECT_EQ(list.max_size(), std::numeric_limits<size_t>::max() / sizeof(int));
}

TEST(ListTest, AssignmentOperator) {
  s21::List<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  s21::List<int> list2;
  list2 = list1;
  EXPECT_EQ(list2.size(), 3);
  EXPECT_EQ(*list2.cBegin(), 1);
  EXPECT_EQ(*(++list2.cBegin()), 2);
  EXPECT_EQ(*(++ ++list2.cBegin()), 3);
}

TEST(ListTest, Empty) {
  s21::List<int> list;
  EXPECT_TRUE(list.empty());
  list.push_back(1);
  EXPECT_FALSE(list.empty());
  list.pop_front();
  EXPECT_TRUE(list.empty());
}

TEST(ListTest, Size) {
  s21::List<int> list;
  EXPECT_EQ(list.size(), 0);
  list.push_back(1);
  EXPECT_EQ(list.size(), 1);
  list.push_back(2);
  EXPECT_EQ(list.size(), 2);
  list.pop_front();
  EXPECT_EQ(list.size(), 1);
  list.pop_front();
  EXPECT_EQ(list.size(), 0);
}

TEST(ListTest, CopyConstructor) {
  s21::List<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  s21::List<int> list2(list1);
  EXPECT_EQ(list2.size(), 3);
  EXPECT_EQ(*list2.cBegin(), 1);
  EXPECT_EQ(*(++list2.cBegin()), 2);
  EXPECT_EQ(*(++ ++list2.cBegin()), 3);
}

TEST(ListTest, MoveConstructor) {
  s21::List<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  s21::List<int> list2(std::move(list1));
  EXPECT_EQ(list2.size(), 3);
  EXPECT_EQ(*list2.cBegin(), 1);
  EXPECT_EQ(*(++list2.cBegin()), 2);
  EXPECT_EQ(*(++ ++list2.cBegin()), 3);
  EXPECT_EQ(list1.size(), 0);
}

TEST(ListTest, MoveAssignmentOperator) {
  s21::List<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);
  s21::List<int> list2;
  list2 = std::move(list1);
  EXPECT_EQ(list2.size(), 3);
  EXPECT_EQ(*list2.cBegin(), 1);
  EXPECT_EQ(*(++list2.cBegin()), 2);
  EXPECT_EQ(*(++ ++list2.cBegin()), 3);
  EXPECT_EQ(list1.size(), 0);
}

TEST(ListTest, InsertMany) {
  s21::List<int> list;
  list.push_back(1);
  list.push_back(4);
  list.push_back(5);

  auto it = ++list.cBegin();
  s21::List<int>::iterator mutable_it(it._node);
  list.insert_many(mutable_it, 2, 3);

  EXPECT_EQ(list.size(), 5);

  auto iter = list.cBegin();
  EXPECT_EQ(*iter, 1);
  iter++;
  EXPECT_EQ(*iter, 2);
  iter++;
  EXPECT_EQ(*iter, 3);
  iter++;
  EXPECT_EQ(*iter, 4);
  iter++;
  EXPECT_EQ(*iter, 5);
}
TEST(ListTest, InsertManyFront) {
  s21::List<int> list;
  list.push_back(3);
  list.push_back(4);
  list.push_back(5);

  list.insert_many_front(1, 2);

  EXPECT_EQ(list.size(), 5);

  auto iter = list.cBegin();
  EXPECT_EQ(*iter, 1);
  iter++;
  EXPECT_EQ(*iter, 2);
  iter++;
  EXPECT_EQ(*iter, 3);
  iter++;
  EXPECT_EQ(*iter, 4);
  iter++;
  EXPECT_EQ(*iter, 5);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
