#ifndef S21_LIST_H
#define S21_LIST_H
#include <initializer_list>
#include <iostream>
#include <limits>

namespace s21
{
    template <typename T>
    class List
    {
    public:
        using const_reference = const T &;
        using size_type = size_t;
        using reference = T &;
        using value_type = T;

    private:
        struct Node;

    public:
        class ConstListIterator
        {
        public:
            /**
             * @brief Конструктор по умолчанию. Инициализирует поле _node как nullptr.
             */
            ConstListIterator() noexcept { _node = nullptr; }

            /**
             * @brief Конструктор, который принимает указатель на Node и инициализирует _node этим значением.
             *
             * @param node Указатель на узел, который будет использоваться для инициализации _node.
             */
            ConstListIterator(Node *node) noexcept : _node(node) {}

            /**
             * @brief Конструктор копирования, который принимает другой ConstListIterator и копирует его _node.
             *
             * @param other Другой ConstListIterator, который будет использоваться для копирования _node.
             */
            ConstListIterator(const ConstListIterator &other) noexcept : _node(other._node) {}

            /**
             * @brief Конструктор перемещения, который принимает другой ConstListIterator и "перемещает" его _node в текущий объект, затем устанавливает _node другого объекта в nullptr.
             *
             * @param other Другой ConstListIterator, который будет использоваться для перемещения _node.
             */
            ConstListIterator(ConstListIterator &&other) noexcept
            {
                _node = other._node;
                other._node = nullptr;
            }
            ~ConstListIterator() noexcept {}

            const_reference operator*() noexcept;

            bool operator==(const ConstListIterator other) const noexcept;

            bool operator!=(const ConstListIterator &other) const noexcept;
            ConstListIterator operator++() noexcept;
            ConstListIterator operator++(int) noexcept;
            ConstListIterator operator--() noexcept;
            ConstListIterator operator--(int) noexcept;

        public:
            Node *_node; // указатель на текущий узел
            reference error_value = 0;
        } using const_iterator = List<T>::ConstListIterator;

        class ListIterator : public ConstListIterator
        {
            // Используем конструкторы базового класса
            using ConstListIterator::ConstListIterator;
            // Перегружаем оператор разыменования, чтобы он возвращал неконстантную ссылку
            reference operator*() noexcept
            {
                if (this->_node)
                {
                    return this->_node->value_;
                }
            }
            return this->error_value;

        } using iterator = List<T>::ListIterator;

        List() noexcept;
        List(size_type n);
        List(const std::initializer_list<value_type> &items);
        List(const List<T> &other);
        ~List();

        void push_back(const_reference value);
        void push_front(const_reference value);
        void pop_back();
        void pop_front();
        void remove_node(Node *node);
        void clear();
        
        bool empty() const noexcept;
        size_type size() const noexcept;
        size_type max_size() const noexcept;

    private:
        /*
            Структура Node представляет собой узел в двусвязном списке. Она содержит три поля:

            1. T value_: Это поле хранит значение узла. Тип T является шаблонным, что означает, что узел может хранить значения любого типа.

            2. Node* next_: Это указатель на следующий узел в списке. Если этот узел является последним в списке, next_ будет равен nullptr.

            3. Node* prev_: Это указатель на предыдущий узел в списке. Если этот узел является первым в списке, prev_ будет равен nullptr.

            У структуры Node есть два конструктора:

            1. Конструктор по умолчанию создает узел, в котором value_ равно 0, а next_ и prev_ равны nullptr.

            2. Второй конструктор принимает ссылку на константу типа T и создает узел с value_, равным переданному значению, а next_ и prev_ равными nullptr.

            Ключевое слово noexcept указывает, что конструкторы не выбрасывают исключений.

            В двусвязном списке каждый узел связан с предыдущим и следующим узлами, что позволяет эффективно перемещаться в обоих направлениях по списку.
        */
        struct Node
        {
            Node() noexcept : next_(nullptr), prev_(nullptr), value_(0) {}
            Node(const_reference t) noexcept
                : value_(t), next_(nullptr), prev_(nullptr) {}
            value_type value_;
            Node *next_;
            Node *prev_;
        };

        Node *head;
        Node *tail;
        size_t list_size;
    };
}
#include "s21_list.tpp"
#endif // S21_LIST_H
