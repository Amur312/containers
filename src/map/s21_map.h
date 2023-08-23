#ifndef CPP2_S21_CONTAINERS_1_S21_MAP_H
#define CPP2_S21_CONTAINERS_1_S21_MAP_H

#include "../tree/RedBlackTree.h"
#include <stdexcept>


namespace s21 {
template <typename Key, typename Type> class map {
public:
  // Типы данных
  using key_type = Key;
  using mapped_type = Type;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type &;
  using const_reference = const value_type &;

  struct MapKeyComparator {
    template <typename PairType>
    bool operator()(const PairType &lhs, const PairType &rhs) const noexcept {
      return lhs.first < rhs.first;
    }
  };

  using tree_type = RedBlackTree<value_type, MapKeyComparator>;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;
  using size_type = std::size_t;

  // Конструкторы, деструкторы и операторы присваивания
  map();
  map(std::initializer_list<value_type> const &items);
  map(const map &otherMap);
  map(map &&otherMap) noexcept;
  ~map();

  map &operator=(const map &otherMap);
  map &operator=(map &&otherMap) noexcept;

  // Операторы сравнения
  bool operator==(const map &otherMap) const;
  bool operator!=(const map &otherMap) const;

  // Доступ к элементам
  mapped_type &at(const key_type &key);
  const mapped_type &at(const key_type &key) const;
  mapped_type &operator[](const key_type &key);

  // Итераторы
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  iterator end() noexcept;
  const_iterator end() const noexcept;

  // Размеры
  [[nodiscard]] bool empty() const noexcept;
  [[nodiscard]] size_type size() const noexcept;
  [[nodiscard]] size_type max_size() const noexcept;

  // Модификаторы
  void clear() noexcept;
  std::pair<iterator, bool> insert(const value_type &element_to_insert);
  std::pair<iterator, bool> insert(const key_type &key,
                                   const mapped_type &value);
  std::pair<iterator, bool> insert_or_assign(const key_type &key,
                                             const mapped_type &value);
  void erase(iterator pos) noexcept;
  void swap(map &otherMap) noexcept;
  void merge(map &otherMap) noexcept;
  template <typename... Args> std::pair<iterator, bool> emplace(Args &&...args);
  template <typename InputIt> void insert_many(InputIt first, InputIt last);

  // Операции над элементами
  iterator find(const key_type &key) noexcept;
  const_iterator find(const key_type &key) const noexcept;
  size_type count(const key_type &key) const noexcept;
  bool contains(const key_type &key) const noexcept;

private:
  tree_type *tree_;
};

} // namespace s21
#include "s21_map.tpp"
#endif // CPP2_S21_CONTAINERS_1_S21_MAP_H
