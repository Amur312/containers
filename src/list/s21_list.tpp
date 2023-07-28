#include "s21_list.h"

namespace s21
{
    // Конструкторы и деструктор
    template <typename T>
    List<T>::List() noexcept
        : head(nullptr), tail(nullptr), list_size(0) {}

    template <typename T>
    List<T>::List(size_type n) : head(nullptr), tail(nullptr), list_size(0)
    {
        for (size_type i = 0; i < n; ++i)
        {
            push_back(value_type());
        }
    };

    template <typename T>
    List<T>::List(const std::initializer_list<T> &items)
        : head(nullptr), tail(nullptr), list_size(0)
    {
        for (const T &item : items)
        {
            push_back(item);
        }
    };

    template <typename T>
    List<T>::List(const List<T> &other)
        : head(nullptr), tail(nullptr), list_size(0)
    {
        for (Node *node = other.head; node != nullptr; node = node->next)
        {
            push_back(node->value);
        }
    };

    template <typename T>
    List<T>::~List()
    {
        clear();
    };

    // Методы изменения списка

    /**
     * @brief Добавляет элемент в конец списка.
     *
     * Создает новый узел, устанавливает его значение равным переданному значению и добавляет его в конец списка.
     * Если список пуст, новый узел становится головой и хвостом списка.
     * В противном случае новый узел становится новым хвостом списка, а старый хвост указывает на новый узел.
     * Увеличивает размер списка на 1.
     *
     * @param value Значение для добавления в список.
     */

