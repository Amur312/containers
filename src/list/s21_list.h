#ifndef S21_LIST_H
#define S21_LIST_H

#include <iostream>
#include <limits>

namespace s21
{

    template <typename T>
    class list
    {
    private:
        struct Node
        {
            T data;
            Node *next;
            Node *prev;
        };

        Node *head;
        Node *tail;
        size_t list_size;

    public:
        list() : head(nullptr), tail(nullptr), list_size(0) {}

        ~list()
        {
            Node *current = head;
            while (current != nullptr)
            {
                Node *nodeToDelete = current;
                current = current->next;
                delete nodeToDelete;
            }
        }
        /**
         * Конструктор копирования
         *
         * Этот конструктор принимает const ссылку на другой список 'other', который мы хотим скопировать.
         * Конструктор создает новый список, который является копией списка 'other'.
         *
         * В цикле for мы проходим по всем узлам списка 'other' и добавляем их в новый список с помощью
         * метода push_back(). Это создает новые узлы в новом списке, которые имеют те же значения, что и узлы 'other'.
         *
         * В результате у нас получается новый список, который содержит все элементы списка 'other' в том же порядке.
         *
         * @param other Список, который нужно скопировать.
         */
        list(const list &other) : list()
        {
            for (Node *node = other.head; node != nullptr; node = node->next)
            {
                push_back(node->data);
            }
        }

        /**
         * Конструктор перемещения
         *
         * Этот конструктор принимает rvalue-ссылку на другой список 'other'.
         *
         * После того как узлы были перемещены из 'other' в этот список, мы устанавливаем
         * 'head', 'tail' и 'list_size' в 'other' в начальное состояние . Это оставляет 'other' в валидном, но пустом состоянии.
         * Это важно, потому что 'other' все равно будет уничтожен, когда выйдет из области видимости,
         * и мы не хотим, чтобы его деструктор попытался удалить узлы, которые теперь принадлежат
         * этому списку. https://metanit.com/cpp/tutorial/14.2.php
         *
         * Спецификатор 'noexcept' гарантирует, что этот конструктор не будет генерировать исключения.
         * Это важно для оптимальной работы некоторых функций стандартной библиотеки,
         * таких как std::move_if_noexcept. https://metanit.com/cpp/tutorial/14.4.php
         *
         * @param other Список, из которого будут перемещены узлы.
         */
        list(list &&other) noexcept : head(other.head), tail(other.tail), list_size(other.size)
        {
            other.head = nullptr;
            other.tail = nullptr;
            other.list_size = 0;
        }

        /**
         * Конструктор со списком инициализации
         *
         * Этот конструктор принимает список инициализации, который содержит элементы, которые
         * должны быть добавлены в новый список при его создании.
         *
         * Внутри конструктора мы проходим по каждому элементу в списке инициализации и
         * добавляем его в конец нашего нового списка с помощью метода push_back().
         *
         * @param init_list Список инициализации с элементами для добавления в список.
         */
        list(std::initializer_list<T> init_list) : list()
        {
            for (const T &value : init_list)
            {
                push_back(value);
            }
        }
        /**
         * Возвращает ссылку на первый элемент в списке
         *
         * Этот метод возвращает ссылку на данные в первом узле списка. Если список пуст,
         * выбрасывается исключение с сообщением, что невозможно получить первый элемент пустого списка.
         *
         * @return Ссылка на первый элемент в списке.
         * @throws std::runtime_error Если список пуст.
         */
        const T &front() const
        {
            if (head == nullptr)
            {
                throw std::runtime_error("Невозможно получить первый элемент пустого списка");
            }
            return head->data;
        }
        /**
         * Возвращает ссылку на последний элемент в списке
         *
         * Этот метод возвращает ссылку на данные в последнем узле списка. Если список пуст,
         * выбрасывается исключение с сообщением, что невозможно получить последний элемент пустого списка.
         *
         * @return Ссылка на последний элемент в списке.
         * @throws std::runtime_error Если список пуст.
         */
        const T &back() const
        {
            if (tail == nullptr)
            {
                throw std::runtime_error("Невозможно получить последний элемент пустого списка");
            }
            return tail->data;
        }
        /**
         * Возвращает количество элементов в списке.
         *
         * Этот метод возвращает текущее количество элементов в списке, которое хранится в переменной 'list_size'.
         *
         * @return Количество элементов в списке.
         */
        size_t size() const
        {
            return list_size;
        }

        /**
         * Проверяет, пуст ли список.
         *
         * Этот метод возвращает true, если в списке нет элементов (то есть 'list_size' равен 0), и false в противном случае.
         *
         * @return true, если список пуст, и false, если в списке есть элементы.
         */
        bool empty() const
        {
            return list_size == 0;
        }

        /**
         * Добавляет элемент в конец списка.
         *
         * Этот метод создает новый узел с данными 'value', устанавливает его как следующий узел для текущего последнего узла,
         * и затем обновляет 'tail' чтобы указывать на новый узел. Если список был пуст, 'head' также обновляется, чтобы указывать на новый узел.
         *
         * @param value Данные для добавления в список.
         */
        void push_back(const T &value)
        {
            Node *newNode = new Node{value, nullptr, tail};
            if (tail)
            {
                tail->next = newNode;
            }
            else
            {
                head = newNode;
            }
            tail = newNode;
            list_size++;
        }

        /**
         * Добавляет элемент в начало списка.
         *
         * Этот метод создает новый узел с данными 'value', устанавливает его как предыдущий узел для текущего первого узла,
         * и затем обновляет 'head' чтобы указывать на новый узел. Если список был пуст, 'tail' также обновляется, чтобы указывать на новый узел.
         *
         * @param value Данные для добавления в список.
         */
        void push_front(const T &value)
        {
            Node *newNode = new Node{value, head, nullptr};
            if (head)
            {
                head->prev = newNode;
            }
            else
            {
                tail = newNode;
            }
            head = newNode;
            list_size++;
        }
        /**
         * Удаляет элемент с конца списка.
         *
         * Если список пуст (то есть 'tail' указывает на nullptr), этот метод выбрасывает исключение.
         * В противном случае этот метод удаляет последний узел в списке, обновляет 'tail' чтобы указывать на предыдущий узел
         * и уменьшает 'list_size' на 1. Если после удаления узла список становится пустым, 'head' также обновляется, чтобы указывать на nullptr.
         *
         * @throws std::range_error Если список пуст.
         */
        void pop_back()
        {
            if (tail == nullptr)
            {
                throw std::range_error("Список пуст");
            }
            Node *nodeToDelit = tail;
            tail = tail->prev;
            if (tail != nullptr)
                [tail->next = nullptr;
                ] else
                {
                    head = nullptr;
                }
            delete nodeToDelit;
            list_size--;
        }

        /**
         * Удаляет элемент с начала списка.
         *
         * Если список пуст (то есть 'head' указывает на nullptr), этот метод выбрасывает исключение.
         * В противном случае этот метод удаляет первый узел в списке, обновляет 'head' чтобы указывать на следующий узел
         * и уменьшает 'list_size' на 1. Если после удаления узла список становится пустым, 'tail' также обновляется, чтобы указывать на nullptr.
         *
         * @throws std::range_error Если список пуст.
         */
        void pop_front()
        {
            if (head == nullptr)
            {
                throw std::range_error("Список пуст");
            }
            Node *nodeToDelete = head;
            head = head->next;
            if (head != nullptr)
            {
                head->prev = nullptr;
            }
            else
            {
                tail = nullptr;
            }
            delete nodeToDelete;
            list_size--;
        }
    };

} // namespace s21

#endif // S21_LIST_H
