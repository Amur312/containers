#ifndef CPP2_S21_CONTAINERS_1_SET_H
#define CPP2_S21_CONTAINERS_1_SET_H
#include "../tree/RedBlackTree.h"
#include <cassert>
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
  // Конструкторы и Деструктор
  set();
  set(std::initializer_list<value_type> const &items);
  set(const set &other);
  set(set &&other) noexcept;
  set &operator=(const set &other);
  set &operator=(set &&other) noexcept;
  ~set();
  // Операции сравнения
  friend bool operator==(const set &lhs, const set &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) &&
           lhs.size() == rhs.size();
  }

  friend bool operator!=(const set &lhs, const set &rhs) {
    return !(lhs == rhs);
  }

  // Методы доступа к элементам и информации
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;
  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;

  // Модификация контейнера
  void clear() noexcept;
  std::pair<iterator, bool> insert(const value_type &value);
  void erase(iterator pos) noexcept;
  size_type erase(const key_type &key) noexcept;
  void swap(set &other) noexcept;
  void merge(set &other) noexcept;

  // Поиск и проверка наличия элементов
  iterator find(const key_type &key) noexcept;
  const_iterator find(const key_type &key) const noexcept;
  size_type count(const key_type &key) const noexcept;
  bool contains(const key_type &key) const noexcept;

  // Эффективная вставка нескольких элементов
  template <typename... Args>
  std::vector<std::pair<iterator, bool>> emplace(Args &&...args);

  template <typename InputIt>
  size_type insert_many(InputIt first, InputIt last) noexcept;

private:
  tree_type *tree_;
};

} // namespace s21

#include "s21_set.tpp"
#endif // CPP2_S21_CONTAINERS_1_SET_H