    template <typename T>
    void List<T>::push_back(const_reference value)
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
    };

    /**
     * @brief Добавляет элемент в начало списка.
     *
     * Создает новый узел, устанавливает его значение равным переданному значению и добавляет его в начало списка.
     * Если список пуст, новый узел становится головой и хвостом списка.
     * В противном случае новый узел становится новой головой списка, а старая голова указывает на новый узел.
     * Увеличивает размер списка на 1.
     *
     * @param value Значение для добавления в список.
     */

    template <typename T>
    void List<T>::push_front(const_reference value)
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
    };

    /**
     * @brief Удаляет элемент из конца списка.
     *
     * Если список пуст, выбрасывает исключение.
     * В противном случае удаляет хвост списка.
     * Если голова и хвост указывают на один и тот же узел, обнуляет голову и хвост.
     * Уменьшает размер списка на 1.
     */

    template <typename T>
    void List<T>::pop_back()
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
    };

    /**
     * @brief Удаляет элемент из начала списка.
     *
     * Если список пуст, выбрасывает исключение.
     * В противном случае удаляет голову списка.
     * Если голова и хвост указывают на один и тот же узел, обнуляет голову и хвост.
     * Уменьшает размер списка на 1.
     */

    template <typename T>
    void List<T>::pop_front()
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
    };

    /**
     * @brief Возвращает размер списка.
     *
     * @return Размер списка.
     */

    template <typename T>
    size_t List<T>::size() const noexcept
    {
        return list_size;
    };

    template <typename T>
    bool List<T>::empty() const noexcept
    {
        return list_size == 0;
    };

    /**
     * @brief Возвращает максимальное количество элементов типа T, которые могут быть размещены в памяти.
     *
     * Метод возвращает максимальное количество элементов типа T, которые могут быть размещены в памяти.
     * Это значение вычисляется путем деления максимально возможного значения для типа size_type на размер типа T.
     * Это может быть полезно, если мы работаем с большими объектами и хотим узнать, сколько таких объектов мы можем разместить в памяти.
     *
     * @return Максимальное количество элементов типа T, которые могут быть размещены в памяти.
     */
    template <typename T>
    typename List<T>::size_type List<T>::max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    };

    //  Вспомогательные методы
    template <typename T>
    void List<T>::clear()
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
    };

    // ConstListIterator

    /**
     * @brief Перегруженный оператор разыменования для получения значения узла, на который указывает итератор.
     *
     * Если итератор указывает на узел (_node не равен nullptr), метод возвращает значение этого узла (_node->value_).
     * Если итератор не указывает на узел (_node равен nullptr), метод возвращает специальное значение error_value.
     * Это может быть полезно для обработки ошибок, когда итератор не указывает на действительный узел в списке.
     *
     * @return Значение узла, на который указывает итератор, или error_value, если итератор не указывает на действительный узел.
     */
    template <class T>
    typename List<T>::const_reference
    List<T>::ConstListIterator::operator*() noexcept
    {
        if (_node)
        {
            return _node->value_;
        }
        return error_value;
    };

    /**
     * @brief Перегруженный оператор инкремента для перемещения итератора вперед по списку.
     *
     * Если итератор указывает на узел, метод изменяет указатель _node так, чтобы он указывал на следующий узел в списку.
     * Если итератор не указывает на узел (т.е. _node равен nullptr), метод не делает ничего.
     * Возвращает итератор, указывающий на новый узел (или на nullptr, если итератор достиг конца списка или изначально был равен nullptr).
     *
     * @return Итератор, указывающий на следующий узел в списке.
     */
    template <typename T>
    typename s21::List<T>::ConstListIterator
    List<T>::ConstListIterator::operator++() noexcept
    {
        if (_node)
        {
            _node = _node->next_;
        }
        return _node;
    };

    /**
     * @brief Перегруженный оператор инкремента (postfix) для перемещения итератора к следующему узлу в списке.
     *
     * Этот метод сначала создает временный объект итератора, инициализированный текущим состоянием итератора.
     * Затем, если итератор указывает на узел (_node не равен nullptr), метод перемещает итератор к следующему узлу в списке (_node = _node->next_).
     * Независимо от того, указывает ли итератор на узел, метод возвращает временный объект итератора.
     * Это означает, что значение итератора до инкремента сохраняется и возвращается, а сам итератор перемещается к следующему узлу.
     *
     * @return Копия итератора до его инкремента.
     */
    template <typename T>
    typename s21::List<T>::ConstListIterator
    s21::List<T>::ConstListIterator::operator++(int) noexcept
    {
        s21::List<T>::ConstListIterator tmp(*this);
        if (_node)
        {
            _node = _node->next_;
        }
        return tmp;
    }

    /**
     * @brief Перегруженный оператор декремента (prefix) для перемещения итератора к предыдущему узлу в списке.
     *
     * Если итератор указывает на узел (_node не равен nullptr), метод перемещает итератор к предыдущему узлу в списке (_node = _node->prev_).
     * Затем метод возвращает обновленный итератор.
     *
     * @return Итератор, указывающий на предыдущий узел в списке.
     */
    template <typename T>
    typename s21::List<T>::ConstListIterator
    s21::List<T>::ConstListIterator::operator--() noexcept
    {
        if (_node)
        {
            _node = _node->prev_;
        }
        return _node;
    };

    /**
     * @brief Перегруженный оператор декремента (postfix) для перемещения итератора к предыдущему узлу в списке.
     *
     * Этот метод сначала создает временный объект итератора, инициализированный текущим состоянием итератора.
     * Затем, если итератор указывает на узел (_node не равен nullptr), метод перемещает итератор к предыдущему узлу в списке (_node = _node->prev_).
     * Независимо от того, указывает ли итератор на узел, метод возвращает временный объект итератора.
     * Это означает, что значение итератора до декремента сохраняется и возвращается, а сам итератор перемещается к предыдущему узлу.
     *
     * @return Копия итератора до его декремента.
     */
    template <typename T>
    typename s21::List<T>::ConstListIterator
    s21::List<T>::ConstListIterator::operator--(int) noexcept
    {
        s21::List<T>::ConstListIterator tmp(*this);
        if (_node)
        {
            _node = _node->prev_;
        }
        return tmp;
    };

    /**
     * @brief Перегруженный оператор сравнения для проверки, указывают ли два итератора на один и тот же узел.
     *
     * Метод сначала проверяет, являются ли два итератора (текущий объект и переданный в качестве аргумента) одним и тем же объектом.
     * Это делается путем сравнения адресов этих объектов (this == &other).
     * Если это так, метод возвращает true, поскольку один и тот же объект, очевидно, равен самому себе.
     *
     * Если итераторы не являются одним и тем же объектом, метод сравнивает узлы, на которые указывают эти итераторы (_node == other._node).
     * Если итераторы указывают на один и тот же узел, метод возвращает true, в противном случае - false.
     *
     * @return true, если два итератора указывают на один и тот же узел, иначе false.
     */
    template <typename T>
    bool List<T>::ConstListIterator::operator==(
        const ConstListIterator other) const noexcept
    {
        if (this == &other)
        {
            return true;
        }
        return _node == other._node;
    };

    /**
     * @brief Перегруженный оператор неравенства для сравнения двух итераторов.
     *
     * Метод сначала вызывает оператор равенства (==) для двух итераторов и затем возвращает противоположное значение.
     * Таким образом, если два итератора указывают на один и тот же узел, метод возвращает false, иначе - true.
     *
     * @param other Итератор для сравнения с текущим итератором.
     * @return true, если итераторы указывают на разные узлы, и false, если они указывают на один и тот же узел.
     */
    template <typename T>
    bool s21::List<T>::ConstListIterator::operator!=(
        const ConstListIterator &other) const noexcept
    {
        return !(*this == other);
    };
}