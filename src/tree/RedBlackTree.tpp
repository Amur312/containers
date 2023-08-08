#ifndef REDBLACKTREE_TPP
#define REDBLACKTREE_TPP

#include "RedBlackTree.h"
#include <stack>

namespace s21 {
template <typename Key, typename Compare>
struct RedBlackTree<Key, Compare>::TreeIterator {
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = Key;
  using pointer = value_type *;
  using reference = value_type &;
  friend class RedBlackTree;

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

private:
  TreeNode *node_;
};

template <typename Key, typename Compare>
struct RedBlackTree<Key, Compare>::TreeIteratorConst {
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = Key;
  using pointer = const value_type *;
  using reference = const value_type &;
  friend class RedBlackTree;
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

private:
  const TreeNode *node_;
};

template <typename Key, typename Compare>
int RedBlackTree<Key, Compare>::calculateBlackNodeHeight(
    const TreeNode *node) const noexcept {
  if (node == nullptr) {
    return 0;
  }
  int leftHeight = calculateBlackNodeHeight(node->left_);
  int rightHeight = calculateBlackNodeHeight(node->right_);
  int blackNodeIncrement = node->color == BLACK ? 1 : 0;
  if (leftHeight == -1 || rightHeight == -1 || leftHeight != rightHeight) {
    return -1;
  } else {
    return leftHeight + blackNodeIncrement;
  }
}

template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::handleRedSibling(TreeNode *sibling,
                                                  TreeNode *parent,
                                                  bool isLeftChild) {
  std::swap(sibling->color, parent->color);
  isLeftChild ? rotateLeft(parent) : rotateRight(parent);
}

template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::handleBlackSibling(TreeNode *sibling,
                                                    TreeNode *parent,
                                                    TreeNode *&currentNode,
                                                    bool isLeftChild) {
  bool leftIsBlack =
      sibling->left_ == nullptr || sibling->left_->color == BLACK;
  bool rightIsBlack =
      sibling->right_ == nullptr || sibling->right_->color == BLACK;

  if (leftIsBlack && rightIsBlack) {
    sibling->color = RED;
    if (parent->color == RED) {
      parent->color = BLACK;
    } else {
      currentNode = parent;
    }
  } else {
    if (isLeftChild && sibling->left_->color == RED && rightIsBlack) {
      std::swap(sibling->color, sibling->left_->color);
      rotateRight(sibling);
    } else if (!isLeftChild && sibling->right_->color == RED && leftIsBlack) {
      std::swap(sibling->color, sibling->right_->color);
      rotateLeft(sibling);
    }
    sibling->color = parent->color;
    parent->color = BLACK;
    isLeftChild ? sibling->right_->color = BLACK
                : sibling->left_->color = BLACK;
    isLeftChild ? rotateLeft(parent) : rotateRight(parent);
  }
}

template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::eraseBalancing(
    TreeNode *deletedNode) noexcept {
  TreeNode *currentNode = deletedNode;
  TreeNode *parent = deletedNode->parent_;

  while (currentNode != head_->parent_ && currentNode->color == BLACK) {
    bool isLeftChild = currentNode == parent->left_;
    TreeNode *sibling = isLeftChild ? parent->right_ : parent->left_;

    if (sibling->color == RED) {
      handleRedSibling(sibling, parent, isLeftChild);
      parent = currentNode->parent_;
      sibling = isLeftChild ? parent->right_ : parent->left_;
    }

    handleBlackSibling(sibling, parent, currentNode, isLeftChild);

    parent = currentNode->parent_;
    sibling = isLeftChild ? parent->right_ : parent->left_;
  }
}

template <typename Key, typename Compare>
bool RedBlackTree<Key, Compare>::checkRedNodes(
    const TreeNode *Node) const noexcept {
  if (Node == nullptr) {
    return true;
  }
  if (Node->color == RED &&
      ((Node->left_ != nullptr && Node->left_->color_ == RED) ||
       (Node->right_ != nullptr && Node->right_->color_ == RED))) {
    return false;
  }
  return checkRedNodes(Node->left_) && checkRedNodes(Node->right_);
}
template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::swapNodesForErase(TreeNode *node,
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
  std::swap(node->color, other->color);

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
template <typename Key, typename Compare>
typename RedBlackTree<Key, Compare>::TreeNode *
RedBlackTree<Key, Compare>::extractNode(iterator pos) noexcept {
  if (pos == cEnd()) {
    return nullptr;
  }

  TreeNode *deleted_node = pos.node_;

  if (deleted_node->left_ != nullptr && deleted_node->right_ != nullptr) {
    TreeNode *replace = searchMinimum(deleted_node->right_);
    swapNodesForErase(deleted_node, replace);
  }

  if (deleted_node->color == BLACK &&
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

  if (deleted_node->color == BLACK && deleted_node->left_ == nullptr &&
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

  deleted_node->to_default();

  return deleted_node;
}

template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::rotateRight(TreeNode *node) noexcept {
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
template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::rotateLeft(TreeNode *node) noexcept {
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

template <typename Key, typename Compare>
bool RedBlackTree<Key, Compare>::checkTree() const noexcept {
  if (head_->color == BLACK) {
    return false;
  }

  //
  if (head_->parent_ == nullptr) {
    return true;
  }
  if (head_->parent_->color == RED) {
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

template <typename Key, typename Compare>
template <typename... Args>
std::vector<std::pair<typename RedBlackTree<Key, Compare>::iterator, bool>>
RedBlackTree<Key, Compare>::emplace(Args &&...args) {
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

template <typename Key, typename Compare>
template <typename... Args>
std::vector<std::pair<typename RedBlackTree<Key, Compare>::iterator, bool>>
RedBlackTree<Key, Compare>::emplaceUnique(Args &&...args) {
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

template <typename Key, typename Compare>
typename RedBlackTree<Key, Compare>::iterator
RedBlackTree<Key, Compare>::find(const_reference key) {
  iterator result = lowerBound(key);

  if (result == cEnd() || key_comparator_(key, *result)) {
    return cEnd();
  }

  return result;
}
template <typename Key, typename Compare>
typename RedBlackTree<Key, Compare>::iterator
RedBlackTree<Key, Compare>::lowerBound(const_reference key) {
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

template <typename Key, typename Compare>
typename RedBlackTree<Key, Compare>::iterator
RedBlackTree<Key, Compare>::upperBound(const_reference key) {
  TreeNode *start = head_->parent_;
  TreeNode *result = cEnd().node_;

  while (start != nullptr) {
    if (key_comparator_(key, start->key)) {
      result = start;
      start = start->left_;
    } else {
      start = start->right_;
    }
  }

  return iterator(result);
}

template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::erase(iterator pos) noexcept {
  TreeNode *result = extractNode(pos);
  delete result;
}

template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::merge(RedBlackTree &other) {
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

template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::mergeUnique(RedBlackTree &other) {
  if (this != &other) {
    iterator other_begin = other.cBegin();
    iterator other_end = other.cEnd();

    while (other_begin != other_end) {
      iterator result_it = find(other_begin.node_->key);
      if (result_it == cEnd()) {
        iterator tmp = other_begin;
        ++other_begin;
        TreeNode *moving_node = other.extractNode(tmp);
        insert(head_->parent_, moving_node, false);
      } else {
        ++other_begin;
      }
    }
  }
}

template <typename Key, typename Compare>
std::pair<typename RedBlackTree<Key, Compare>::iterator, bool>
RedBlackTree<Key, Compare>::insertUnique(const key_type &key) {
  TreeNode *newNode = new TreeNode{key};
  std::pair<iterator, bool> result = insert(head_->parent_, newNode, true);
  if (result.second == false) {
    delete newNode;
  }

  return result;
}

template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::balanceInsert(TreeNode *node) {
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

template <typename Key, typename Compare>
typename RedBlackTree<Key, Compare>::iterator
RedBlackTree<Key, Compare>::insert(const key_type &key) {
  TreeNode *newNode = new TreeNode{key};
  return insert(head_->parent_, newNode, false).first;
}

template <typename Key, typename Compare>
std::pair<typename RedBlackTree<Key, Compare>::iterator, bool>
RedBlackTree<Key, Compare>::insert(TreeNode *root, TreeNode *newNode,
                                   bool unique_only) {
  TreeNode *node = root;
  TreeNode *parent = nullptr;

  while (node != nullptr) {
    parent = node;
    if (key_comparator_(newNode->key, node->key)) {

      node = node->left_;
    } else {

      if (unique_only == false) {

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

    newNode->color = BLACK;
    newNode->parent_ = head_;
    head_->parent_ = newNode;
  }

  ++size_;

  if (head_->left_ == head_ || head_->left_->left_ != nullptr) {
    head_->left_ = newNode;
  }

  if (head_->right_ == head_ || head_->right_->right_ != nullptr) {
    head_->right_ = newNode;
  }

  balanceInsert(newNode);

  return {iterator(newNode), true};
}

template <typename Key, typename Compare>
typename RedBlackTree<Key, Compare>::iterator
RedBlackTree<Key, Compare>::cBegin() noexcept {
  TreeNode *node = head_->parent_;
  while (node && node->left_ != nullptr) {
    node = node->left_;
  }
  return iterator(node);
}

template <typename Key, typename Compare>
typename RedBlackTree<Key, Compare>::const_iterator
RedBlackTree<Key, Compare>::cBegin() const noexcept {
  TreeNode *node = head_->parent_;
  while (node && node->left_ != nullptr) {
    node = node->left_;
  }
  return const_iterator(node);
}

template <typename Key, typename Compare>
typename RedBlackTree<Key, Compare>::iterator
RedBlackTree<Key, Compare>::cEnd() noexcept {
  TreeNode *node = head_->parent_;
  while (node && node->right_ != nullptr) {
    node = node->right_;
  }
  return iterator(node);
}

template <typename Key, typename Compare>
typename RedBlackTree<Key, Compare>::const_iterator
RedBlackTree<Key, Compare>::cEnd() const noexcept {
  TreeNode *node = head_->parent_;
  while (node && node->right_ != nullptr) {
    node = node->right_;
  }
  return const_iterator(node);
}

template <typename Key, typename Compare>
bool RedBlackTree<Key, Compare>::empty() const noexcept {
  return size_ == 0;
}

template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::swap(RedBlackTree &other) noexcept {
  std::swap(head_, other.head_);
  std::swap(size_, other.size_);
  std::swap(key_comparator_, other.key_comparator_);
}

template <typename Key, typename Compare>
size_t RedBlackTree<Key, Compare>::size() const noexcept {
  return size_;
}

template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::copyTreeFrom(const RedBlackTree &other) {
  TreeNode *copiedRoot = copyTree(other.head_->parent_, nullptr);
  clear();
  head_->parent_ = copiedRoot;
  head_->parent_->parent_ = head_;
  head_->left_ = searchMinimum(head_->parent_);
  head_->right_ = searchMaximum(head_->parent_);
  size_ = other.size_;
  key_comparator_ = other.key_comparator_;
}

template <typename Key, typename Compare>
typename RedBlackTree<Key, Compare>::TreeNode *
RedBlackTree<Key, Compare>::copyTree(const TreeNode *node, TreeNode *parent) {
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

template <typename Key, typename Compare>
typename RedBlackTree<Key, Compare>::TreeNode *
RedBlackTree<Key, Compare>::searchMinimum(TreeNode *node) const noexcept {
  while (node && node->left_ != nullptr) {
    node = node->left_;
  }
  return node;
}

template <typename Key, typename Compare>
typename RedBlackTree<Key, Compare>::TreeNode *
RedBlackTree<Key, Compare>::searchMaximum(TreeNode *node) const noexcept {
  while (node && node->right_ != nullptr) {
    node = node->right_;
  }
  return node;
}

template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::clear() noexcept {
  destroy(head_->parent_);
  initializeRootNode();
  size_ = 0;
}

template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::destroy(TreeNode *root) noexcept {
  if (root == nullptr)
    return;
  destroy(root->left_);
  destroy(root->right_);
  delete root;
}

template <typename Key, typename Compare>
void RedBlackTree<Key, Compare>::initializeRootNode() noexcept {
  head_->parent_ = nullptr;
  head_->left_ = head_;
  head_->right_ = head_;
}

} // namespace s21

#endif