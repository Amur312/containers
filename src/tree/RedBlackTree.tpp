#ifndef REDBLACKTREE_TPP
#define REDBLACKTREE_TPP

#include "RedBlackTree.h"
#include <stack>

namespace s21 {

template <typename Key, typename Value>
struct RedBlackTree<Key, Value>::TreeIterator {
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = Key;
  using pointer = value_type *;
  using reference = value_type &;

  TreeIterator() = delete;

  explicit TreeIterator(TreeNode *node) : node_(node) {}

  reference operator*() const noexcept { return node_->key; }

  TreeIterator &operator++() noexcept {
    node_ = node_->next_node();
    return *this;
  }

  TreeIterator operator++(int) noexcept {
    TreeIterator tmp{node_};
    ++(*this);
    return tmp;
  }

  TreeIterator &operator--() noexcept {
    node_ = node_->prev_node();
    return *this;
  }

  TreeIterator operator--(int) noexcept {
    TreeIterator tmp{node_};
    --(*this);
    return tmp;
  }

  bool operator==(const TreeIterator &other) const noexcept {
    return node_ == other.node_;
  }

  bool operator!=(const TreeIterator &other) const noexcept {
    return node_ != other.node_;
  }

  TreeNode *node_;
};

template <typename Key, typename Value>
struct RedBlackTree<Key, Value>::TreeIteratorConst {
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = Key;
  using pointer = const value_type *;
  using reference = const value_type &;

  TreeIteratorConst() = delete;

  explicit TreeIteratorConst(const TreeNode *node) : node_(node) {}

  TreeIteratorConst(const TreeIteratorConst &it) : node_(it.node_) {}

  reference operator*() const noexcept { return node_->key; }

  TreeIteratorConst &operator++() noexcept {
    node_ = node_->next_node();
    return *this;
  }

  TreeIteratorConst operator++(int) noexcept {
    TreeIteratorConst tmp{node_};
    ++(*this);
    return tmp;
  }

  TreeIteratorConst &operator--() noexcept {
    node_ = node_->prev_node();
    return *this;
  }

  TreeIteratorConst operator--(int) noexcept {
    TreeIteratorConst tmp{node_};
    --(*this);
    return tmp;
  }

  friend bool operator==(const TreeIteratorConst &it1,
                         const TreeIteratorConst &it2) noexcept {
    return it1.node_ == it2.node_;
  }

  friend bool operator!=(const TreeIteratorConst &it1,
                         const TreeIteratorConst &it2) noexcept {
    return it1.node_ != it2.node_;
  }

