#ifndef CPP2_S21_CONTAINERS_1_MAIN_H
#define CPP2_S21_CONTAINERS_1_MAIN_H

#include <functional>
#include <limits>
#include <vector>

namespace s21 {

enum Color { RED, BLACK };

template <typename Key, typename Value = std::less<Key>> class RedBlackTree {
private:
  struct TreeIterator;
  struct TreeIteratorConst;
  struct TreeNode {
    Key key;
    Value value;
    Color color;
    TreeNode *parent_;
    TreeNode *left_;
    TreeNode *right_;

    explicit TreeNode(const Key &key)
        : key(key), color(Color::RED), parent_(nullptr), left_(nullptr),
          right_(nullptr) {}

    explicit TreeNode(Key &&key)
        : key(std::move(key)), color(Color::RED), parent_(nullptr),
          left_(nullptr), right_(nullptr) {}

    TreeNode(const Key &key, Color color)
        : key(key), color(color), parent_(nullptr), left_(nullptr),
          right_(nullptr) {}

    void to_default() noexcept {
      left_ = nullptr;
      right_ = nullptr;
      parent_ = nullptr;
      color = Color::RED;
    }

    TreeNode *next_node() noexcept {
      TreeNode *node = this;
      if (node->right_ != nullptr) {
        node = node->right_;
        while (node->left_ != nullptr) {
          node = node->left_;
        }
      } else {
        while (node->parent_ != nullptr && node == node->parent_->right_) {
          node = node->parent_;
        }
        node = node->parent_;
      }
      return node;
    }

    TreeNode *prev_node() noexcept {
      TreeNode *node = this;
      if (node->left_ != nullptr) {
        node = node->left_;
        while (node->right_ != nullptr) {
          node = node->right_;
        }
      } else {
        while (node->parent_ != nullptr && node == node->parent_->left_) {
          node = node->parent_;
        }
        node = node->parent_;
      }
      return node;
    }
  };

public:
  using key_type = Key;
  using reference = key_type &;
  using const_reference = const key_type &;
  using iterator = TreeIterator;
  using const_iterator = TreeIteratorConst;
  using size_type = std::size_t;

  RedBlackTree() : head_(nullptr), size_(0U) {}
  RedBlackTree(const RedBlackTree &other) : head_(nullptr), size_(0U) {
    copyTreeFrom(other);
  }

  RedBlackTree(TreeNode &&other) noexcept;
  RedBlackTree &operator=(const RedBlackTree &other) {
    if (this != &other) {
      if (other.size() > 0) {
        CopyTreeFromOther(other);
      } else {
        clear();
      }
    }
    return *this;
  }
  RedBlackTree &operator=(RedBlackTree &&other) noexcept {
    if (this != &other) {
      swap(other);
    }
    return *this;
  }
  ~RedBlackTree() {
    clear();
    if (head_ != nullptr) {
      delete head_;
      head_ = nullptr;
    }
  }

  void clear() noexcept;
  size_type size() const noexcept;
  bool empty() const noexcept;
  size_type maxSize() const noexcept;
  iterator cBegin() noexcept;
  const_iterator cBegin() const noexcept;
  iterator cEnd() noexcept;
  const_iterator cEnd() const noexcept;
  void merge(RedBlackTree &other);
  void mergeUnique(RedBlackTree &other);
  iterator insert(const key_type &key);
  std::pair<iterator, bool> insertUnique(const key_type &key);
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> emplace(Args &&...args);
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> emplaceUnique(Args &&...args);
  iterator find(const_reference key);
  iterator lowerBound(const_reference key);
  iterator upperBound(const_reference key);
  void erase(iterator pos) noexcept;
  void swap(RedBlackTree &other) noexcept;
  bool checkTree() const noexcept;

private:
  void copyTreeFrom(const RedBlackTree &other);
  TreeNode *copyTree(const TreeNode *node, TreeNode *parent);
  void destroy(TreeNode *root) noexcept;
  void initializeRootNode() noexcept;
  std::pair<iterator, bool> insert(TreeNode *root, TreeNode *newNode,
                                   bool only_unique_values);
  void balanceInsert(TreeNode *node);
  void rotateRight(TreeNode *node) noexcept;
  void rotateLeft(TreeNode *node) noexcept;
  TreeNode *extractNode(iterator pos) noexcept;
  void swapNodesForErase(TreeNode *node, TreeNode *other) noexcept;
  void eraseBalancing(TreeNode *deleted_node) noexcept;
  TreeNode *searchMinimum(TreeNode *node) const noexcept;
  TreeNode *searchMaximum(TreeNode *node) const noexcept;
  [[maybe_unused]] int computeBlackHeight(const TreeNode *node) const noexcept;
  bool checkRedNodes(const TreeNode *Node) const noexcept;

  struct TreeNode;
  struct TreeIterator;
  struct TreeIteratorConst;

  TreeNode *head_;
  size_type size_;
  Value key_comparator_;
};

} // namespace s21
#include "RedBlackTree.tpp"
#endif
