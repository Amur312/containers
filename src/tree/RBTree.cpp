#include "RBTree.h"
#include <stack>
namespace s21 {

template <typename Key, typename Value>
void RBTree<Key, Value>::insert(const Key &key) {
  TreeNode<key_type, value_type> *node =
      new TreeNode<key_type, value_type>(key);
  TreeNode<key_type, value_type> *temp = root;

  if (root == nullptr) {
    node->color = BLACK;
    root = node;
  } else {
    TreeNode<key_type, value_type> *follow = nullptr;

    while (temp != nullptr) {
      follow = temp;
      if (node->key < temp->key) {
        temp = temp->left_;
      } else {
        temp = temp->right_;
      }
    }
    node->parent_ = follow;
    if (node->key < follow->key) {
      follow->left_ = node;
    } else {
      follow->right_ = node;
    }
  }

  fixInsert(node);
}

template <typename Key, typename Value>
void RBTree<Key, Value>::fixInsert(TreeNode<key_type, value_type> *node) {
  TreeNode<key_type, value_type> *parent = nullptr;
  TreeNode<key_type, value_type> *grandparent = nullptr;
  while (node != root && getColor(node) == RED &&
         getColor(node->parent_) == RED) {
    parent = node->parent_;
    grandparent = node->parent_->parent_;
    if (parent == grandparent->left_) {
      TreeNode<key_type, value_type> *uncle = grandparent->right_;
      if (getColor(uncle) == RED) {
        setColor(uncle, BLACK);
        setColor(parent, BLACK);
        setColor(grandparent, RED);
        node = grandparent;
      } else {
        if (node == parent->right_) {
          rotateLeft(root, parent);
          node = parent;
          parent = node->parent_;
        }
        rotateRight(root, grandparent);
        std::swap(parent->color, grandparent->color);
        node = parent;
      }
    } else {
      TreeNode<key_type, value_type> *uncle = grandparent->left_;
      if (getColor(uncle) == RED) {
        setColor(uncle, BLACK);
        setColor(parent, BLACK);
        setColor(grandparent, RED);
        node = grandparent;
      } else {
        if (node == parent->left_) {
          rotateRight(root, parent);
          node = parent;
          parent = node->parent_;
        }
        rotateLeft(root, grandparent);
        std::swap(parent->color, grandparent->color);
        node = parent;
      }
    }
  }
  setColor(root, BLACK);
}

template <typename Key, typename Value>
void RBTree<Key, Value>::rotateLeft(TreeNode<key_type, value_type> *&root,
                                    TreeNode<key_type, value_type> *&pt) {
  TreeNode<key_type, value_type> *pt_right = pt->right_;
  pt->right_ = pt_right->left_;
  if (pt->right_ != nullptr) {
    pt->right_->parent_ = pt;
  }
  pt_right->parent_ = pt->parent_;
  if (pt->parent_ == nullptr) {
    root = pt_right;
  } else if (pt == pt->parent_->left_) {
    pt->parent_->left_ = pt_right;
  } else {
    pt->parent_->right_ = pt_right;
  }
  pt_right->left_ = pt;
  pt->parent_ = pt_right;
}

template <typename Key, typename Value>
void RBTree<Key, Value>::rotateRight(TreeNode<key_type, value_type> *&root,
                                     TreeNode<key_type, value_type> *&pt) {
  TreeNode<key_type, value_type> *pt_left = pt->left_;
  pt->left_ = pt_left->right_;
  if (pt->left_ != nullptr) {
    pt->left_->parent_ = pt;
  }
  pt_left->parent_ = pt->parent_;
  if (pt->parent_ == nullptr) {
    root = pt_left;
  } else if (pt == pt->parent_->left_) {
    pt->parent_->left_ = pt_left;
  } else {
    pt->parent_->right_ = pt_left;
  }
  pt_left->right_ = pt;
  pt->parent_ = pt_left;
}

template <typename Key, typename Value>
Color RBTree<Key, Value>::getColor(TreeNode<key_type, value_type> *&node) {
  if (node == nullptr) {
    return BLACK;
  } else {
    return node->color;
  }
}

template <typename Key, typename Value>
void RBTree<Key, Value>::setColor(TreeNode<key_type, value_type> *&node,
                                  Color color) {
  if (node == nullptr) {
    return;
  } else {
    node->color = color;
  }
}
template <typename Key, typename Value>
Color RBTree<Key, Value>::getRootColor() const {
  if (root == nullptr)
    return BLACK;
  else
    return root->color;
}

template <typename Key, typename Value>
void RBTree<Key, Value>::clear() noexcept {
  destroy(root);
  initializeHead();
  size_ = 0;
}

template <typename Key, typename Value>
void RBTree<Key, Value>::destroy(
    TreeNode<key_type, value_type> *root) noexcept {
  if (root == nullptr) {
    return;
  }

  std::stack<TreeNode<key_type, value_type> *> nodes;
  nodes.push(root);

  while (!nodes.empty()) {
    TreeNode<key_type, value_type> *node = nodes.top();
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
void RBTree<Key, Value>::initializeHead() noexcept {
  root = nullptr;
}

} // namespace s21