  const TreeNode *node_;
};

template <typename Key, typename Value>
[[maybe_unused]] int RedBlackTree<Key, Value>::computeBlackHeight(
    const TreeNode *node) const noexcept {
  {
    if (node == nullptr) {
      return 0;
    }
    int left_height = computeBlackHeight(node->left_);
    int right_height = computeBlackHeight(node->right_);
    int add = node->color_ == BLACK ? 1 : 0;
    if (left_height == -1 || right_height == -1 ||
        left_height != right_height) {
      return -1;
    } else {
      return left_height + add;
    }
  }
}
template <typename Key, typename Value>
void RedBlackTree<Key, Value>::eraseBalancing(TreeNode *deleted_node) noexcept {
  TreeNode *check_node = deleted_node;
  TreeNode *parent = deleted_node->parent_;

  while (check_node != head_->parent_ && check_node->color_ == BLACK) {
    if (check_node == parent->left_) {
      TreeNode *sibling = parent->right_;
      if (sibling->color_ == RED) {
        std::swap(sibling->color_, parent->color_);
        RotateLeft(parent);
        parent = check_node->parent_;
        sibling = parent->right_;
      }
      if (sibling->color_ == BLACK &&
          (sibling->left_ == nullptr || sibling->left_->color_ == BLACK) &&
          (sibling->right_ == nullptr || sibling->right_->color_ == BLACK)) {
        sibling->color_ = RED;
        if (parent->color_ == RED) {
          parent->color_ = BLACK;
          break;
        }
        check_node = parent;
        parent = check_node->parent_;
      } else {
        if (sibling->left_ != nullptr && sibling->left_->color_ == RED &&
            (sibling->right_ == nullptr || sibling->right_->color_ == BLACK)) {
          std::swap(sibling->color_, sibling->left_->color_);
          rotateRight(sibling);
          sibling = parent->right_;
        }
        sibling->right_->color_ = BLACK;
        sibling->color_ = parent->color_;
        parent->color_ = BLACK;
        rotateLeft(parent);
        break;
      }
    } else {
      TreeNode *sibling = parent->left_;

      // Случай 6.1
      if (sibling->color_ == RED) {
        std::swap(sibling->color_, parent->color_);
        rotateRight(parent);
        parent = check_node->parent_;
        sibling = parent->left_;
      }

      // Случай 6.2
      if (sibling->color_ == BLACK &&
          (sibling->left_ == nullptr || sibling->left_->color_ == BLACK) &&
          (sibling->right_ == nullptr || sibling->right_->color_ == BLACK)) {
        sibling->color_ = RED;
        if (parent->color_ == RED) {
          parent->color_ = BLACK;
          break;
        }
        check_node = parent;
        parent = check_node->parent_;
      } else {
        if (sibling->right_ != nullptr && sibling->right_->color_ == RED &&
            (sibling->left_ == nullptr || sibling->left_->color_ == BLACK)) {
          // Случай 6.3
          std::swap(sibling->color_, sibling->right_->color_);
          rotateLeft(sibling);
          sibling = parent->left_;
        }
        // Случай 6.4
        sibling->left_->color_ = BLACK;
        sibling->color_ = parent->color_;
        parent->color_ = BLACK;
        rotateRight(parent);
        break;
      }
    }
  }
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::swapNodesForErase(TreeNode *node,
                                                 TreeNode *other) noexcept {
  if (other->parent_->left_ == other) {
    other->parent_->left_ = node;
  } else {
    other->parent_->right_ = node;
  }
  if (node == head_->parent_) {
    head_->parent_ = other;
  } else {
    if (node->parent_->left_ == node) {
      node->parent_->left_ = other;
    } else {
      node->parent_->right_ = other;
    }
  }
  std::swap(node->parent_, other->parent_);
  std::swap(node->left_, other->left_);
  std::swap(node->right_, other->right_);
  std::swap(node->color_, other->color_);

  if (node->left_) {
    node->left_->parent_ = node;
  }

  if (node->right_) {
    node->right_->parent_ = node;
  }

  if (other->left_) {
    other->left_->parent_ = other;
  }

  if (other->right_) {
    other->right_->parent_ = other;
  }
}
template <typename Key, typename Value>
typename RedBlackTree<Key, Value>::TreeNode *
RedBlackTree<Key, Value>::extractNode(iterator pos) noexcept {
  if (pos == cEnd()) {
    return nullptr;
  }

  TreeNode *deleted_node = pos.node_;

  if (deleted_node->left_ != nullptr && deleted_node->right_ != nullptr) {
    TreeNode *replace = searchMinimum(deleted_node->right_);
    swapNodesForErase(deleted_node, replace);
  }

  if (deleted_node->color_ == BLACK &&
      ((deleted_node->left_ == nullptr && deleted_node->right_ != nullptr) ||
       (deleted_node->left_ != nullptr && deleted_node->right_ == nullptr))) {
    TreeNode *replace;
    if (deleted_node->left_ != nullptr) {
      replace = deleted_node->left_;
    } else {
      replace = deleted_node->right_;
    }
    swapNodesForErase(deleted_node, replace);
  }

  if (deleted_node->color_ == BLACK && deleted_node->left_ == nullptr &&
      deleted_node->right_ == nullptr) {
    eraseBalancing(deleted_node);
  }

  if (deleted_node == head_->parent_) {
    initializeRootNode();
  } else {
    if (deleted_node == deleted_node->parent_->left_) {
      deleted_node->parent_->left_ = nullptr;
    } else {
      deleted_node->parent_->right_ = nullptr;
    }

    if (head_->left_ == deleted_node) {
      head_->left_ = searchMinimum(head_->parent_);
    }

    if (head_->right_ == deleted_node) {
      head_->right_ = searchMaximum(head_->parent_);
    }
  }

  --size_;

  deleted_node->ToDefault();

  return deleted_node;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::rotateRight(TreeNode *node) noexcept {
  TreeNode *const pivot = node->left_;
  pivot->parent_ = node->parent_;
  if (node == head_->parent_) {
    head_->parent_ = pivot;
  } else if (node->parent_->left_ == node) {
    node->parent_->left_ = pivot;
  } else {
    node->parent_->right_ = pivot;
  }
  node->left_ = pivot->right_;
  if (pivot->right_ != nullptr) {
    pivot->right_->parent_ = node;
  }
  node->parent_ = pivot;
  pivot->right_ = node;
}
template <typename Key, typename Value>
void RedBlackTree<Key, Value>::rotateLeft(TreeNode *node) noexcept {
  TreeNode *const pivot = node->right_;
  pivot->parent_ = node->parent_;
  if (node == head_->parent_) {
    head_->parent_ = pivot;
  } else if (node->parent_->right_ == node) {
    node->parent_->right_ = pivot;
  } else {
    node->parent_->left_ = pivot;
  }

  node->right_ = pivot->left_;
  if (pivot->left_ != nullptr) {
    pivot->left_->parent_ = node;
  }

  node->parent_ = pivot;
  pivot->left_ = node;
}

template <typename Key, typename Value>
bool RedBlackTree<Key, Value>::checkTree() const noexcept {
  if (head_->color_ == BLACK) {
    return false;
  }

  //
  if (head_->parent_ == nullptr) {
    return true;
  }
  if (head_->parent_->color_ == RED) {
    return false;
  }
  if (checkRedNodes(head_->parent_) == false) {
    return false;
  }
  if (computeBlackHeight(head_->parent_) == -1) {
    return false;
  }

  return true;
}

template <typename Key, typename Value>
template <typename... Args>
std::vector<std::pair<typename RedBlackTree<Key, Value>::iterator, bool>>
RedBlackTree<Key, Value>::emplace(Args &&...args) {
  std::vector<std::pair<iterator, bool>> result;
  result.reserve(sizeof...(args));
  auto insertItem = [&](const auto &item) {
    TreeNode *newNode = new TreeNode(std::move(item));
    std::pair<iterator, bool> result_insert =
        insert(head_->parent_, newNode, false);
    result.push_back(result_insert);
  };

  (insertItem(std::forward<Args>(args)), ...);

  return result;
}

template <typename Key, typename Value>
template <typename... Args>
std::vector<std::pair<typename RedBlackTree<Key, Value>::iterator, bool>>
RedBlackTree<Key, Value>::emplaceUnique(Args &&...args) {
  std::vector<std::pair<iterator, bool>> result;
  result.reserve(sizeof...(args));

  for (auto item : {std::forward<Args>(args)...}) {
    TreeNode *newNode = new TreeNode(std::move(item));
    std::pair<iterator, bool> result_insert =
        insert(head_->parent_, newNode, true);
    if (result_insert.second == false) {
      delete newNode;
    }
    result.push_back(result_insert);
  }
  return result;
}

template <typename Key, typename Value>
typename RedBlackTree<Key, Value>::iterator
RedBlackTree<Key, Value>::find(const_reference key) {
  iterator result = lowerBound(key);

  if (result == cEnd() || key_comparator_(key, *result)) {
    return cEnd();
  }

  return result;
}
template <typename Key, typename Value>
typename RedBlackTree<Key, Value>::iterator
RedBlackTree<Key, Value>::lowerBound(const_reference key) {
  TreeNode *start = head_->parent_;
  TreeNode *result = cEnd().node_;

  while (start != nullptr) {
    if (!key_comparator_(start->key, key)) {
      result = start;
      start = start->left_;
    } else {
      start = start->right_;
    }
  }

  return iterator(result);
}

template <typename Key, typename Value>
typename RedBlackTree<Key, Value>::iterator
RedBlackTree<Key, Value>::upperBound(const_reference key) {
  TreeNode *start = head_->parent_;
  TreeNode *result = cEnd().node_;

  while (start != nullptr) {
    if (key_comparator_(key, start->key_)) {
      result = start;
      start = start->left_;
    } else {
      start = start->right_;
    }
  }

  return iterator(result);
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::erase(iterator pos) noexcept {
  TreeNode *result = extractNode(pos);
  delete result;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::merge(RedBlackTree &other) {
  if (this != &other) {
    iterator other_begin = other.cBegin();

    while (other.size_ > 0) {
      TreeNode *moving_node = other_begin.node_;
      ++other_begin;

      if (moving_node->left_ != nullptr) {
        moving_node->left_->parent_ = moving_node->parent_;
      }

      if (moving_node->right_ != nullptr) {
        moving_node->right_->parent_ = moving_node->parent_;
      }

      if (moving_node->parent_->left_ == moving_node) {
        moving_node->parent_->left_ = nullptr;
      }

      if (moving_node->parent_->right_ == moving_node) {
        moving_node->parent_->right_ = nullptr;
      }

      moving_node->to_default();

      --other.size_;

      insert(head_->parent_, moving_node, false);
    }

    other.initializeRootNode();
  }
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::mergeUnique(RedBlackTree &other) {
  if (this != &other) {
    iterator other_begin = other.cBegin();
    iterator other_end = other.cEnd();

    while (other_begin != other_end) {
      iterator result_it = find(other_begin.node_->key);
      if (result_it == cEnd()) {
        iterator tmp = other_begin;
        ++other_begin;
        TreeNode *moving_node = other.extractNode(tmp);
        Insert(head_->parent_, moving_node, false);
      } else {
        ++other_begin;
      }
    }
  }
}

template <typename Key, typename Value>
std::pair<typename RedBlackTree<Key, Value>::iterator, bool>
RedBlackTree<Key, Value>::insertUnique(const key_type &key) {
  TreeNode *newNode = new TreeNode{key};
  std::pair<iterator, bool> result = insert(head_->parent_, newNode, true);
  if (result.second == false) {
    delete newNode;
  }

  return result;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::balanceInsert(TreeNode *node) {
  while (node != head_->parent_ && node->parent_->color == Color::RED) {
    TreeNode *grandparent = node->parent_->parent_;
    if (node->parent_ == grandparent->left_) {
      TreeNode *uncle = grandparent->right_;
      if (uncle != nullptr && uncle->color == Color::RED) {
        node->parent_->color = Color::BLACK;
        uncle->color = Color::BLACK;
        grandparent->color = Color::RED;
        node = grandparent;
      } else {
        if (node == node->parent_->right_) {
          node = node->parent_;
          rotateLeft(node);
        }
        node->parent_->color = Color::BLACK;
        grandparent->color = Color::RED;
        rotateRight(grandparent);
      }
    } else {
      TreeNode *uncle = grandparent->left_;
      if (uncle != nullptr && uncle->color == Color::RED) {
        node->parent_->color = Color::BLACK;
        uncle->color = Color::BLACK;
        grandparent->color = Color::RED;
        node = grandparent;
      } else {
        if (node == node->parent_->left_) {
          node = node->parent_;
          rotateRight(node);
        }
        node->parent_->color = Color::BLACK;
        grandparent->color = Color::RED;
        rotateLeft(grandparent);
      }
    }
  }
  head_->parent_->color = Color::BLACK;
}

template <typename Key, typename Value>
typename RedBlackTree<Key, Value>::iterator
RedBlackTree<Key, Value>::insert(const key_type &key) {
  TreeNode *newNode = new TreeNode(key);
  auto insert_result = insert(head_->parent_, newNode, false);
  if (!insert_result.second) {
    delete newNode;
  }
  return insert_result.first;
}

template <typename Key, typename Value>
std::pair<typename RedBlackTree<Key, Value>::iterator, bool>
RedBlackTree<Key, Value>::insert(TreeNode *root, TreeNode *newNode,
                                 bool only_unique_values) {
  TreeNode *node = root;
  TreeNode *parent = nullptr;

  while (node != nullptr) {
    parent = node;
    if (key_comparator_  (newNode->key, node->key)) {
      node = node->left_;
    } else {
      if (only_unique_values == false) {
        node = node->right_;
      } else {
        if (key_comparator_(node->key, newNode->key)) {
          node = node->right_;
        } else {
          return {iterator(node), false};
        }
      }
    }
  }

  if (parent != nullptr) {
    newNode->parent_ = parent;
    if (key_comparator_(newNode->key, parent->key)) {
      parent->left_ = newNode;
    } else {
      parent->right_ = newNode;
    }
  } else {

    newNode->color = Color::BLACK;
    newNode->parent_ = head_;
    head_->parent_ = newNode;
  }

  ++size_;
  balanceInsert(newNode);
  return {iterator(newNode), true};
}

template <typename Key, typename Value>
typename RedBlackTree<Key, Value>::iterator
RedBlackTree<Key, Value>::cBegin() noexcept {
  TreeNode *node = head_->parent_;
  while (node && node->left_ != nullptr) {
    node = node->left_;
  }
  return iterator(node);
}

template <typename Key, typename Value>
typename RedBlackTree<Key, Value>::const_iterator
RedBlackTree<Key, Value>::cBegin() const noexcept {
  TreeNode *node = head_->parent_;
  while (node && node->left_ != nullptr) {
    node = node->left_;
  }
  return const_iterator(node);
}

template <typename Key, typename Value>
typename RedBlackTree<Key, Value>::iterator
RedBlackTree<Key, Value>::cEnd() noexcept {
  TreeNode *node = head_->parent_;
  while (node && node->right_ != nullptr) {
    node = node->right_;
  }
  return iterator(node);
}

template <typename Key, typename Value>
typename RedBlackTree<Key, Value>::const_iterator
RedBlackTree<Key, Value>::cEnd() const noexcept {
  TreeNode *node = head_->parent_;
  while (node && node->right_ != nullptr) {
    node = node->right_;
  }
  return const_iterator(node);
}

template <typename Key, typename Value>
bool RedBlackTree<Key, Value>::empty() const noexcept {
  return size_ == 0;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::swap(RedBlackTree &other) noexcept {
  std::swap(head_, other.head_);
  std::swap(size_, other.size_);
  std::swap(key_comparator_, other.key_comparator_);
}

template <typename Key, typename Value>
size_t RedBlackTree<Key, Value>::size() const noexcept {
  return size_;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::copyTreeFrom(const RedBlackTree &other) {
  TreeNode *copiedRoot = copyTree(other.head_->parent_, nullptr);
  clear();
  head_->parent_ = copiedRoot;
  head_->parent_->parent_ = head_;
  head_->left_ = searchMinimum(head_->parent_);
  head_->right_ = searchMaximum(head_->parent_);
  size_ = other.size_;
  key_comparator_ = other.key_comparator_;
}

template <typename Key, typename Value>
typename RedBlackTree<Key, Value>::TreeNode *
RedBlackTree<Key, Value>::copyTree(const TreeNode *node, TreeNode *parent) {
  if (node == nullptr) {
    return nullptr;
  }

  TreeNode *copiedNode = new TreeNode{node->key, node->color, parent};
  copiedNode->left_ = copyTree(node->left_, copiedNode);
  copiedNode->right_ = copyTree(node->right_, copiedNode);
  if (node->left_ != nullptr && node->right_ != nullptr) {
    destroy(copiedNode);
    return nullptr;
  }
  return copiedNode;
}

template <typename Key, typename Value>
typename RedBlackTree<Key, Value>::TreeNode *
RedBlackTree<Key, Value>::searchMinimum(TreeNode *node) const noexcept {
  while (node && node->left_ != nullptr) {
    node = node->left_;
  }
  return node;
}

template <typename Key, typename Value>
typename RedBlackTree<Key, Value>::TreeNode *
RedBlackTree<Key, Value>::searchMaximum(TreeNode *node) const noexcept {
  while (node && node->right_ != nullptr) {
    node = node->right_;
  }
  return node;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::clear() noexcept {
  destroy(head_);
  initializeRootNode();
  size_ = 0;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::destroy(TreeNode *root) noexcept {
  if (root == nullptr) {
    return;
  }
  std::stack<TreeNode *> nodes;
  nodes.push(root);

  while (!nodes.empty()) {
    TreeNode *node = nodes.top();
    nodes.pop();
    if (node->left_) {
      nodes.push(node->left_);
    }
    if (node->right_) {
      nodes.push(node->right_);
    }
    delete node;
  }
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::initializeRootNode() noexcept {
  head_->parent_ = nullptr;
  head_->left_ = head_;
  head_->right_ = head_;
}

} // namespace s21

#endif