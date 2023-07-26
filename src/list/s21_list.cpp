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
        if (head != nullptr){
            head -> prev = newNode;
        }
        head = newNode;
        if(tail == nullptr){
            tail = newNode;
        }
        list_size++;
    }

    template <typename T>
    void list<T>::pop_back(){
        if(head == nullptr){
            return;
        }else if(tail == head){
            delete head;
            head  = tail = nullptr;
        }else{
            Node *newTail = tail->prev;
            delete tail;
            tail = newTail;
            tail->next = nullptr;
            list_size--;
        }
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
}