#ifndef LIST_CONST_ITERATOR_H
#define LIST_CONST_ITERATOR_H

#include "s21_list.h"

namespace s21
{

    template <typename T>
    class ListConstIterator
    {
    public:
        using Node = typename list<T>::Node;
        using value_type = T;
        using reference = const T &;
        using iterator = ListConstIterator<T>;

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

        iterator begin() const
        {
            return iterator(head);
        }

        iterator end() const
        {
            return iterator(nullptr);
        }

    private:
        Node *current_node;
    };

} // namespace s21

#endif // LIST_CONST_ITERATOR_H
