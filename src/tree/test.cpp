#include "RBTree.h"
#include <gtest/gtest.h>

TEST(RBTreeTest, InsertionAndIterationTest) {
  s21::RBTree<int, int> tree;

  // Вставляем значения
  tree.insert(3);
  tree.insert(1);
  tree.insert(5);
  tree.insert(4);
  tree.insert(2);

  // Ожидаем, что итерация будет в отсортированном порядке
  std::vector<int> expected = {1, 2, 3, 4, 5};
  std::vector<int> actual;

  for (const auto& node : tree) {
    actual.push_back(node.key);
  }

  ASSERT_EQ(expected, actual);
}

TEST(RBTreeTest, RootIsBlackTest) {
  s21::RBTree<int, int> tree;

  tree.insert(3);
  tree.insert(1);
  tree.insert(5);
  tree.insert(4);
  tree.insert(2);

  // Проверяем, что цвет корня - черный
  ASSERT_EQ(tree.getRootColor(), s21::BLACK);
}





int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
