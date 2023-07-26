#ifndef S21_LIST_H
#define S21_LIST_H

#include <iostream>
#include <limits>

namespace s21
{
    template <typename T>
    class list
    {
    public:
        using value_type = T;
        using reference = T &;
        using const_reference = const T &;

        struct Node
        {
            value_type value;
            Node *next;
            Node *prev;
        };

    public:
        list();
        list(size_t n);
        list(const std::initializer_list<value_type> &items);
        list(const list<T> &other);
        ~list();

        class ListIterator;
        class ListConstIterator;

        void push_back(const_reference value);
        void push_front(const_reference value);
        void pop_back();
        void pop_front();
        void remove_node(Node* node);
        void clear();
        size_t size() const;
        bool empty() const;


        iterator insert(iterator pos, const T& value);
        iterator erase(iterator pos);
    private:
        Node *head;
        Node *tail;
        size_t list_size;
    };
}

#endif
