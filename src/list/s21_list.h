#ifndef S21_LIST_H
#define S21_LIST_H
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>


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
        class ListIterator;
        class ListConstIterator;

        using iterator = ListIterator;
        using const_iterator = ListConstIterator;
        list();
        list(size_t n);
        list(const std::initializer_list<value_type> &items);
        list(const list<T> &other);
        ~list();

        void push_back(const_reference value);
        void push_front(const_reference value);
        void pop_back();
        void pop_front();
        void remove_node(Node *node);
        void clear();
        size_t size() const;
        bool empty() const;
        const_iterator cbegin() const { return const_iterator(head); }
        const_iterator cend() const { return const_iterator(nullptr); }

        iterator begin() { return iterator(head); }
        iterator end() { return iterator(nullptr); }

        iterator insert(iterator pos, const T &value);
        iterator erase(iterator pos);

        class ListIterator
        {
        public:
            using Node = typename list<T>::Node;
            using value_type = T;
            using reference = T &;
            using iterator = ListIterator;

            ListIterator() : current_node(nullptr) {}
            ListIterator(Node *current) : current_node(current) {}
            ListIterator(const ListIterator &other) : current_node(other.current_node) {}

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

            ListIterator operator++(int)
            {
                ListIterator iterator = *this;
                ++(*this);
                return iterator;
            }

            ListIterator &operator--()
            {
                if (!current_node || !current_node->prev)
                {
                    throw std::out_of_range("Cannot decrement iterator at beginning of list");
                }
                current_node = current_node->prev;
                return *this;
            }

            ListIterator operator--(int)
            {
                ListIterator iterator = *this;
                --(*this);
                return iterator;
            }

            bool operator==(const ListIterator &other) const
            {
                return current_node == other.current_node;
            }
            bool operator!=(const ListIterator &other) const
            {
                return current_node != other.current_node;
            }
            ListIterator &operator=(const ListIterator &other)
            {
                if (this != &other)
                {
                    current_node = other.current_node;
                }
                return *this;
            }
            ListIterator operator+(const size_t step) const
            {
                Node *traversedNode = current_node;
                for (size_t i = 0; i < step; i++)
                {
                    if (!traversedNode || !traversedNode->next)
                    {
                        throw std::out_of_range("Iterator cannot move past the end of the list");
                    }
                    traversedNode = traversedNode->next;
                }
                return ListIterator(traversedNode);
            }
            ListIterator operator-(const size_t step) const
            {
                Node *traversedNode = current_node;
                for (size_t i = 0; i < step; i++)
                {
                    if (!traversedNode || !traversedNode->prev)
                    {
                        throw std::out_of_range("Iterator cannot move past the beginning of the list");
                    }
                    traversedNode = traversedNode->prev;
                }
                return ListIterator(traversedNode);
            }

        private:
            Node *current_node;
        };
        class ListConstIterator
        {
        public:
            using Node = typename list<T>::Node;
            using value_type = T;
            using reference = const T &;
            using iterator = ListConstIterator;

            ListConstIterator() : current_node(nullptr) {}
            ListConstIterator(Node *current) : current_node(current) {}
            ListConstIterator(const ListConstIterator &other) : current_node(other.current_node) {}

            reference operator*() const
            {
                if (!current_node)
                {
                    throw std::invalid_argument("Invalid access. Iterator is pointing to end of the list or list is empty.");
                }
                return current_node->value;
            }

            ListConstIterator &operator++()
            {
                if (!current_node)
                {
                    throw std::runtime_error("Cannot increment end iterator");
                }
                current_node = current_node->next;
                return *this;
            }

            ListConstIterator operator++(int)
            {
                ListConstIterator iterator = *this;
                ++(*this);
                return iterator;
            }

            ListConstIterator &operator--()
            {
                if (!current_node || !current_node->prev)
                {
                    throw std::out_of_range("Cannot decrement iterator at beginning of list");
                }
                current_node = current_node->prev;
                return *this;
            }

            ListConstIterator operator--(int)
            {
                ListConstIterator iterator = *this;
                --(*this);
                return iterator;
            }

            bool operator==(const ListConstIterator &other) const
            {
                return current_node == other.current_node;
            }
            bool operator!=(const ListConstIterator &other) const
            {
                return current_node != other.current_node;
            }
            ListConstIterator &operator=(const ListConstIterator &other)
            {
                if (this != &other)
                {
                    current_node = other.current_node;
                }
                return *this;
            }
            ListConstIterator operator+(const size_t step) const
            {
                Node *traversedNode = current_node;
                for (size_t i = 0; i < step; i++)
                {
                    if (!traversedNode || !traversedNode->next)
                    {
                        throw std::out_of_range("Iterator cannot move past the end of the list");
                    }
                    traversedNode = traversedNode->next;
                }
                return ListConstIterator(traversedNode);
            }
            ListConstIterator operator-(const size_t step) const
            {
                Node *traversedNode = current_node;
                for (size_t i = 0; i < step; i++)
                {
                    if (!traversedNode || !traversedNode->prev)
                    {
                        throw std::out_of_range("Iterator cannot move past the beginning of the list");
                    }
                    traversedNode = traversedNode->prev;
                }
                return ListConstIterator(traversedNode);
            }

        private:
            Node *current_node;
        };

    private:
        Node *head;
        Node *tail;
        size_t list_size;
    };

}
#include "s21_list.tpp"
#endif
