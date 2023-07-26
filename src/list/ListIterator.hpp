#ifndef LIST_ITERATOR_H
#define LIST_ITERATOR_H

#include "s21_list.h"

namespace s21
{

    template <typename T>
    class ListIterator
    {
    public:
        using Node = typename list<T>::Node;
        using value_type = T;
        using reference = T &;

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
                if (traversedNode->next_ == nullptr)
                {
                    throw std::out_of_range("Iterator cannot move past the end of the list");
                }
                traversedNode = traversedNode->next_;
            }
            return ListIterator(traversedNode);
        }
        ListIterator operator-(const size_t step) const
        {
            Node *traversedNode = current_node;
            for (size_t i = 0; i < step; i++)
            {
                if (traversedNode->prev == nullptr)
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

    template <typename T>
    class ListConstIterator
    {
    public:
        using Node = const typename list<T>::Node;
        using value_type = T;
        using const_reference = const T &;

        ListConstIterator() : current_node(nullptr) {}
        ListConstIterator(Node *current) : current_node(current) {}
        ListConstIterator(const ListConstIterator &other) : current_node(other.current_node) {}

        const_reference operator*() const
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
                if (traversedNode->next == nullptr)
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
                if (traversedNode->prev == nullptr)
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

} // namespace s21

#endif // LIST_ITERATOR_H