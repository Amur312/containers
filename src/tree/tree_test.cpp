 #include "../tree/RedBlackTree.h"
 #include <gtest/gtest.h>

 TEST(RedBlackTreeTest, InsertAndSize) {
   s21::RedBlackTree<int> tree;
   EXPECT_TRUE(tree.Empty());
   EXPECT_EQ(tree.Size(), 0);
   tree.Insert(5);
   EXPECT_FALSE(tree.Empty());
   EXPECT_EQ(tree.Size(), 1);

   tree.Insert(10);
   EXPECT_EQ(tree.Size(), 2);
 }

 TEST(RedBlackTreeTest, FindAndErase) {
   s21::RedBlackTree<int> tree;
   tree.Insert(5);
   tree.Insert(10);

   auto it = tree.Find(5);
   EXPECT_NE(it, tree.End());
   EXPECT_EQ(*it, 5);

   tree.Erase(it);
   EXPECT_EQ(tree.Size(), 1);
   it = tree.Find(5);
   EXPECT_EQ(it, tree.End());
 }
//
 TEST(RedBlackTreeTest, MergeTrees) {
   s21::RedBlackTree<int> tree1;
   tree1.Insert(5);
   tree1.Insert(10);

   s21::RedBlackTree<int> tree2;
   tree2.Insert(15);
   tree2.Insert(20);

   tree1.Merge(tree2);

   EXPECT_EQ(tree1.Size(), 4);
   EXPECT_EQ(tree2.Size(), 0);
 }

 TEST(RedBlackTreeTest, UpperBoundCheck) {
   s21::RedBlackTree<int> tree;
   tree.Insert(5);
   tree.Insert(10);
   tree.Insert(15);

   auto it = tree.UpperBound(7);
   EXPECT_NE(it, tree.End());
   EXPECT_EQ(*it, 10);
 }

 TEST(RedBlackTreeTest, ClearTree) {
   s21::RedBlackTree<int> tree;
   tree.Insert(5);
   tree.Insert(10);
   tree.Insert(15);

   EXPECT_EQ(tree.Size(), 3);
   tree.Clear();
   EXPECT_TRUE(tree.Empty());
   EXPECT_EQ(tree.Size(), 0);
 }

 TEST(RedBlackTreeTest, InsertDuplicateKeys) {
   s21::RedBlackTree<int> tree;
   tree.Insert(5);
   tree.Insert(5);
   tree.Insert(5);
   EXPECT_EQ(tree.Size(), 3);
 }

 TEST(RedBlackTreeTest, ReverseOrderIteration) {
   s21::RedBlackTree<int> tree;
   for (int i = 9; i >= 0; --i) {
     tree.Insert(i);
   }

   int expectedValue = 0;
   for (auto it = tree.Begin(); it != tree.End(); ++it) {
     EXPECT_EQ(*it, expectedValue);
     ++expectedValue;
   }
 }

 TEST(RedBlackTreeTest, LowerBoundCheck) {
   s21::RedBlackTree<int> tree;
   tree.Insert(5);
   tree.Insert(10);
   tree.Insert(15);

   auto it = tree.LowerBound(7);
   EXPECT_NE(it, tree.End());
   EXPECT_EQ(*it, 10);

   it = tree.LowerBound(5);
   EXPECT_EQ(*it, 5);
 }

 TEST(RedBlackTreeTest, EraseAndBalance) {
   s21::RedBlackTree<int> tree;
   tree.Insert(5);
   tree.Insert(10);
   tree.Insert(15);
   tree.Insert(20);
   tree.Insert(25);

   auto it = tree.Find(15);
   tree.Erase(it);

   EXPECT_EQ(tree.Size(), 4);
   EXPECT_EQ(*tree.Find(5), 5);
   EXPECT_EQ(*tree.Find(10), 10);
   EXPECT_EQ(*tree.Find(20), 20);
   EXPECT_EQ(*tree.Find(25), 25);
   it = tree.Find(15);
   EXPECT_EQ(it, tree.End());
 }
 TEST(RedBlackTreeTest, CopyConstructor) {
   s21::RedBlackTree<int> original;
   original.Insert(5);
   original.Insert(10);
   original.Insert(15);

   s21::RedBlackTree<int> copy = original;

   EXPECT_EQ(copy.Size(), 3);
   EXPECT_EQ(*copy.Find(5), 5);
   EXPECT_EQ(*copy.Find(10), 10);
   EXPECT_EQ(*copy.Find(15), 15);

   // Ensure the original is unchanged
   EXPECT_EQ(original.Size(), 3);
   EXPECT_EQ(*original.Find(5), 5);
 }

 TEST(RedBlackTreeTest, AssignmentOperator) {
   s21::RedBlackTree<int> original;
   original.Insert(5);
   original.Insert(10);
   original.Insert(15);

   s21::RedBlackTree<int> assigned;
   assigned = original;

   EXPECT_EQ(assigned.Size(), 3);
   EXPECT_EQ(*assigned.Find(5), 5);
   EXPECT_EQ(*assigned.Find(10), 10);
   EXPECT_EQ(*assigned.Find(15), 15);

   // Ensure the original is unchanged
   EXPECT_EQ(original.Size(), 3);
   EXPECT_EQ(*original.Find(5), 5);
 }

 TEST(RedBlackTreeTest, InsertAndFindNegativeNumbers) {
   s21::RedBlackTree<int> tree;
   tree.Insert(-5);
   tree.Insert(-10);
   EXPECT_EQ(tree.Size(), 2);
   EXPECT_EQ(*tree.Find(-5), -5);
   EXPECT_EQ(*tree.Find(-10), -10);
 }
 TEST(RedBlackTreeTest, MoveConstructorAndAssignment) {
   s21::RedBlackTree<int> original;
   original.Insert(5);
   original.Insert(10);
   original.Insert(15);

   s21::RedBlackTree<int> moved = std::move(original);

   EXPECT_EQ(moved.Size(), 3);
   EXPECT_EQ(*moved.Find(5), 5);
   EXPECT_EQ(*moved.Find(10), 10);
   EXPECT_EQ(*moved.Find(15), 15);

   s21::RedBlackTree<int> anotherTree;
   anotherTree.Insert(20);

   anotherTree = std::move(moved);

   EXPECT_EQ(anotherTree.Size(), 3);
   EXPECT_EQ(*anotherTree.Find(5), 5);
   EXPECT_EQ(*anotherTree.Find(10), 10);
   EXPECT_EQ(*anotherTree.Find(15), 15);
 }

 TEST(RedBlackTreeTest, OperationsOnEmptyTree) {
   s21::RedBlackTree<int> tree;

   EXPECT_TRUE(tree.Empty());
   EXPECT_EQ(tree.Size(), 0);
   EXPECT_EQ(tree.Find(5), tree.End());
   EXPECT_NO_THROW(tree.Clear());
   auto it = tree.Begin();
   EXPECT_EQ(it, tree.End());
   EXPECT_NO_THROW(tree.Erase(it));
 }

 TEST(RedBlackTreeTest, EraseVariousNodes) {
   s21::RedBlackTree<int> tree;
   tree.Insert(30);
   tree.Insert(20);
   tree.Insert(40);
   tree.Insert(10);
   tree.Insert(25);
   tree.Insert(35);
   tree.Insert(50);

   EXPECT_EQ(tree.Size(), 7);

   tree.Erase(tree.Find(30));
   EXPECT_EQ(tree.Size(), 6);
   EXPECT_EQ(tree.Find(30), tree.End());

   tree.Erase(tree.Find(20));
   EXPECT_EQ(tree.Size(), 5);
   EXPECT_EQ(tree.Find(20), tree.End());

   tree.Erase(tree.Find(40));
   EXPECT_EQ(tree.Size(), 4);
   EXPECT_EQ(tree.Find(40), tree.End());

   tree.Erase(tree.Find(10));
   EXPECT_EQ(tree.Size(), 3);
   EXPECT_EQ(tree.Find(10), tree.End());
 }

 TEST(RedBlackTreeTest, ExtractNode) {
   s21::RedBlackTree<int> tree;
   tree.Insert(30);
   tree.Insert(20);
   tree.Insert(40);
   tree.Insert(10);
   tree.Insert(25);
   tree.Insert(35);
   tree.Insert(50);

   EXPECT_EQ(tree.Size(), 7);

   tree.Erase(tree.Find(30));

   EXPECT_EQ(tree.Size(), 6);

   EXPECT_EQ(tree.Find(30), tree.End());
 }

 TEST(RedBlackTreeTest, MergeAndFindElements) {
   s21::RedBlackTree<int> tree1;
   tree1.Insert(5);
   tree1.Insert(10);

   s21::RedBlackTree<int> tree2;
   tree2.Insert(15);
   tree2.Insert(20);

   tree1.Merge(tree2);

   EXPECT_EQ(tree1.Size(), 4);
   EXPECT_EQ(tree2.Size(), 0);

   EXPECT_NE(tree1.Find(5), tree1.End());
   EXPECT_NE(tree1.Find(10), tree1.End());
   EXPECT_NE(tree1.Find(15), tree1.End());
   EXPECT_NE(tree1.Find(20), tree1.End());
 }

 TEST(RedBlackTreeTest, DestroyFunctionality) {
   s21::RedBlackTree<int> tree;
   tree.Insert(5);
   tree.Insert(10);

   tree.Clear();

   EXPECT_TRUE(tree.Empty());
   EXPECT_EQ(tree.Size(), 0);
 }

 TEST(RedBlackTreeTest, InitializeHeadFunctionality) {
   s21::RedBlackTree<int> tree;

   EXPECT_TRUE(tree.Empty());
   EXPECT_EQ(tree.Size(), 0);
 }






 int main(int argc, char **argv) {

   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
 }
