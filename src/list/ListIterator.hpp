#ifndef LIST_ITERATOR_H
#define LIST_ITERATOR_H

#include "s21_list.h" 

namespace s21 {

template <typename T>
class ListIterator {
public:
    using Node = typename list<T>::Node; // assuming Node is defined in list
    using value_type = T;
    using reference = T&;

    ListIterator() : current_node(nullptr) {}
    ListIterator(Node* current) : current_node(current) {}

    reference operator*() {
        if (!current_node) {
            throw std::invalid_argument("Invalid access. Iterator is pointing to end of the list or list is empty.");
        }
        return current_node->value;
    }

    ListIterator& operator++() {
        if (!current_node) {
            throw std::runtime_error("Cannot increment end iterator");
        }
        current_node = current_node->next;
        return *this;
    }

    ListIterator operator++(int) {
        ListIterator iterator = *this;
        ++(*this);
        return iterator;
    }

    ListIterator& operator--() {
        if (current_node->prev != nullptr) {
            current_node = current_node->prev;
            return *this;
        }
        else {
            throw std::out_of_range("Cannot decrement iterator at beginning of list");
        }
    }

    ListIterator operator--(int) {
        ListIterator iterator = *this;
        --(*this);
        return iterator;
    }

private:
    Node* current_node;
};

} // namespace s21

#endif // LIST_ITERATOR_H