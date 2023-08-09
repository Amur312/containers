#include "RedBlackTree.h"

namespace s21 {

template <typename Key, typename Comparator>
RedBlackTree<Key, Comparator>::RedBlackTree(const RedBlackTree &other)
    : RedBlackTree() {
  if (other.Size() > 0) {
    CopyTreeFromOther(other);
  }
}

template <typename Key, typename Comparator>
RedBlackTree<Key, Comparator>::RedBlackTree(RedBlackTree &&other) noexcept
    : RedBlackTree() {
  Swap(other);
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTree &
RedBlackTree<Key, Comparator>::operator=(const RedBlackTree &other) {
  if (this != &other) {
    if (other.Size() > 0) {
      CopyTreeFromOther(other);
    } else {
      Clear();
    }
  }

  return *this;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTree &
RedBlackTree<Key, Comparator>::operator=(RedBlackTree &&other) noexcept {
  Clear();
  Swap(other);
  return *this;
}

template <typename Key, typename Comparator>
RedBlackTree<Key, Comparator>::~RedBlackTree() {
  Clear();
  delete head_;
  head_ = nullptr;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Clear() noexcept {
  Destroy(Root());
  InitializeHead();

  size_ = 0;
}



template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::size_type
RedBlackTree<Key, Comparator>::Size() const noexcept {
  return size_;
}

template <typename Key, typename Comparator>
bool RedBlackTree<Key, Comparator>::Empty() const noexcept {
  return size_ == 0;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::size_type
RedBlackTree<Key, Comparator>::MaxSize() const noexcept {
  return ((std::numeric_limits<size_type>::max() / 2) - sizeof(RedBlackTree) -
          sizeof(RedBlackTreeNode)) /
         sizeof(RedBlackTreeNode);
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::Begin() noexcept {
  return iterator(MostLeft());
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::const_iterator
RedBlackTree<Key, Comparator>::Begin() const noexcept {
  return const_iterator(MostLeft());
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::End() noexcept {
  return iterator(head_);
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::const_iterator
RedBlackTree<Key, Comparator>::End() const noexcept {
  return const_iterator(head_);
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Merge(RedBlackTree &other) {
  if (this != &other) {
    iterator other_begin = other.Begin();

    while (other.size_ > 0) {
      RedBlackTreeNode *moving_node = other_begin.node_;
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

      moving_node->ToDefault();

      --other.size_;

      Insert(Root(), moving_node, false);
    }

    other.InitializeHead();
  }
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::MergeUnique(RedBlackTree &other) {
  if (this != &other) {
    iterator other_begin = other.Begin();
    iterator other_end = other.End();

    while (other_begin != other_end) {
      iterator result_it = Find(other_begin.node_->key_);
      if (result_it == End()) {
        iterator tmp = other_begin;
        ++other_begin;
        RedBlackTreeNode *moving_node = other.ExtractNode(tmp);
        Insert(Root(), moving_node, false);
      } else {
        ++other_begin;
      }
    }
  }
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::Insert(const key_type &key) {
  auto *new_node = new RedBlackTreeNode{key};
  return Insert(Root(), new_node, false).first;
}

template <typename Key, typename Comparator>
std::pair<typename RedBlackTree<Key, Comparator>::iterator, bool>
RedBlackTree<Key, Comparator>::InsertUnique(const key_type &key) {
  auto *new_node = new RedBlackTreeNode{key};
  std::pair<iterator, bool> result = Insert(Root(), new_node, true);
  if (result.second == false) {
    delete new_node;
  }
  return result;
}

template <typename Key, typename Comparator>
template <typename... Args>
std::vector<std::pair<typename RedBlackTree<Key, Comparator>::iterator, bool>>
RedBlackTree<Key, Comparator>::Emplace(Args &&...args) {
  std::vector<std::pair<iterator, bool>> result;
  result.reserve(sizeof...(args));

  for (auto item : {std::forward<Args>(args)...}) {
    auto *new_node = new RedBlackTreeNode(std::move(item));
    std::pair<iterator, bool> result_insert = Insert(Root(), new_node, false);
    result.push_back(result_insert);
  }
  return result;
}

template <typename Key, typename Comparator>
template <typename... Args>
std::vector<std::pair<typename RedBlackTree<Key, Comparator>::iterator, bool>>
RedBlackTree<Key, Comparator>::EmplaceUnique(Args &&...args) {
  std::vector<std::pair<iterator, bool>> result;
  result.reserve(sizeof...(args));

  for (auto item : {std::forward<Args>(args)...}) {
    auto *new_node = new RedBlackTreeNode(std::move(item));
    std::pair<iterator, bool> result_insert = Insert(Root(), new_node, true);
    if (result_insert.second == false) {
      delete new_node;
    }
    result.push_back(result_insert);
  }
  return result;
}
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::Find(const_reference key) {
  iterator result = LowerBound(key);

  if (result == End() || keyComparator_(key, *result)) {
    return End();
  }

  return result;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::LowerBound(const_reference key) {
  RedBlackTreeNode *start = Root();
  RedBlackTreeNode *result = End().node_;

  while (start != nullptr) {
    if (!keyComparator_(start->key_, key)) {
      result = start;
      start = start->left_;
    } else {
      start = start->right_;
    }
  }

  return iterator(result);
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::UpperBound(const_reference key) {
  RedBlackTreeNode *start = Root();
  RedBlackTreeNode *result = End().node_;

  while (start != nullptr) {
    if (keyComparator_(key, start->key_)) {
      result = start;
      start = start->left_;
    } else {
      start = start->right_;
    }
  }

  return iterator(result);
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Erase(iterator pos) noexcept {
  RedBlackTreeNode *result = ExtractNode(pos);
  delete result;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Swap(RedBlackTree &other) noexcept {
  std::swap(head_, other.head_);
  std::swap(size_, other.size_);
  std::swap(keyComparator_, other.keyComparator_);
}

template <typename Key, typename Comparator>
bool RedBlackTree<Key, Comparator>::CheckTree() const noexcept {
  if (head_->color_ == BLACK) {
    return false;
  }

  if (Root() == nullptr) {
    return true;
  }

  if (Root()->color_ == RED) {
    return false;
  }

  if (CheckRedNodesRecursively(Root()) == false) {
    return false;
  }

  if (ComputeBlackHeight(Root()) == -1) {
    return false;
  }

  return true;
}
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::CopyTreeFromOther(const RedBlackTree &other) {
  RedBlackTreeNode *other_copy_root = CopyTree(other.Root(), nullptr);

  Clear();
  Root() = other_copy_root;
  Root()->parent_ = head_;
  MostLeft() = SearchMinimum(Root());
  MostRight() = SearchMaximum(Root());
  size_ = other.size_;
  keyComparator_ = other.keyComparator_;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::CopyTree(const RedBlackTreeNode *node,
                                        RedBlackTreeNode *parent) {
  auto *copy = new RedBlackTreeNode{node->key_, node->color_};

  try {
    if (node->left_) {
      copy->left_ = CopyTree(node->left_, copy);
    }

    if (node->right_) {
      copy->right_ = CopyTree(node->right_, copy);
    }
  } catch (...) {
    Destroy(copy);
    throw;
  }

  copy->parent_ = parent;
  return copy;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Destroy(RedBlackTreeNode *node) noexcept {
  if (node == nullptr)
    return;
  Destroy(node->left_);
  Destroy(node->right_);
  delete node;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::InitializeHead() noexcept {
  Root() = nullptr;
  MostLeft() = head_;
  MostRight() = head_;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *&
RedBlackTree<Key, Comparator>::Root() {
  return head_->parent_;
}

template <typename Key, typename Comparator>
const typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::Root() const {
  return head_->parent_;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *&
RedBlackTree<Key, Comparator>::MostLeft() {
  return head_->left_;
}

template <typename Key, typename Comparator>
const typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::MostLeft() const {
  return head_->left_;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *&
RedBlackTree<Key, Comparator>::MostRight() {
  return head_->right_;
}
template <typename Key, typename Comparator>
std::pair<typename RedBlackTree<Key, Comparator>::iterator, bool>
RedBlackTree<Key, Comparator>::Insert(RedBlackTreeNode *root, RedBlackTreeNode *new_node,
                                      bool unique_only) {
  RedBlackTreeNode *node = root;
  RedBlackTreeNode *parent = nullptr;

  while (node != nullptr) {
    parent = node;
    if (keyComparator_(new_node->key_, node->key_)) {

      node = node->left_;
    } else {

      if (!unique_only) {

        node = node->right_;
      } else {

        if (keyComparator_(node->key_, new_node->key_)) {

          node = node->right_;
        } else {

          return {iterator(node), false};
        }
      }
    }
  }

  if (parent != nullptr) {

    new_node->parent_ = parent;
    if (keyComparator_(new_node->key_, parent->key_)) {
      parent->left_ = new_node;
    } else {
      parent->right_ = new_node;
    }
  } else {

    new_node->color_ = BLACK;
    new_node->parent_ = head_;
    Root() = new_node;
  }

  ++size_;

  if (MostLeft() == head_ || MostLeft()->left_ != nullptr) {
    MostLeft() = new_node;
  }

  if (MostRight() == head_ || MostRight()->right_ != nullptr) {
    MostRight() = new_node;
  }

  BalancingInsert(new_node);

  return {iterator(new_node), true};
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::BalancingInsert(RedBlackTreeNode *node) {
  RedBlackTreeNode *parent = node->parent_;

  while (node != Root() && parent->color_ == RED) {

    RedBlackTreeNode *gparent = parent->parent_;

    if (gparent->left_ == parent) {

      RedBlackTreeNode *uncle = gparent->right_;

      if (uncle != nullptr && uncle->color_ == RED) {

        parent->color_ = BLACK;
        uncle->color_ = BLACK;
        gparent->color_ = RED;

        node = gparent;
        parent = node->parent_;
      } else {

        if (parent->right_ == node) {

          RotateLeft(parent);

          std::swap(parent, node);
        }

        RotateRight(gparent);
        gparent->color_ = RED;
        parent->color_ = BLACK;
        break;
      }
    } else {

      RedBlackTreeNode *uncle = gparent->left_;

      if (uncle != nullptr && uncle->color_ == RED) {

        parent->color_ = BLACK;
        uncle->color_ = BLACK;
        gparent->color_ = RED;

        node = gparent;
        parent = node->parent_;
      } else {

        if (parent->left_ == node) {
          RotateRight(parent);
          std::swap(parent, node);
        }

        RotateLeft(gparent);
        gparent->color_ = RED;
        parent->color_ = BLACK;
        break;
      }
    }
  }

  Root()->color_ = BLACK;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::RotateRight(RedBlackTreeNode *node) noexcept {
  RedBlackTreeNode *const pivot = node->left_;

  pivot->parent_ = node->parent_;

  if (node == Root()) {

    Root() = pivot;
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

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::RotateLeft(RedBlackTreeNode *node) noexcept {
  RedBlackTreeNode *const pivot = node->right_;

  pivot->parent_ = node->parent_;

  if (node == Root()) {
    Root() = pivot;
  } else if (node->parent_->left_ == node) {
    node->parent_->left_ = pivot;
  } else {
    node->parent_->right_ = pivot;
  }

  node->right_ = pivot->left_;
  if (pivot->left_ != nullptr) {
    pivot->left_->parent_ = node;
  }

  node->parent_ = pivot;
  pivot->left_ = node;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::ExtractNode(iterator pos) noexcept {
  if (pos == End()) {

    return nullptr;
  }

  RedBlackTreeNode *deleted_node = pos.node_;

  if (deleted_node->left_ != nullptr && deleted_node->right_ != nullptr) {

    RedBlackTreeNode *replace = SearchMinimum(deleted_node->right_);

    SwapNodesForErase(deleted_node, replace);
  }

  if (deleted_node->color_ == BLACK &&
      ((deleted_node->left_ == nullptr && deleted_node->right_ != nullptr) ||
       (deleted_node->left_ != nullptr && deleted_node->right_ == nullptr))) {

    RedBlackTreeNode *replace;
    if (deleted_node->left_ != nullptr) {
      replace = deleted_node->left_;
    } else {
      replace = deleted_node->right_;
    }

    SwapNodesForErase(deleted_node, replace);
  }

  if (deleted_node->color_ == BLACK && deleted_node->left_ == nullptr &&
      deleted_node->right_ == nullptr) {
    EraseBalancing(deleted_node);
  }

  if (deleted_node == Root()) {

    InitializeHead();
  } else {

    if (deleted_node == deleted_node->parent_->left_) {
      deleted_node->parent_->left_ = nullptr;
    } else {
      deleted_node->parent_->right_ = nullptr;
    }

    if (MostLeft() == deleted_node) {
      MostLeft() = SearchMinimum(Root());
    }

    if (MostRight() == deleted_node) {
      MostRight() = SearchMaximum(Root());
    }
  }

  --size_;

  deleted_node->ToDefault();

  return deleted_node;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::SwapNodesForErase(
    RedBlackTreeNode *node, RedBlackTreeNode *other) noexcept {

  if (other->parent_->left_ == other) {
    other->parent_->left_ = node;
  } else {
    other->parent_->right_ = node;
  }

  if (node == Root()) {

    Root() = other;
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

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::EraseBalancing(
    RedBlackTreeNode *deleted_node) noexcept {
  RedBlackTreeNode *check_node = deleted_node;
  RedBlackTreeNode *parent = deleted_node->parent_;

  while (check_node != Root() && check_node->color_ == BLACK) {
    if (check_node == parent->left_) {

      RedBlackTreeNode *sibling = parent->right_;

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
          RotateRight(sibling);
          sibling = parent->right_;
        }

        sibling->right_->color_ = BLACK;
        sibling->color_ = parent->color_;
        parent->color_ = BLACK;
        RotateLeft(parent);

        break;
      }
    } else {

      RedBlackTreeNode *sibling = parent->left_;

      if (sibling->color_ == RED) {
        std::swap(sibling->color_, parent->color_);
        RotateRight(parent);
        parent = check_node->parent_;
        sibling = parent->left_;
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
        if (sibling->right_ != nullptr && sibling->right_->color_ == RED &&
            (sibling->left_ == nullptr || sibling->left_->color_ == BLACK)) {

          std::swap(sibling->color_, sibling->right_->color_);
          RotateLeft(sibling);
          sibling = parent->left_;
        }

        sibling->left_->color_ = BLACK;
        sibling->color_ = parent->color_;
        parent->color_ = BLACK;
        RotateRight(parent);

        break;
      }
    }
  }
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::SearchMinimum(RedBlackTreeNode *node) const noexcept {
  while (node->left_ != nullptr) {
    node = node->left_;
  };
  return node;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::SearchMaximum(RedBlackTreeNode *node) const noexcept {
  while (node->right_ != nullptr) {
    node = node->right_;
  };
  return node;
}

template <typename Key, typename Comparator>
int RedBlackTree<Key, Comparator>::ComputeBlackHeight(
    const RedBlackTreeNode *node) const noexcept {

  if (node == nullptr) {
    return 0;
  }

  int left_height = ComputeBlackHeight(node->left_);
  int right_height = ComputeBlackHeight(node->right_);
  int add = node->color_ == BLACK ? 1 : 0;

  if (left_height == -1 || right_height == -1 || left_height != right_height) {
    return -1;
  } else {
    return left_height + add;
  }
}

template <typename Key, typename Comparator>
bool RedBlackTree<Key, Comparator>::CheckRedNodesRecursively(
    const RedBlackTreeNode *Node) const noexcept {
  if (Node->color_ == RED) {
    if (Node->left_ != nullptr && Node->left_->color_ == RED) {
      return false;
    }
    if (Node->right_ != nullptr && Node->right_->color_ == RED) {
      return false;
    }
  }

  if (Node->left_ != nullptr) {
    if (CheckRedNodesRecursively(Node->left_) == false) {
      return false;
    }
  }

  if (Node->right_ != nullptr) {
    if (CheckRedNodesRecursively(Node->right_) == false) {
      return false;
    }
  }

  return true;
}
} // namespace s21
