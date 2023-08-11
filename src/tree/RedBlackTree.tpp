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
  Destroy(GetRoot());
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
  return iterator(GetLeftmostNode());
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::const_iterator
RedBlackTree<Key, Comparator>::Begin() const noexcept {
  return const_iterator(GetLeftmostNode());
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

      Insert(GetRoot(), moving_node, false);
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
        Insert(GetRoot(), moving_node, false);
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
  return Insert(GetRoot(), new_node, false).first;
}

template <typename Key, typename Comparator>
std::pair<typename RedBlackTree<Key, Comparator>::iterator, bool>
RedBlackTree<Key, Comparator>::InsertUnique(const key_type &key) {
  auto *new_node = new RedBlackTreeNode{key};
  std::pair<iterator, bool> result = Insert(GetRoot(), new_node, true);
  if (result.second == false) {
    delete new_node;
  }
  return result;
}
template <typename Key, typename Comparator>
std::vector<std::pair<typename RedBlackTree<Key, Comparator>::iterator, bool>>
RedBlackTree<Key, Comparator>::Emplace(const std::vector<Key> &items) {
  std::vector<std::pair<iterator, bool>> insertResults;
  insertResults.reserve(items.size());

  for (const auto &item : items) {
    auto *newNode = new RedBlackTreeNode(item);
    auto insertResult = Insert(GetRoot(), newNode, false);
    insertResults.push_back(insertResult);
  }

  return insertResults;
}

