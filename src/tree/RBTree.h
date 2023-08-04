#ifndef CPP2_S21_CONTAINERS_1_RBTREE_H
#define CPP2_S21_CONTAINERS_1_RBTREE_H

#include <algorithm>

namespace s21 {
// Определение цвета для узла дерева
enum Color { RED, BLACK };

// Структура TreeNode для узлов красно-черного дерева
template <typename Key, typename Value> struct TreeNode {
  Key key;
  Value value;
  Color color;
  TreeNode *parent_;
  TreeNode *left_;
  TreeNode *right_;

  // Конструкторы для TreeNode
  explicit TreeNode(const Key &key)
      : key(key), color(Color::RED), parent_(nullptr), left_(nullptr),
        right_(nullptr) {}

  explicit TreeNode(Key &&key)
      : key(std::move(key)), color(Color::RED), parent_(nullptr),
        left_(nullptr), right_(nullptr) {}

  TreeNode(const Key &key, Color color)
      : key(key), color(color), parent_(nullptr), left_(nullptr),
        right_(nullptr) {}

  // Метод возвращает узел к своему значению по умолчанию
  void to_default() noexcept {
    left_ = nullptr;
    right_ = nullptr;
    parent_ = nullptr;
    color = Color::RED;
  }

  // Метод возвращает следующий узел в дереве
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
};
// Класс RBTree реализует функциональность красно-черного дерева
template <typename Key, typename Value> class RBTree {
public:
  using key_type = Key;
  using value_type = Value;
  using reference = key_type &;
  using const_reference = const key_type &;
  using size_type = std::size_t;

private:
  TreeNode<key_type, value_type> *root;
  TreeNode<key_type, value_type> *head_;
  size_type size_;

public:
  RBTree() : root(nullptr), head_(nullptr), size_(0U) {}

  ~RBTree() {
    clear();
    if (head_ != nullptr) {
      delete head_;
      head_ = nullptr;
    }
  }


  // Реализация итератора для красно-черного дерева
  class iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = TreeNode<Key, Value>;
    using pointer = TreeNode<Key, Value> *;
    using reference = TreeNode<Key, Value> &;

    iterator(pointer ptr) : ptr_(ptr) {}

    reference operator*() const { return *ptr_; }
    pointer operator->() { return ptr_; }

    // Prefix increment
    iterator &operator++() {
      ptr_ = ptr_->next_node();
      return *this;
    }

    // Postfix increment
    iterator operator++(int) {
      iterator temp = *this;
      ++(*this);
      return temp;
    }

    friend bool operator==(const iterator &a, const iterator &b) {
      return a.ptr_ == b.ptr_;
    };
    friend bool operator!=(const iterator &a, const iterator &b) {
      return a.ptr_ != b.ptr_;
    };

  private:
    pointer ptr_;
  };

  class const_iterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const TreeNode<Key, Value>;
    using pointer = const TreeNode<Key, Value> *;
    using reference = const TreeNode<Key, Value> &;

    const_iterator(pointer ptr) : ptr_(ptr) {}

    reference operator*() const { return *ptr_; }
    pointer operator->() { return ptr_; }

    // Prefix increment
    const_iterator &operator++() {
      ptr_ = ptr_->next_node();
      return *this;
    }

    // Postfix increment
    const_iterator operator++(int) {
      const_iterator temp = *this;
      ++(*this);
      return temp;
    }

    friend bool operator==(const const_iterator &a, const const_iterator &b) {
      return a.ptr_ == b.ptr_;
    };
    friend bool operator!=(const const_iterator &a, const const_iterator &b) {
      return a.ptr_ != b.ptr_;
    };

  private:
    pointer ptr_;
  };

  // Методы для работы с красно-черным деревом
  void fixInsert(TreeNode<key_type, value_type> *node);
  Color getColor(TreeNode<key_type, value_type> *&node);
  void setColor(TreeNode<key_type, value_type> *&node, Color color);
  iterator begin() {
    TreeNode<key_type, value_type> *node = root;
    while (node->left_ != nullptr) {
      node = node->left_;
    }
    return iterator(node);
  }

  iterator end() { return iterator(nullptr); }

  const_iterator begin() const {
    TreeNode<key_type, value_type> *node = root;
    while (node->left_ != nullptr) {
      node = node->left_;
    }
    return const_iterator(node);
  }

  const_iterator end() const { return const_iterator(nullptr); }
  void insert(const Key &key);
  Color getRootColor() const;

  void clear() noexcept;
  void destroy(TreeNode<key_type, value_type> *root) noexcept;
  void initializeHead() noexcept;

private:
  void rotateLeft(TreeNode<key_type, value_type> *&root,
                  TreeNode<key_type, value_type> *&pt);
  void rotateRight(TreeNode<key_type, value_type> *&root,
                   TreeNode<key_type, value_type> *&pt);
};

}; // namespace s21
#include "RBTree.cpp"
#endif // CPP2_S21_CONTAINERS_1_RBTREE_H