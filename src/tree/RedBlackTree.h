#ifndef S21_CONTAINERS_S21_CONTAINERS_REDBLACKTREE_H_
#define S21_CONTAINERS_S21_CONTAINERS_REDBLACKTREE_H_

#include <functional>
#include <limits>
#include <stack>
#include <vector>

namespace s21 {

enum RedBlackTreeColor { kBlack, kRed };

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

  using tree_type = RedBlackTree;
  using tree_node = RedBlackTreeNode;
  using tree_color = RedBlackTreeColor;

  RedBlackTree();
  RedBlackTree(const tree_type &other);
  RedBlackTree(tree_type &&other) noexcept;
  tree_type &operator=(const tree_type &other);
  tree_type &operator=(tree_type &&other) noexcept;
  ~RedBlackTree();

  void Clear() noexcept;
  size_type Size() const noexcept;
  bool Empty() const noexcept;
  size_type MaxSize() const noexcept;

  iterator Begin() noexcept;
  const_iterator Begin() const noexcept;
  iterator End() noexcept;
  const_iterator End() const noexcept;

  void Merge(tree_type &other);
  void MergeUnique(tree_type &other);

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
  void Swap(tree_type &other) noexcept;
  bool CheckTree() const noexcept;

private:
  void CopyTreeFromOther(const tree_type &other);
  tree_node *CopyTree(const tree_node *node, tree_node *parent);
  void Destroy(tree_node *node) noexcept;
  void InitializeHead() noexcept;

  tree_node *&Root();
  const tree_node *Root() const;
  tree_node *&MostLeft();
  const tree_node *MostLeft() const;
  tree_node *&MostRight();

  std::pair<iterator, bool> Insert(tree_node *root, tree_node *new_node,
                                   bool unique_only);
  void BalancingInsert(tree_node *node);
  void RotateRight(tree_node *node) noexcept;
  void RotateLeft(tree_node *node) noexcept;
  tree_node *ExtractNode(iterator pos) noexcept;
  void SwapNodesForErase(tree_node *node, tree_node *other) noexcept;
  void EraseBalancing(tree_node *deleted_node) noexcept;
  tree_node *SearchMinimum(tree_node *node) const noexcept;
  tree_node *SearchMaximum(tree_node *node) const noexcept;
  int ComputeBlackHeight(const tree_node *node) const noexcept;
  bool checkRedNodes(const tree_node *Node) const noexcept;

  struct RedBlackTreeNode {
    RedBlackTreeNode()
        : parent_(nullptr), left_(this), right_(this), key_(key_type{}),
          color_(kRed) {}

    RedBlackTreeNode(const key_type &key)
        : parent_(nullptr), left_(nullptr), right_(nullptr), key_(key),
          color_(kRed) {}

    RedBlackTreeNode(key_type &&key)
        : parent_(nullptr), left_(nullptr), right_(nullptr),
          key_(std::move(key)), color_(kRed) {}

    RedBlackTreeNode(key_type key, tree_color color)
        : parent_(nullptr), left_(nullptr), right_(nullptr), key_(key),
          color_(color) {}

    void ToDefault() noexcept {
      left_ = nullptr;
      right_ = nullptr;
      parent_ = nullptr;
      color_ = kRed;
    }

    tree_node *NextNode() const noexcept {
      tree_node *node = const_cast<tree_node *>(this);
      if (node->color_ == kRed &&
          (node->parent_ == nullptr || node->parent_->parent_ == node)) {
        node = node->left_;
      } else if (node->right_ != nullptr) {
        node = node->right_;

        while (node->left_ != nullptr) {
          node = node->left_;
        }
      } else {
        tree_node *parent = node->parent_;

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

    tree_node *PrevNode() const noexcept {
      tree_node *node = const_cast<tree_node *>(this);

      if (node->color_ == kRed &&
          (node->parent_ == nullptr || node->parent_->parent_ == node)) {
        node = node->right_;
      } else if (node->left_ != nullptr) {
        node = node->left_;
        while (node->right_ != nullptr) {
          node = node->right_;
        }
      } else {
        tree_node *parent = node->parent_;
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
    tree_node *parent_;
    tree_node *left_;
    tree_node *right_;
    key_type key_;
    tree_color color_;
  };

  struct RedBlackTreeIterator {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = tree_type::key_type;
    using pointer = value_type *;
    using reference = value_type &;

    RedBlackTreeIterator() = delete;

    explicit RedBlackTreeIterator(tree_node *node) : node_(node) {}

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

    tree_node *node_;
  };

  struct RedBlackTreeIteratorConst {
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = tree_type::key_type;
    using pointer = const value_type *;
    using reference = const value_type &;

    RedBlackTreeIteratorConst() = delete;

    explicit RedBlackTreeIteratorConst(const tree_node *node) : node_(node) {}

    RedBlackTreeIteratorConst(const iterator &it) : node_(it.node_) {}

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

    const tree_node *node_;
  };

  RedBlackTreeNode *head_;
  size_type size_;
  Comparator cmp_;
};

} // namespace s21
#include "RedBlackTree.tpp"
#endif