template <typename Key, typename Comparator>
std::vector<std::pair<typename RedBlackTree<Key, Comparator>::iterator, bool>>
RedBlackTree<Key, Comparator>::EmplaceUnique(const std::vector<Key> &items) {
  std::vector<std::pair<iterator, bool>> insertResults;
  insertResults.reserve(items.size());

  for (const auto &item : items) {
    auto *newNode = new RedBlackTreeNode(item);
    auto insertResult = Insert(GetRoot(), newNode, true);

    if (!insertResult.second) {
      delete newNode;
    }

    insertResults.push_back(insertResult);
  }

  return insertResults;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::Find(const_reference key) {
  iterator lowerBound = LowerBound(key);

  if (lowerBound == End() || keyComparator_(key, *lowerBound)) {
    return End();
  }

  return lowerBound;
}
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::LowerBound(const_reference key) {
  RedBlackTreeNode *currentNode = GetRoot();
  RedBlackTreeNode *bestCandidate = End().node_;
  while (currentNode != nullptr) {
    if (!keyComparator_(currentNode->key_, key)) {
      bestCandidate = currentNode;
      currentNode = currentNode->left_;
    } else {
      currentNode = currentNode->right_;
    }
  }

  return iterator(bestCandidate);
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::UpperBound(const_reference key) {
  RedBlackTreeNode *currentNode = GetRoot();
  RedBlackTreeNode *bestCandidate = End().node_;
  while (currentNode != nullptr) {
    if (keyComparator_(key, currentNode->key_)) {
      bestCandidate = currentNode;
      currentNode = currentNode->left_;
    } else {
      currentNode = currentNode->right_;
    }
  }

  return iterator(bestCandidate);
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
  if (GetRoot() == nullptr) {
    return true;
  }

  if (GetRoot()->color_ == RED) {
    return false;
  }

  if (!CheckRedNodesRecursively(GetRoot())) {
    return false;
  }
  if (CalculateBlackNodeHeight(GetRoot()) == -1) {
    return false;
  }

  return true;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::CopyTreeFromOther(
    const RedBlackTree &other) {

  RedBlackTreeNode *copiedRootNode = CopyTree(other.GetRoot(), nullptr);

  Clear();

  GetRoot() = copiedRootNode;
  GetRoot()->parent_ = head_;

  GetLeftmostNode() = FindMinimumValueNode(GetRoot());
  GetRightmostNode() = FindMaximumValueNode(GetRoot());

  size_ = other.size_;
  keyComparator_ = other.keyComparator_;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::CopyTree(const RedBlackTreeNode *GetRoot,
                                        RedBlackTreeNode *parent) {
  if (GetRoot == nullptr) {
    return nullptr;
  }

  std::stack<std::pair<const RedBlackTreeNode *, RedBlackTreeNode **>> nodes;
  RedBlackTreeNode *newRoot = nullptr;
  nodes.emplace(GetRoot, &newRoot);

  while (!nodes.empty()) {
    auto [oldNode, newNodePtr] = nodes.top();
    nodes.pop();

    *newNodePtr = new RedBlackTreeNode{oldNode->key_, oldNode->color_};
    (*newNodePtr)->parent_ = parent;

    if (oldNode->left_) {
      nodes.emplace(oldNode->left_, &(*newNodePtr)->left_);
    }
    if (oldNode->right_) {
      nodes.emplace(oldNode->right_, &(*newNodePtr)->right_);
    }
  }

  return newRoot;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Destroy(RedBlackTreeNode *node) noexcept {
  std::stack<RedBlackTreeNode *> nodes;
  nodes.push(node);

  while (!nodes.empty()) {
    RedBlackTreeNode *current = nodes.top();
    nodes.pop();

    if (current == nullptr) {
      continue;
    }

    nodes.push(current->left_);
    nodes.push(current->right_);

    delete current;
  }
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::InitializeHead() noexcept {
  GetRoot() = nullptr;
  GetLeftmostNode() = head_;
  GetRightmostNode() = head_;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *&
RedBlackTree<Key, Comparator>::GetRoot() {
  return head_->parent_;
}

template <typename Key, typename Comparator>
const typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::GetRoot() const {
  return head_->parent_;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *&
RedBlackTree<Key, Comparator>::GetLeftmostNode() {
  return head_->left_;
}

template <typename Key, typename Comparator>
const typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::GetLeftmostNode() const {
  return head_->left_;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *&
RedBlackTree<Key, Comparator>::GetRightmostNode() {
  return head_->right_;
}
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::FindInsertionPoint(
    RedBlackTreeNode *currentRoot, RedBlackTreeNode *newNode, bool uniqueOnly,
    RedBlackTreeNode *&parentNode) {
  RedBlackTreeNode *currentNode = currentRoot;
  parentNode = nullptr;

  while (currentNode != nullptr) {
    parentNode = currentNode;
    if (keyComparator_(newNode->key_, currentNode->key_)) {
      currentNode = currentNode->left_;
    } else {
      if (!uniqueOnly || keyComparator_(currentNode->key_, newNode->key_)) {
        currentNode = currentNode->right_;
      } else {
        return currentNode;
      }
    }
  }
  return nullptr;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::LinkNewNode(RedBlackTreeNode *newNode,
                                                RedBlackTreeNode *parentNode) {
  newNode->parent_ = parentNode;
  if (parentNode != nullptr) {
    if (keyComparator_(newNode->key_, parentNode->key_)) {
      parentNode->left_ = newNode;
    } else {
      parentNode->right_ = newNode;
    }
  } else {
    newNode->color_ = BLACK;
    newNode->parent_ = head_;
    GetRoot() = newNode;
  }
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::UpdateExtremes(RedBlackTreeNode *newNode) {
  if (GetLeftmostNode() == nullptr || GetLeftmostNode() == head_ || GetLeftmostNode()->left_ != nullptr) {
    GetLeftmostNode() = newNode;
  }

  if (GetRightmostNode() == head_ || GetRightmostNode()->right_ != nullptr) {
    GetRightmostNode() = newNode;
  }
}

template <typename Key, typename Comparator>
std::pair<typename RedBlackTree<Key, Comparator>::iterator, bool>
RedBlackTree<Key, Comparator>::Insert(RedBlackTreeNode *currentRoot,
                                      RedBlackTreeNode *newNode,
                                      bool uniqueOnly) {
  RedBlackTreeNode *parentNode = nullptr;
  RedBlackTreeNode *existingNode =
      FindInsertionPoint(currentRoot, newNode, uniqueOnly, parentNode);
  if (existingNode != nullptr) {
    return {iterator(existingNode), false};
  }

  LinkNewNode(newNode, parentNode);
  UpdateExtremes(newNode);

  ++size_;
  BalancingInsert(newNode);

  return {iterator(newNode), true};
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::BalancingInsert(
    RedBlackTreeNode *currentNode) {
  RedBlackTreeNode *parentNode = currentNode->parent_;

  while (currentNode != GetRoot() && parentNode->color_ == RED) {
    RedBlackTreeNode *grandParentNode = parentNode->parent_;
    bool isParentLeftChild = (grandParentNode->left_ == parentNode);

    RedBlackTreeNode *uncleNode =
        isParentLeftChild ? grandParentNode->right_ : grandParentNode->left_;

    if (uncleNode != nullptr && uncleNode->color_ == RED) {
      parentNode->color_ = BLACK;
      uncleNode->color_ = BLACK;
      grandParentNode->color_ = RED;

      currentNode = grandParentNode;
      parentNode = currentNode->parent_;
    } else {
      if (isParentLeftChild) {
        if (parentNode->right_ == currentNode) {
          RotateLeft(parentNode);
          std::swap(parentNode, currentNode);
        }
        RotateRight(grandParentNode);
      } else {
        if (parentNode->left_ == currentNode) {
          RotateRight(parentNode);
          std::swap(parentNode, currentNode);
        }
        RotateLeft(grandParentNode);
      }
      grandParentNode->color_ = RED;
      parentNode->color_ = BLACK;
      break;
    }
  }

  GetRoot()->color_ = BLACK;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Rotate(RedBlackTreeNode *targetNode,
                                           bool isLeftRotation) noexcept {
  RedBlackTreeNode *pivotNode =
      isLeftRotation ? targetNode->right_ : targetNode->left_;

  pivotNode->parent_ = targetNode->parent_;

  if (targetNode == GetRoot()) {
    GetRoot() = pivotNode;
  } else {
    RedBlackTreeNode *parentNode = targetNode->parent_;
    if (parentNode->left_ == targetNode) {
      parentNode->left_ = pivotNode;
    } else {
      parentNode->right_ = pivotNode;
    }
  }

  if (isLeftRotation) {
    targetNode->right_ = pivotNode->left_;
    if (pivotNode->left_ != nullptr) {
      pivotNode->left_->parent_ = targetNode;
    }
    pivotNode->left_ = targetNode;
  } else {
    targetNode->left_ = pivotNode->right_;
    if (pivotNode->right_ != nullptr) {
      pivotNode->right_->parent_ = targetNode;
    }
    pivotNode->right_ = targetNode;
  }

  targetNode->parent_ = pivotNode;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::RotateLeft(
    RedBlackTreeNode *node) noexcept {
  Rotate(node, true);
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::RotateRight(
    RedBlackTreeNode *node) noexcept {
  Rotate(node, false);
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::ExtractNode(iterator pos) noexcept {
  if (pos == End()) {
    return nullptr;
  }

  RedBlackTreeNode *deletedNode = pos.node_;
  ReplaceNodeWithChildIfRequired(deletedNode);

  if (IsSingleNodeAndBlack(deletedNode)) {
    ReplaceNodeWithChild(deletedNode);
  }

  if (IsLeafAndBlack(deletedNode)) {
    EraseBalancing(deletedNode);
  }

  AdjustTreeAfterDeletion(deletedNode);

  --size_;
  deletedNode->ToDefault();

  return deletedNode;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::ReplaceNodeWithChildIfRequired(
    RedBlackTreeNode *node) {
  if (node->left_ != nullptr && node->right_ != nullptr) {
    RedBlackTreeNode *replace = FindMinimumValueNode(node->right_);
    SwapNodes(node, replace);
  }
}

template <typename Key, typename Comparator>
bool RedBlackTree<Key, Comparator>::IsSingleNodeAndBlack(
    RedBlackTreeNode *node) {
  return node->color_ == BLACK &&
         ((node->left_ == nullptr && node->right_ != nullptr) ||
          (node->left_ != nullptr && node->right_ == nullptr));
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::ReplaceNodeWithChild(
    RedBlackTreeNode *node) {
  RedBlackTreeNode *replace =
      (node->left_ != nullptr) ? node->left_ : node->right_;
  SwapNodes(node, replace);
}

template <typename Key, typename Comparator>
bool RedBlackTree<Key, Comparator>::IsLeafAndBlack(RedBlackTreeNode *node) {
  return node->color_ == BLACK && node->left_ == nullptr &&
         node->right_ == nullptr;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::AdjustTreeAfterDeletion(
    RedBlackTreeNode *deletedNode) {
  if (deletedNode == GetRoot()) {
    InitializeHead();
  } else {
    if (deletedNode == deletedNode->parent_->left_) {
      deletedNode->parent_->left_ = nullptr;
    } else {
      deletedNode->parent_->right_ = nullptr;
    }
    if (GetLeftmostNode() == deletedNode) {
      GetLeftmostNode() = FindMinimumValueNode(GetRoot());
    }
    if (GetRightmostNode() == deletedNode) {
      GetRightmostNode() = FindMaximumValueNode(GetRoot());
    }
  }
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::UpdateParentPointers(
    RedBlackTreeNode *node, RedBlackTreeNode *other) noexcept {
  if (other->parent_->left_ == other)
    other->parent_->left_ = node;
  else
    other->parent_->right_ = node;

  if (node->parent_->left_ == node)
    node->parent_->left_ = other;
  else
    node->parent_->right_ = other;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::UpdateRootIfNeeded(
    RedBlackTreeNode *node, RedBlackTreeNode *other) noexcept {
  if (node == GetRoot())
    GetRoot() = other;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::SwapNodeProperties(
    RedBlackTreeNode *node, RedBlackTreeNode *other) noexcept {
  std::swap(node->parent_, other->parent_);
  std::swap(node->left_, other->left_);
  std::swap(node->right_, other->right_);
  std::swap(node->color_, other->color_);

  if (node->left_)
    node->left_->parent_ = node;
  if (node->right_)
    node->right_->parent_ = node;
  if (other->left_)
    other->left_->parent_ = other;
  if (other->right_)
    other->right_->parent_ = other;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::SwapNodes(
    RedBlackTreeNode *node, RedBlackTreeNode *other) noexcept {
  UpdateParentPointers(node, other);
  UpdateRootIfNeeded(node, other);
  SwapNodeProperties(node, other);
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::HandleSiblingRed(RedBlackTreeNode *sibling,
                                                     RedBlackTreeNode *parent) {
  std::swap(sibling->color_, parent->color_);
  if (sibling == parent->right_) {
    RotateLeft(parent);
  } else {
    RotateRight(parent);
  }
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::HandleSiblingBlack(
    RedBlackTreeNode *node, RedBlackTreeNode *sibling,
    RedBlackTreeNode *parent) {
  sibling->color_ = RED;
  if (parent->color_ == RED) {
    parent->color_ = BLACK;
    return;
  }

  node = parent;
  parent = node->parent_;
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::HandleSiblingBlackRed(
    RedBlackTreeNode *sibling, RedBlackTreeNode *parent) {
  if (sibling->right_ != nullptr && sibling->right_->color_ == RED &&
      (sibling->left_ == nullptr || sibling->left_->color_ == BLACK)) {
    std::swap(sibling->color_, sibling->right_->color_);
    RotateLeft(sibling);
  } else if (sibling->left_ != nullptr && sibling->left_->color_ == RED &&
             (sibling->right_ == nullptr || sibling->right_->color_ == BLACK)) {
    std::swap(sibling->color_, sibling->left_->color_);
    RotateRight(sibling);
  }

  sibling->left_->color_ = BLACK;
  sibling->right_->color_ = BLACK;
  sibling->color_ = parent->color_;
  parent->color_ = BLACK;
  if (sibling == parent->right_) {
    RotateLeft(parent);
  } else {
    RotateRight(parent);
  }
}

template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::EraseBalancing(
    RedBlackTreeNode *deletedNode) noexcept {
  RedBlackTreeNode *nodeToCheck = deletedNode;
  RedBlackTreeNode *parent = deletedNode->parent_;

  while (nodeToCheck != GetRoot() && nodeToCheck->color_ == BLACK) {
    RedBlackTreeNode *sibling =
        nodeToCheck == parent->left_ ? parent->right_ : parent->left_;

    if (sibling->color_ == RED) {
      HandleSiblingRed(sibling, parent);
      sibling = nodeToCheck == parent->left_ ? parent->right_ : parent->left_;
    }

    if (sibling->color_ == BLACK &&
        (sibling->left_ == nullptr || sibling->left_->color_ == BLACK) &&
        (sibling->right_ == nullptr || sibling->right_->color_ == BLACK)) {
      HandleSiblingBlack(nodeToCheck, sibling, parent);
    } else {
      HandleSiblingBlackRed(sibling, parent);
      break;
    }

    parent = nodeToCheck->parent_;
  }
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::FindMinimumValueNode(
    RedBlackTreeNode *currentNode) const noexcept {
  while (currentNode->left_ != nullptr) {
    currentNode = currentNode->left_;
  }
  return currentNode;
}

template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::FindMaximumValueNode(
    RedBlackTreeNode *currentNode) const noexcept {
  while (currentNode->right_ != nullptr) {
    currentNode = currentNode->right_;
  }
  return currentNode;
}

template <typename Key, typename Comparator>
int RedBlackTree<Key, Comparator>::CalculateBlackNodeHeight(
    const RedBlackTreeNode *currentNode) const noexcept {

  if (currentNode == nullptr) {
    return 0;
  }

  int leftBlackHeight = CalculateBlackNodeHeight(currentNode->left_);
  int rightBlackHeight = CalculateBlackNodeHeight(currentNode->right_);

  if (leftBlackHeight == -1 || rightBlackHeight == -1 ||
      leftBlackHeight != rightBlackHeight) {
    return -1;
  }

  return leftBlackHeight + (currentNode->color_ == BLACK ? 1 : 0);
}

template <typename Key, typename Comparator>
bool RedBlackTree<Key, Comparator>::CheckRedNodesRecursively(
    const RedBlackTreeNode *currentNode) const noexcept {
  if (currentNode->color_ == RED) {
    if (currentNode->left_ != nullptr && currentNode->left_->color_ == RED) {
      return false;
    }
    if (currentNode->right_ != nullptr && currentNode->right_->color_ == RED) {
      return false;
    }
  }

  if (currentNode->left_ != nullptr) {
    if (CheckRedNodesRecursively(currentNode->left_) == false) {
      return false;
    }
  }

  if (currentNode->right_ != nullptr) {
    if (CheckRedNodesRecursively(currentNode->right_) == false) {
      return false;
    }
  }

  return true;
}
} // namespace s21
