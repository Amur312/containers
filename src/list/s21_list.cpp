#include "s21_list.h"

namespace s21
{
    template <typename T>
    list<T>::list() : head(nullptr), tail(nullptr), list_size(0) {}

    template <typename T>
    list<T>::list(size_t n) : head(nullptr), tail(nullptr), list_size(0)
    {
        for (size_t i = 0; i < n; ++i)
        {
            push_back(value_type());
        }
    }

    template <typename T>
    list<T>::list(const std::initializer_list<T> &items)
        : head(nullptr), tail(nullptr), list_size(0)
    {
        for (const T &item : items)
        {
            push_back(item);
        }
    }

    template <typename T>
    list<T>::list(const list<T> &other)
        : head(nullptr), tail(nullptr), list_size(0)
    {
        for (Node *node = other.head; node != nullptr; node = node->next)
        {
            push_back(node->value);
        }
    }

    template <typename T>
    list<T>::~list()
    {
        clear();
    }

    // методы для изменения контейнера

    template <typename T>
    void list<T>::push_back(const_reference value)
    {
        Node *newNode = new Node;
        newNode->value = value;
        newNode->next = nullptr;
        if (head == nullptr)
        {

            newNode->prev = nullptr;
            head = newNode;
            tail = newNode;
        }
        else
        {

            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        list_size++;
    }

    template <typename T>
    void list<T>::push_front(const_reference value)
    {
        Node *newNode = new Node;
        newNode->value = value;
        newNode->next = head;
        newNode->prev = nullptr;
        if (head != nullptr)
        {
            head->prev = newNode;
        }
        head = newNode;
        if (tail == nullptr)
        {
            tail = newNode;
        }
        list_size++;
    }

    template <typename T>
    void list<T>::pop_back()
    {
        if (head == nullptr)
        {
            throw std::out_of_range("list is empty");
        }
        else
        {
            Node *node_to_remove = tail;
            if (head == tail)
                head = nullptr;
            tail = node_to_remove->prev;
            remove_node(node_to_remove);
        }
    }

    template <typename T>
    void list<T>::pop_front()
    {
        if (head == nullptr)
        {
            throw std::out_of_range("list is empty");
        }
        else
        {
            Node *node_to_remove = head;
            head = node_to_remove->next;
            if (tail == node_to_remove)
                tail = nullptr;
            remove_node(node_to_remove);
        }
    }

    template <typename T>
    size_t list<T>::size() const
    {
        return list_size;
    }

    template <typename T>
    bool list<T>::empty() const
    {
        return size() == 0;
    }

    // методы для итерирования по элементам класса

    // Доп методы
    template <typename T>
    void list<T>::clear()
    {
        Node *current = head;
        while (current != nullptr)
        {
            Node *next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
        list_size = 0;
    }

    void list<T>::remove_node(Node *node)
    {
        if (node->prev != nullptr)
            node->prev->next = node->next;
        if (node->next != nullptr)
            node->next->prev = node->prev;
        delete node;
        list_size--;
    }
    
    template <typename T>
typename list<T>::iterator list<T>::insert(iterator pos, const T& value) {
    Node* newNode = new Node;
    newNode->value = value;
    
    if (pos.current_node == head) {
        newNode->next = head;
        newNode->prev = nullptr;
        head->prev = newNode;
        head = newNode;
    } else {
        newNode->next = pos.current_node;
        newNode->prev = pos.current_node->prev;
        pos.current_node->prev->next = newNode;
        pos.current_node->prev = newNode;
    }
    
    list_size++;
    return iterator(newNode);
}

template <typename T>
typename list<T>::iterator list<T>::erase(iterator pos) {
    if (pos.current_node == head) {
        head = head->next;
        delete pos.current_node;
        if (head) {
            head->prev = nullptr;
        }
    } else if (pos.current_node == tail) {
        tail = tail->prev;
        delete pos.current_node;
        if (tail) {
            tail->next = nullptr;
        }
    } else {
        pos.current_node->prev->next = pos.current_node->next;
        pos.current_node->next->prev = pos.current_node->prev;
        Node* nextNode = pos.current_node->next;
        delete pos.current_node;
        return iterator(nextNode);
    }

    list_size--;
    return iterator(nullptr);
}

}