#ifndef S21_CONTAINERS_S21_CONTAINERS_REDBLACKTREE_H_
#define S21_CONTAINERS_S21_CONTAINERS_REDBLACKTREE_H_

#include <functional>
#include <limits>
#include <vector>

namespace s21 {

enum Color{BLACK, RED};


template <typename Key, typename Comparator = std::less<Key>>
class RedBlackTree {
private:
  struct RedBlackTreeNode;
  struct RedBlackTreeIterator;
  struct RedBlackTreeIteratorConst;

public:
  using key_type = Key;
  using reference = key_type &;
  using const_reference = const key_type &;
  using iterator = RedBlackTreeIterator;
  using const_iterator = RedBlackTreeIteratorConst;
  using size_type = std::size_t;

  


  RedBlackTree() : head_(new RedBlackTreeNode), size_(0U) {}
  RedBlackTree(const RedBlackTree &other);
  RedBlackTree(RedBlackTree &&other) noexcept;
  RedBlackTree &operator=(const RedBlackTree &other);
  RedBlackTree &operator=(RedBlackTree &&other) noexcept;
  ~RedBlackTree();

  void Clear() noexcept;
  [[nodiscard]] size_type Size() const noexcept;
  [[nodiscard]] bool Empty() const noexcept;
  [[nodiscard]] size_type MaxSize() const noexcept;
  iterator Begin() noexcept;
  const_iterator Begin() const noexcept;
  iterator End() noexcept;
  const_iterator End() const noexcept;
  void Merge(RedBlackTree &other);
  void MergeUnique(RedBlackTree &other);
  iterator Insert(const key_type &key);
  std::pair<iterator, bool> InsertUnique(const key_type &key);
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> Emplace(Args &&...args);
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> EmplaceUnique(Args &&...args);
  iterator Find(const_reference key);
  iterator LowerBound(const_reference key);
  iterator UpperBound(const_reference key);
  void Erase(iterator pos) noexcept;
  void Swap(RedBlackTree &other) noexcept;
  [[nodiscard]] bool CheckTree() const noexcept;

private:
  bool CheckRedNodesRecursively(const RedBlackTreeNode *Node) const noexcept;
  int ComputeBlackHeight(const RedBlackTreeNode *node) const noexcept;
  RedBlackTreeNode *SearchMaximum(RedBlackTreeNode *node) const noexcept;
  RedBlackTreeNode *SearchMinimum(RedBlackTreeNode *node) const noexcept;
  void EraseBalancing(RedBlackTreeNode *deleted_node) noexcept;
  void SwapNodesForErase(RedBlackTreeNode *node, RedBlackTreeNode *other) noexcept;
  void RotateRight(RedBlackTreeNode *node) noexcept;
  RedBlackTreeNode *ExtractNode(iterator pos) noexcept;
  void RotateLeft(RedBlackTreeNode *node) noexcept;
  void CopyTreeFromOther(const RedBlackTree &other);
  RedBlackTreeNode *CopyTree(const RedBlackTreeNode *node, RedBlackTreeNode *parent);
  void Destroy(RedBlackTreeNode *node) noexcept;
  void InitializeHead() noexcept;
  RedBlackTreeNode *&Root();
  const RedBlackTreeNode *Root() const;
  RedBlackTreeNode *&MostLeft();
  const RedBlackTreeNode *MostLeft() const;
  RedBlackTreeNode *&MostRight();
  std::pair<iterator, bool> Insert(RedBlackTreeNode *root, RedBlackTreeNode *new_node,
                                   bool unique_only);
  void BalancingInsert(RedBlackTreeNode *node);


  struct RedBlackTreeNode {
    RedBlackTreeNode()
        : parent_(nullptr), left_(this), right_(this), key_(key_type{}),
          color_(RED) {}

    explicit RedBlackTreeNode(const key_type &key)
        : parent_(nullptr), left_(nullptr), right_(nullptr), key_(key),
          color_(RED) {}

