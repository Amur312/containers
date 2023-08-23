#ifndef S21_LIST_H
#define S21_LIST_H
#include <initializer_list>
#include <iostream>
#include <limits>

namespace s21 {
template <typename T> class List {
public:
  using const_reference = const T &;
  using size_type = size_t;
  using reference = T &;
  using value_type = T;

private:
  struct Node;

public:
  class ConstListIterator {
  public:
    ConstListIterator() noexcept { _node = nullptr; }
    ConstListIterator(Node *node) noexcept : _node(node) {}
    ConstListIterator(const ConstListIterator &other) noexcept
        : _node(other._node) {}
    ConstListIterator(ConstListIterator &&other) noexcept {
      _node = other._node;
      other._node = nullptr;
    }
    ~ConstListIterator() noexcept {}
    bool operator==(const ConstListIterator other) const noexcept;
    bool operator!=(const ConstListIterator other) const noexcept {
      return !operator==(other);
    }
    ConstListIterator operator++() noexcept;
    ConstListIterator operator++(int) noexcept;
    ConstListIterator operator--() noexcept;
    ConstListIterator operator--(int) noexcept;
    const_reference operator*() noexcept;

  public:
    Node *_node;
    value_type error_ = 0;
    reference error_value = error_;
  };
  using const_iterator = List<T>::ConstListIterator;

  class ListIterator : public ConstListIterator {
  public:
    ListIterator() noexcept : ConstListIterator() {}
    ListIterator(Node *node) noexcept : ConstListIterator(node) {}
    ListIterator(const ListIterator &node) noexcept : ConstListIterator(node) {}
    ListIterator(ListIterator &&other) noexcept : ConstListIterator(other) {}
    ~ListIterator() noexcept {}
    reference operator*() noexcept;
    reference operator=(T value) noexcept {
      return this->_node->value_ = value;
    }
    void operator=(const ListIterator &node) noexcept {
      this->_node = node._node;
    }
    void operator=(const_iterator node) noexcept { this->_node = node._node; }
    void operator=(Node *node) noexcept { this->_node = node; }
    void operator=(Node &node) noexcept { this->_node = &node; }
  };
  using iterator = List<T>::ListIterator;

  List() noexcept;
  List(size_type n);
  List(const std::initializer_list<value_type> &items);
  List(const List<T> &other);
  List(List<T> &&other) noexcept;
  ~List() noexcept;
  iterator cBegin() const noexcept;
  iterator cEnd() const noexcept;
  iterator insert(iterator position, const_reference value);
  void operator=(const List<T> &other) noexcept;
  void operator=(List<T> &&other) noexcept;
  void copyList(const List &other) noexcept;
  void moveList(List &other) noexcept;
  void push_back(const_reference value);
  void push_front(const_reference value);
  void pop_back() noexcept;
  void pop_front() noexcept;
  void clear();
  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;
  void endAddress() noexcept;
  void swap(List &other) noexcept;
  void merge(List &other);
  void reverse() noexcept;
  void sort() noexcept;
  void remove_node(Node *node_to_remove);
  const_reference front() noexcept;
  const_reference back() noexcept;
  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args);
  template <typename InputIt>
  iterator insert_range(const_iterator pos, InputIt first, InputIt last);
  template <typename... Args> void insert_many_back(Args &&...args);
  template <typename... Args> void insert_many_front(Args &&...args);

private:
  template <typename First, typename... Rest>
  void insert_front_helper(First &&first, Rest &&...rest);
  struct Node {
    Node() noexcept : next_(nullptr), prev_(nullptr), value_(0) {}
    Node(const_reference t) noexcept
        : value_(t), next_(nullptr), prev_(nullptr) {}
    value_type value_;
    Node *next_;
    Node *prev_;
  };

  Node *_head;
  Node *_tail;
  Node *_end;
  size_type _size;

  value_type error_ = 0;
  reference error_value = error_;
};
} // namespace s21
#include "s21_list.tpp"
#endif // S21_LIST_H
