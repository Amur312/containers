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

    public:
        list();
        list(size_t n);
        list(const std::initializer_list<value_type> &items);
        list(const list<T> &other);
        ~list();

        class iterator
        {
        };

        iterator begin();
        iterator end();

        void push_back(const value_type &value);
        void push_front(const value_type &value);
        void pop_back();
        void pop_front();
        void insert(iterator pos, const value_type &value);
        void erase(iterator pos);
        void clear();
        bool empty() const;
        size_t size() const;

    private:
        struct Node
        {
            value_type value;
            Node *next;
            Node *prev;
        };

        Node *head;
        Node *tail;
        size_t list_size;
    };

    class ListIterator
    {
    public:
        ListIterator() : current_node(nullptr) {}
        ListIterator(Node *current) : current_node(current) {}

        reference operator*()
        {
            if (!current_node)
            {
                throw std::invalid_argument("Invalid access. Iterator is pointing to end of the list or list is empty.");
            }
            return current_node->value;
        }

        ListIterator &operator++()
        {
            if (!current_node)
            {
                throw std::runtime_error("Cannot increment end iterator");
            }
            current_node = current_node->next;
            return *this;
        }
        // постфиксная
        ListIterator &operator++(int)
        {
            ListIterator iterator = *this;
            ++(*this);
            return iterator;
        }

        ListIterator &operator--()
        {
            if (current_node->prev != nullptr)
            {
                current_node = current_node->prev;
                return *this;
            }
            else
            {
                throw std::out_of_range("Cannot decrement iterator at beginning of list");
            }
        }

    private:
    }
}

#endif S21_LIST_H
