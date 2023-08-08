#ifndef CPP2_S21_CONTAINERS_1_MAIN_H
#define CPP2_S21_CONTAINERS_1_MAIN_H

#include <functional>
#include <limits>
#include <vector>

namespace s21 {

enum Color { RED, BLACK };

template <typename Key, typename Compare = std::less<Key>> class RedBlackTree {
private:
  struct TreeIterator;
  struct TreeIteratorConst;
  struct TreeNode {
    Key key;
    Color color;
    TreeNode *parent_;
    TreeNode *left_;
    TreeNode *right_;
    explicit TreeNode(const Key &key, Color color = Color::RED,
                      TreeNode *parent = nullptr)
        : key(key), color(color), parent_(parent), left_(nullptr),
          right_(nullptr) {}

    void to_default() noexcept {
      left_ = nullptr;
      right_ = nullptr;
      parent_ = nullptr;
      color = Color::RED;
    }

    TreeNode *next_node() noexcept {
      TreeNode *node = const_cast<TreeNode *>(this);
      if (node->color == RED &&
          (node->parent_ == nullptr || node->parent_->parent_ == node)) {

        node = node->left_;
      } else if (node->right_ != nullptr) {

        node = node->right_;

        while (node->left_ != nullptr) {
          node = node->left_;
        }
      } else {

        TreeNode *parent = node->parent_;

        while (node == parent->right_) {
          node = parent;
          parent = parent->parent_;
        }

        if (node->right_ != parent) {
          node = parent;
        }
      }

      return node;
    }
    TreeNode *prev_node() noexcept {
      TreeNode *node = const_cast<TreeNode *>(this);

      if (node->color == RED &&
          (node->parent_ == nullptr || node->parent_->parent_ == node)) {
        node = node->right_;
      } else if (node->left_ != nullptr) {
        node = node->left_;
        while (node->right_ != nullptr) {
          node = node->right_;
        }
      } else {
        TreeNode *parent = node->parent_;
        while (node == parent->left_) {
          node = parent;
          parent = parent->parent_;
        }
        if (node->left_ != parent) {
          node = parent;
        }
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

  RedBlackTree() : size_(0U) {
    head_ = new TreeNode(Key(), Color::BLACK);
    head_->left_ = head_;
    head_->right_ = head_;
    head_->parent_ = nullptr;
  }
  RedBlackTree(const RedBlackTree &other) : head_(nullptr), size_(0U) {

    if (other.size() > 0) {
      copyTreeFrom(other);
    }
  }

  RedBlackTree(RedBlackTree &&other) noexcept { swap(other); }
  RedBlackTree &operator=(const RedBlackTree &other) {
    if (this != &other) {
      if (other.Size() > 0) {
        copyTreeFrom(other);
      } else {
        clear();
      }
    }

    return *this;
  }
  RedBlackTree &operator=(RedBlackTree &&other) noexcept {
    clear();
    swap(other);
    return *this;
  }
  ~RedBlackTree() {
    clear();
    delete head_;
    head_ = nullptr;
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
  [[nodiscard]] bool checkTree() const noexcept;

private:
  void copyTreeFrom(const RedBlackTree &other);
  TreeNode *copyTree(const TreeNode *node, TreeNode *parent);
  void destroy(TreeNode *root) noexcept;
  void initializeRootNode() noexcept;
  std::pair<iterator, bool> insert(TreeNode *root, TreeNode *newNode,
                                   bool unique_only);
  void balanceInsert(TreeNode *node);
  void rotateRight(TreeNode *node) noexcept;
  void rotateLeft(TreeNode *node) noexcept;
  TreeNode *extractNode(iterator pos) noexcept;
  void swapNodesForErase(TreeNode *node, TreeNode *other) noexcept;
  void eraseBalancing(TreeNode *deleted_node) noexcept;
  TreeNode *searchMinimum(TreeNode *node) const noexcept;
  TreeNode *searchMaximum(TreeNode *node) const noexcept;
  [[maybe_unused]] int
  calculateBlackNodeHeight(const TreeNode *node) const noexcept;
  bool checkRedNodes(const TreeNode *Node) const noexcept;
  void handleBlackSibling(TreeNode *sibling, TreeNode *parent,
                          TreeNode *&currentNode, bool isLeftChild);
  void handleRedSibling(TreeNode *sibling, TreeNode *parent, bool isLeftChild);

  TreeNode *head_;
  size_type size_{};
  Compare key_comparator_;
};

} // namespace s21

#include "RedBlackTree.tpp"
#endif
