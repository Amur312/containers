#include "RedBlackTree.h"

namespace s21 {

template <typename Key, typename Comparator>
RedBlackTree<Key, Comparator>::RedBlackTree()
    : head_(new tree_node), size_(0U) {}

template <typename Key, typename Comparator>
RedBlackTree<Key, Comparator>::RedBlackTree(const tree_type &other)
    : RedBlackTree() {
  if (other.Size() > 0) {
    CopyTreeFromOther(other);
  }
}

template <typename Key, typename Comparator>
RedBlackTree<Key, Comparator>::RedBlackTree(tree_type &&other) noexcept
    : RedBlackTree() {
  Swap(other);
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::tree_type &
RedBlackTree<Key, Comparator>::operator=(const tree_type &other) {
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
typename RedBlackTree<Key, Comparator>::tree_type &
RedBlackTree<Key, Comparator>::operator=(tree_type &&other) noexcept {
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
  return ((std::numeric_limits<size_type>::max() / 2) - sizeof(tree_type) -
          sizeof(tree_node)) /
         sizeof(tree_node);
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
void RedBlackTree<Key, Comparator>::Merge(tree_type &other) {
  if (this != &other) {
    iterator other_begin = other.Begin();

    while (other.size_ > 0) {
      tree_node *moving_node = other_begin.node_;
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
void RedBlackTree<Key, Comparator>::MergeUnique(tree_type &other) {
  if (this != &other) {
    iterator other_begin = other.Begin();
    iterator other_end = other.End();

    while (other_begin != other_end) {
      iterator result_it = Find(other_begin.node_->key_);
      if (result_it == End()) {
        iterator tmp = other_begin;
        ++other_begin;
        tree_node *moving_node = other.ExtractNode(tmp);
        Insert(Root(), moving_node, false);
      } else {
        ++other_begin;
      }
    }
    other.Clear();
  }
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::Insert(const key_type &key) {
  tree_node *new_node = new tree_node{key};
  return Insert(Root(), new_node, false).first;
}

template <typename Key, typename Comparator>
std::pair<typename RedBlackTree<Key, Comparator>::iterator, bool>
RedBlackTree<Key, Comparator>::InsertUnique(const key_type &key) {
  tree_node *new_node = new tree_node{key};
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

    tree_node *new_node = new tree_node(std::move(item));
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
    tree_node *new_node = new tree_node(std::move(item));
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

  if (result == End() || cmp_(key, *result)) {
    return End();
  }

  return result;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::LowerBound(const_reference key) {
  tree_node *start = Root();
  tree_node *result = End().node_;

  while (start != nullptr) {
    if (!cmp_(start->key_, key)) {
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
  tree_node *start = Root();
  tree_node *result = End().node_;

  while (start != nullptr) {
    if (cmp_(key, start->key_)) {
      result = start;
      start = start->left_;
    } else
      start = start->right_;
  }

  return iterator(result);
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Erase(iterator pos) noexcept {
  tree_node *result = ExtractNode(pos);
  delete result;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Swap(tree_type &other) noexcept {
  std::swap(head_, other.head_);
  std::swap(size_, other.size_);
  std::swap(cmp_, other.cmp_);
}
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::CopyTreeFromOther(const tree_type &other) {
  tree_node *other_copy_root = CopyTree(other.Root(), nullptr);
  Clear();
  Root() = other_copy_root;
  Root()->parent_ = head_;
  MostLeft() = SearchMinimum(Root());
  MostRight() = SearchMaximum(Root());
  size_ = other.size_;
  cmp_ = other.cmp_;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::tree_node *
RedBlackTree<Key, Comparator>::CopyTree(const tree_node *node,
                                        tree_node *parent) {
  tree_node *copy = new tree_node{node->key_, node->color_};
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
void RedBlackTree<Key, Comparator>::Destroy(tree_node *node) noexcept {
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
bool RedBlackTree<Key, Comparator>::CheckTree() const noexcept {
  if (head_->color_ == kBlack) {
    return false;
  }

  if (Root() == nullptr) {
    return true;
  }

  if (Root()->color_ == kRed) {
    return false;
  }

  if (checkRedNodes(Root()) == false) {
    return false;
  }

  if (ComputeBlackHeight(Root()) == -1) {
    return false;
  }

  return true;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::tree_node *&
RedBlackTree<Key, Comparator>::Root() {
  return head_->parent_;
}

template <typename Key, typename Comparator>
const typename RedBlackTree<Key, Comparator>::tree_node *
RedBlackTree<Key, Comparator>::Root() const {
  return head_->parent_;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::tree_node *&
RedBlackTree<Key, Comparator>::MostLeft() {
  return head_->left_;
}

template <typename Key, typename Comparator>
const typename RedBlackTree<Key, Comparator>::tree_node *
RedBlackTree<Key, Comparator>::MostLeft() const {
  return head_->left_;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::tree_node *&
RedBlackTree<Key, Comparator>::MostRight() {
  return head_->right_;
}
template <typename KeyType, typename Compare>
std::pair<typename RedBlackTree<KeyType, Compare>::iterator, bool>
RedBlackTree<KeyType, Compare>::Insert(tree_node *root, tree_node *new_node,
                                       bool unique_only) {
  tree_node *node = root;
  tree_node *parent = nullptr;

  while (node != nullptr) {
    parent = node;
    if (cmp_(new_node->key_, node->key_)) {

      node = node->left_;
    } else {

      if (unique_only == false) {

        node = node->right_;
      } else {

        if (cmp_(node->key_, new_node->key_)) {

          node = node->right_;
        } else {

          return {iterator(node), false};
        }
      }
    }
  }

  if (parent != nullptr) {

    new_node->parent_ = parent;
    if (cmp_(new_node->key_, parent->key_)) {
      parent->left_ = new_node;
    } else {
      parent->right_ = new_node;
    }
  } else {

    new_node->color_ = kBlack;
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
template <typename KeyType, typename Compare>
void RedBlackTree<KeyType, Compare>::BalancingInsert(tree_node *node) {
  tree_node *parent = node->parent_;

  while (node != Root() && parent->color_ == kRed) {
    tree_node *gparent = parent->parent_;

    if (gparent->left_ == parent) {
      tree_node *uncle = gparent->right_;

      if (uncle != nullptr && uncle->color_ == kRed) {
        parent->color_ = kBlack;
        uncle->color_ = kBlack;
        gparent->color_ = kRed;
        node = gparent;
        parent = node->parent_;
      } else {
        if (parent->right_ == node) {
          RotateLeft(parent);
          std::swap(parent, node);
        }
        RotateRight(gparent);
        gparent->color_ = kRed;
        parent->color_ = kBlack;
        break;
      }
    } else {
      tree_node *uncle = gparent->left_;

      if (uncle != nullptr && uncle->color_ == kRed) {
        parent->color_ = kBlack;
        uncle->color_ = kBlack;
        gparent->color_ = kRed;

        node = gparent;
        parent = node->parent_;
      } else {
        if (parent->left_ == node) {
          RotateRight(parent);
          std::swap(parent, node);
        }
        RotateLeft(gparent);
        gparent->color_ = kRed;
        parent->color_ = kBlack;
        break;
      }
    }
  }

  Root()->color_ = kBlack;
}
template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::RotateRight(tree_node *node) noexcept {
  tree_node *const pivot = node->left_;

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

template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::RotateLeft(
    typename RedBlackTree<KeyType, Comparator>::tree_node *node) noexcept {
  tree_node *const pivot = node->right_;

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
template <typename KeyType, typename Comparator>
typename RedBlackTree<KeyType, Comparator>::tree_node *
RedBlackTree<KeyType, Comparator>::ExtractNode(
    typename RedBlackTree<KeyType, Comparator>::iterator pos) noexcept {
  if (pos == End()) {
    return nullptr;
  }

  tree_node *deleted_node = pos.node_;

  if (deleted_node->left_ != nullptr && deleted_node->right_ != nullptr) {
    tree_node *replace = SearchMinimum(deleted_node->right_);
    SwapNodesForErase(deleted_node, replace);
  }

  if (deleted_node->color_ == kBlack &&
      ((deleted_node->left_ == nullptr && deleted_node->right_ != nullptr) ||
       (deleted_node->left_ != nullptr && deleted_node->right_ == nullptr))) {
    tree_node *replace;
    if (deleted_node->left_ != nullptr) {
      replace = deleted_node->left_;
    } else {
      replace = deleted_node->right_;
    }
    SwapNodesForErase(deleted_node, replace);
  }

  if (deleted_node->color_ == kBlack && deleted_node->left_ == nullptr &&
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
template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::SwapNodesForErase(
    typename RedBlackTree<KeyType, Comparator>::tree_node *node,
    typename RedBlackTree<KeyType, Comparator>::tree_node *other) noexcept {
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
template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::EraseBalancing(
    typename RedBlackTree<KeyType, Comparator>::tree_node
        *deleted_node) noexcept {
  tree_node *check_node = deleted_node;
  tree_node *parent = deleted_node->parent_;

  while (check_node != Root() && check_node->color_ == kBlack) {
    if (check_node == parent->left_) {
      tree_node *sibling = parent->right_;

      if (sibling->color_ == kRed) {
        std::swap(sibling->color_, parent->color_);
        RotateLeft(parent);
        parent = check_node->parent_;
        sibling = parent->right_;
      }

      if (sibling->color_ == kBlack &&
          (sibling->left_ == nullptr || sibling->left_->color_ == kBlack) &&
          (sibling->right_ == nullptr || sibling->right_->color_ == kBlack)) {
        sibling->color_ = kRed;
        if (parent->color_ == kRed) {
          parent->color_ = kBlack;
          break;
        }
        check_node = parent;
        parent = check_node->parent_;
      } else {
        if (sibling->left_ != nullptr && sibling->left_->color_ == kRed &&
            (sibling->right_ == nullptr || sibling->right_->color_ == kBlack)) {
          std::swap(sibling->color_, sibling->left_->color_);
          RotateRight(sibling);
          sibling = parent->right_;
        }

        sibling->right_->color_ = kBlack;
        sibling->color_ = parent->color_;
        parent->color_ = kBlack;
        RotateLeft(parent);
        break;
      }
    } else {
      tree_node *sibling = parent->left_;

      if (sibling->color_ == kRed) {
        std::swap(sibling->color_, parent->color_);
        RotateRight(parent);
        parent = check_node->parent_;
        sibling = parent->left_;
      }

      if (sibling->color_ == kBlack &&
          (sibling->left_ == nullptr || sibling->left_->color_ == kBlack) &&
          (sibling->right_ == nullptr || sibling->right_->color_ == kBlack)) {
        sibling->color_ = kRed;
        if (parent->color_ == kRed) {
          parent->color_ = kBlack;
          break;
        }
        check_node = parent;
        parent = check_node->parent_;
      } else {
        if (sibling->right_ != nullptr && sibling->right_->color_ == kRed &&
            (sibling->left_ == nullptr || sibling->left_->color_ == kBlack)) {
          std::swap(sibling->color_, sibling->right_->color_);
          RotateLeft(sibling);
          sibling = parent->left_;
        }
        sibling->left_->color_ = kBlack;
        sibling->color_ = parent->color_;
        parent->color_ = kBlack;
        RotateRight(parent);
        break;
      }
    }
  }
}
template <typename KeyType, typename Comparator>
typename RedBlackTree<KeyType, Comparator>::tree_node *
RedBlackTree<KeyType, Comparator>::SearchMinimum(
    typename RedBlackTree<KeyType, Comparator>::tree_node *node)
    const noexcept {
  while (node->left_ != nullptr) {
    node = node->left_;
  };
  return node;
}

template <typename KeyType, typename Compare>
typename RedBlackTree<KeyType, Compare>::tree_node *
RedBlackTree<KeyType, Compare>::SearchMaximum(tree_node *node) const noexcept {
  while (node->right_ != nullptr) {
    node = node->right_;
  };
  return node;
}

template <typename KeyType, typename Compare>
int RedBlackTree<KeyType, Compare>::ComputeBlackHeight(
    const tree_node *node) const noexcept {
  if (node == nullptr) {
    return 0;
  }

  int left_height = ComputeBlackHeight(node->left_);
  int right_height = ComputeBlackHeight(node->right_);
  int add = node->color_ == kBlack ? 1 : 0;
  if (left_height == -1 || right_height == -1 || left_height != right_height) {
    return -1;
  } else {
    return left_height + add;
  }
}
template <typename KeyType, typename Compare>
bool RedBlackTree<KeyType, Compare>::checkRedNodes(
    const tree_node *Node) const noexcept {
  if (Node->color_ == kRed) {
    if (Node->left_ != nullptr && Node->left_->color_ == kRed) {
      return false;
    }
    if (Node->right_ != nullptr && Node->right_->color_ == kRed) {
      return false;
    }
  }

  if (Node->left_ != nullptr) {
    if (checkRedNodes(Node->left_) == false) {
      return false;
    }
  }

  if (Node->right_ != nullptr) {
    if (checkRedNodes(Node->right_) == false) {
      return false;
    }
  }

  return true;
}

} // namespace s21
