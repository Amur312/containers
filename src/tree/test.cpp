#include "s21_tree.h"
#include <gtest/gtest.h>

namespace s21 {

class RedBlackTreeTest : public ::testing::Test {
protected:
  using Tree = RedBlackTree<int>; // Предполагается, что ваш класс дерева называется RedBlackTree и работает с int.
  Tree tree;

  void SetUp() override {
    // Инициализация дерева, если требуется.
  }
};

TEST_F(RedBlackTreeTest, InsertNode) {
  tree.Insert(5);
  tree.Insert(3);
  tree.Insert(7);

  // Проверяем корректное расположение элементов
  EXPECT_EQ(tree.SearchMinimum(tree.Root())->key_, 3);
  EXPECT_EQ(tree.SearchMaximum(tree.Root())->key_, 7);
}

TEST_F(RedBlackTreeTest, ExtractNode) {
  tree.Insert(5);
  tree.Insert(3);
  tree.Insert(7);
  auto node = tree.ExtractNode(tree.Find(3)); // Предполагается, что метод Find возвращает итератор.

  // Проверяем, что узел действительно извлечен
  EXPECT_EQ(node->key_, 3);
  EXPECT_EQ(tree.SearchMinimum(tree.Root())->key_, 5);
}

TEST_F(RedBlackTreeTest, BlackHeightBalance) {
  tree.Insert(5);
  tree.Insert(3);
  tree.Insert(7);
  tree.Insert(1);
  tree.Insert(9);

  // Проверяем корректный баланс черных узлов
  EXPECT_GE(tree.ComputeBlackHeight(tree.Root()), 0);
}

TEST_F(RedBlackTreeTest, RedNodesCheck) {
  tree.Insert(5);
  tree.Insert(3);
  tree.Insert(7);

  // Проверяем, что красные узлы соответствуют правилам
  EXPECT_TRUE(tree.checkRedNodes(tree.Root()));
}

} // namespace s21

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
