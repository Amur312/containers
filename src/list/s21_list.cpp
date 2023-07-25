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
        for (auto it = other.begin(); it != other.end(); ++it)
        {
            push_back(*it);
        }
    }

    // методы для изменения контейнера

    template <typename T>
    void list<T>::push_back(const value_type &value)
    {
        // Создаем новый узел
        Node *newNode = new Node;
        newNode->value = value;
        newNode->next = nullptr;
        // Проверяем, пуст ли список
        if (head == nullptr)
        {
            // Если список пуст, то новый узел становится первым узлом
            newNode->prev = nullptr;
            head = newNode;
            tail = newNode;
        }
        else
        {   
            // Если список не пуст, то добавляем новый узел в конец списка
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        list_size++;
    }
}