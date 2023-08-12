
#ifndef CPP2_S21_CONTAINERS_1_SET_H
#define CPP2_S21_CONTAINERS_1_SET_H
#include "../tree/RedBlackTree.h"
#include <list>
#include <vector>

namespace s21 {
template <class Key> class set {
public:
  using key_type = Key;
  using value_type = key_type;
  using reference = value_type &;
  using const_reference = const value_type &;
  using tree_type = RedBlackTree<value_type>;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;
  using size_type = std::size_t;

  set() : tree_(new tree_type{}) {}

  set(std::initializer_list<value_type> const &items) : set() {
    for (auto item : items) {
      insert(item);
    }
  }

  set(const set &other) : tree_(new tree_type(*other.tree_)) {}

  set(set &&other) noexcept : tree_(new tree_type(std::move(*other.tree_))) {}

  set &operator=(const set &other) {
    *tree_ = *other.tree_;
    return *this;
  }

  set &operator=(set &&other) noexcept {
    *tree_ = std::move(*other.tree_);
    return *this;
  }
  friend bool operator==(const set &lhs, const set &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) &&
           lhs.size() == rhs.size();
  }

  friend bool operator!=(const set &lhs, const set &rhs) {
    return !(lhs == rhs);
  }
  ~set() {
    delete tree_;
    tree_ = nullptr;
  }

  iterator begin() noexcept { return tree_->Begin(); }

  const_iterator begin() const noexcept { return tree_->Begin(); }

  iterator end() noexcept { return tree_->End(); }

  const_iterator end() const noexcept { return tree_->End(); }

  bool empty() const noexcept { return tree_->Empty(); }

  size_type size() const noexcept { return tree_->Size(); }

  size_type max_size() const noexcept { return tree_->MaxSize(); }

  void clear() noexcept { tree_->Clear(); }

  std::pair<iterator, bool> insert(const value_type &value) {
    return tree_->InsertUnique(value);
  }

  void erase(iterator pos) noexcept { tree_->Erase(pos); }
  size_type erase(const key_type &key) noexcept {
    auto it = find(key);
    if (it != end()) {
      erase(it);
      return 1;
    }
    return 0;
  }

  void swap(set &other) noexcept { tree_->Swap(*other.tree_); }

  void merge(set &other) noexcept { tree_->MergeUnique(*other.tree_); }

  iterator find(const key_type &key) noexcept { return tree_->Find(key); }

  const_iterator find(const key_type &key) const noexcept {
    return tree_->Find(key);
  }
  size_type count(const key_type &key) const noexcept {
    return contains(key) ? 1 : 0;
  }

  bool contains(const key_type &key) const noexcept {
    return tree_->Find(key) != tree_->End();
  }

  template <typename... Args>
  std::vector<std::pair<iterator, bool>> emplace(Args &&...args) {
    return tree_->EmplaceUnique(std::forward<Args>(args)...);
  }

private:
  tree_type *tree_;
};

} // namespace s21
#endif // CPP2_S21_CONTAINERS_1_SET_H