    explicit RedBlackTreeNode(key_type &&key)
        : parent_(nullptr), left_(nullptr), right_(nullptr),
          key_(std::move(key)), color_(RED) {}

    RedBlackTreeNode(key_type key, Color color)
        : parent_(nullptr), left_(nullptr), right_(nullptr), key_(key),
          color_(color) {}

    void ToDefault() noexcept {
      left_ = nullptr;
      right_ = nullptr;
      parent_ = nullptr;
      color_ = RED;
    }

    RedBlackTreeNode *NextNode() const noexcept {
      auto *node = const_cast<RedBlackTreeNode *>(this);
      if (node->color_ == RED &&
          (node->parent_ == nullptr || node->parent_->parent_ == node)) {
        node = node->left_;
      } else if (node->right_ != nullptr) {
        node = node->right_;
        while (node->left_ != nullptr) {
          node = node->left_;
        }
      } else {
        RedBlackTreeNode *parent = node->parent_;
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

    RedBlackTreeNode *PrevNode() const noexcept {
      auto *node = const_cast<RedBlackTreeNode *>(this);
      if (node->color_ == RED &&
          (node->parent_ == nullptr || node->parent_->parent_ == node)) {
        node = node->right_;
      } else if (node->left_ != nullptr) {
        node = node->left_;
        while (node->right_ != nullptr) {
          node = node->right_;
        }
      } else {
        RedBlackTreeNode *parent = node->parent_;
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

    RedBlackTreeNode *parent_;
    RedBlackTreeNode *left_;
    RedBlackTreeNode *right_;
    key_type key_;
    Color color_;
  };

  struct RedBlackTreeIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = RedBlackTree::key_type;
    using pointer = value_type *;
    using reference = value_type &;

    RedBlackTreeIterator() = delete;

    explicit RedBlackTreeIterator(RedBlackTreeNode *node) : node_(node) {}

    reference operator*() const noexcept { return node_->key_; }

    iterator &operator++() noexcept {
      node_ = node_->NextNode();
      return *this;
    }

    iterator operator++(int) noexcept {
      iterator tmp{node_};
      ++(*this);
      return tmp;
    }

    iterator &operator--() noexcept {
      node_ = node_->PrevNode();
      return *this;
    }

    iterator operator--(int) noexcept {
      iterator tmp{node_};
      --(*this);
      return tmp;
    }

    bool operator==(const iterator &other) const noexcept {
      return node_ == other.node_;
    }

    bool operator!=(const iterator &other) const noexcept {
      return node_ != other.node_;
    }

    RedBlackTreeNode *node_;
  };

  struct RedBlackTreeIteratorConst {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = RedBlackTree::key_type;
    using pointer = const value_type *;
    using reference = const value_type &;

    RedBlackTreeIteratorConst() = delete;

    explicit RedBlackTreeIteratorConst(const RedBlackTreeNode *node) : node_(node) {}

    explicit RedBlackTreeIteratorConst(const iterator &it) : node_(it.node_) {}

    reference operator*() const noexcept { return node_->key_; }

    const_iterator &operator++() noexcept {
      node_ = node_->NextNode();
      return *this;
    }

    const_iterator operator++(int) noexcept {
      const_iterator tmp{node_};
      ++(*this);
      return tmp;
    }

    const_iterator &operator--() noexcept {
      node_ = node_->PrevNode();
      return *this;
    }

    const_iterator operator--(int) noexcept {
      const_iterator tmp{node_};
      --(*this);
      return tmp;
    }

    friend bool operator==(const const_iterator &it1,
                           const const_iterator &it2) noexcept {
      return it1.node_ == it2.node_;
    }

    friend bool operator!=(const const_iterator &it1,
                           const const_iterator &it2) noexcept {
      return it1.node_ != it2.node_;
    }

    const RedBlackTreeNode *node_;
  };

  RedBlackTreeNode *head_;
  size_type size_{};
  Comparator keyComparator_;
};

} // namespace s21
#include "RedBlackTree.tpp"
#endif
