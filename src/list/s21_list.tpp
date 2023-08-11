#include "s21_list.h"

namespace s21
{
    // Конструкторы и деструктор
    template <typename T>
    List<T>::List() noexcept
        : _head(nullptr), _tail(nullptr), _end(nullptr), _size(0){};

    template <typename T>
    List<T>::List(size_type n) : _head(nullptr), _tail(nullptr), _size(0)
    {
        for (size_type i = 0; i < n; ++i)
        {
            push_back(value_type());
        }
    };

    template <typename T>
    List<T>::List(const std::initializer_list<T> &items)
        : _head(nullptr), _tail(nullptr), _size(0)
    {
        for (const T &item : items)
        {
            push_back(item);
        }
    };

    template <typename T>
    List<T>::List(const List &other)
        : _head(nullptr), _tail(nullptr), _size(0), _end(nullptr)
    {
        copyList(other);
    };
    template <typename T>
    List<T>::List(List &&other) noexcept : _head(nullptr), _tail(nullptr), _end(nullptr), _size(0)
    {
        moveList(other);
    };

    template <typename T>
    List<T>::~List() noexcept
    {
        clear();
    };
    template <typename T>
    void List<T>::operator=(List &&other) noexcept
    {
        moveList(other);
    };

    template <typename T>
    void List<T>::operator=(List &other) noexcept
    {
        copyList(other);
    };

    // Методы изменения списка

    template <typename T>
    void List<T>::push_back(const_reference value)
    {
        if (size() >= max_size())
            throw std::out_of_range(
                "s21_List::PushFront:: Limit of the container is exceeded");
        Node *node = new Node(value);
        node->next_ = _end;
        node->prev_ = _tail;
        if (_tail)
        {
            _tail->next_ = node;
        }
        if (_size == 0)
        {
            _end = nullptr;
            _head = _tail = node;
        }
        else
        {
            _tail = node;
        }
        _size++;
        EndAddress();
    };

    template <typename T>
    void List<T>::push_front(const_reference value)
    {
        if (size() >= max_size())
            throw std::out_of_range(
                "s21_List::PushFront:: Limit of the container is exceeded");
        Node *node = new Node(value);
        node->next_ = _head;
        if (_head)
        {
            _head->prev_ = node;
        }
        if (_size == 0)
        {
            _end = nullptr;
            _head = _tail = node;
        }
        else
        {
            _head = node;
            EndAddress();
        }
        _size++;
    };

    template <class T>
    void List<T>::pop_back() noexcept
    {
        if (_head == _tail)
        {
            _head = nullptr;
        }
        if (_tail)
        {
            Node *newHead = _tail->prev_;
            delete _tail;
            _tail = newHead;
            _end->prev_ = _tail;
            _tail->next_ = _end;
            _size--;
        }
    };

    template <typename T>
    void List<T>::pop_front() noexcept
    {
        if (_head == _tail)
        {
            _tail = nullptr;
        }
        if (_head != _end)
        {
            Node *newHead = _head->next_;
            delete _head;
            _head = newHead;
            _end->next_ = _head;
            _head->prev_ = _end;
            _size--;
        }
        if (_size == 0)
        {
            _head = nullptr;
            _tail = nullptr;
        }
    }

    template <typename T>
    void List<T>::copyList(const List &other) noexcept
    {
        clear();
        for (auto i = other.cBegin(); i != other.cEnd(); i++)
        {
            push_back(*i);
        }
    };

    template <typename T>
    void List<T>::moveList(List &other) noexcept
    {
        clear();
        _head = other._head;
        _tail = other._tail;
        _size = other._size;
        _end = other._end;
        other._end = nullptr;
        other._head = nullptr;
        other._tail = nullptr;
        other._size = 0;
    };

    template <typename T>
    void s21::List<T>::EndAddress() noexcept
    {
        if (_end == nullptr)
        {
            _end = new Node(0);
            _end->next_ = nullptr;
        }
        _head->prev_ = _end;
        _tail->next_ = _end;
        _end->prev_ = _tail;
        _end->value_ = _size;
    };

    template <typename T>
    size_t List<T>::size() const noexcept
    {
        return _size;
    };

    template <typename T>
    bool List<T>::empty() const noexcept
    {
        if (_head)
        {
            return false;
        }
        else
        {
            return true;
        }
    };

    template <typename T>
    typename List<T>::size_type List<T>::max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    };

    //  Вспомогательные методы
    template <typename T>
    void List<T>::clear()
    {
        while (_head != _end && _head)
        {
            pop_front();
        }
        delete _end;
        _head = _tail = _end = nullptr;
        _size = 0;
    };

    // ConstListIterator

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

    template <typename T>
    typename List<T>::ConstListIterator
    List<T>::ConstListIterator::operator++() noexcept
    {
        if (_node)
        {
            _node = _node->next_;
        }
        return _node;
    };

    template <typename T>
    typename List<T>::ConstListIterator
    List<T>::ConstListIterator::operator++(int) noexcept
    {
        List<T>::ConstListIterator tmp(*this);
        if (_node)
        {
            _node = _node->next_;
        }
        return tmp;
    };

    template <typename T>
    typename List<T>::ConstListIterator
    List<T>::ConstListIterator::operator--() noexcept
    {
        if (_node)
        {
            _node = _node->prev_;
        }
        return _node;
    };

    template <typename T>
    typename List<T>::ConstListIterator
    List<T>::ConstListIterator::operator--(int) noexcept
    {
        List<T>::ConstListIterator tmp(*this);
        if (_node)
        {
            _node = _node->prev_;
        }
        return tmp;
    };
    // ListIt
    template <typename T>
    typename List<T>::value_type &
    List<T>::ListIterator::operator*() noexcept
    {
        if (this->_node)
            return this->_node->value_;
        return this->error_value;
    };
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

    template <typename T>
    typename List<T>::iterator List<T>::cBegin() const noexcept
    {
        return iterator(_head);
    };

    template <typename T>
    typename List<T>::iterator List<T>::cEnd() const noexcept
    {
        return iterator(_end);
    };

    template <typename T>
    typename List<T>::iterator List<T>::insert(iterator position, const_reference value)
    {
        Node *currentNode = _head;
        for (auto i = cBegin(); i != position; currentNode = currentNode->next_, ++i)
        {
            if (!currentNode)
            {
                throw std::out_of_range("s21_List::Insert:: list not found!");
            }
        }
        if (!_head)
        {
            push_front(value);
            EndAddress();
            position = _end;
        }
        else if (currentNode && currentNode->prev_ != _end)
        {
            Node *newNode = new Node(value);
            Node *leftNode = currentNode->prev_;
            newNode->prev_ = leftNode;
            newNode->next_ = currentNode;
            leftNode->next_ = newNode;
            currentNode->prev_ = newNode;
            _size++;
        }
        else
        {
            push_front(value);
        }

        --position;
        return position;
    };
    template <typename T>
    void List<T>::swap(List &other) noexcept
    {
        List<T> tempList(*this);
        this->copyList(other);
        other.copyList(tempList);
    };

    template <typename T>
    void List<T>::merge(List &other)
    {
        iterator current = cBegin();
        iterator otherIter = other.cBegin();

        if (empty())
        {
            while (otherIter != other.cEnd())
            {
                push_back(*otherIter);
                ++otherIter;
            }
            other.clear();
            return;
        }

        while (current != cEnd() && otherIter != other.cEnd())
        {
            if (*otherIter <= *current)
            {
                insert(current, *otherIter);
                ++otherIter;
            }
            else
            {
                ++current;
            }
        }

        while (otherIter != other.cEnd())
        {
            push_back(*otherIter);
            ++otherIter;
        }

        other.clear();
    };

    template <typename T>
    void List<T>::reverse() noexcept
    {
        Node *current = _head;
        Node *last = _tail;
        T tmp;
        while (current != last && current != last->next_)
        {
            tmp = current->value_;
            current->value_ = last->value_;
            last->value_ = tmp;
            current = current->next_;
            last = last->prev_;
        }
    }

    template <typename T>
    void List<T>::sort() noexcept
    {
        if (_head == nullptr || _head->next_ == nullptr)
        {
            return;
        }

        bool swapped;
        do
        {
            swapped = false;
            Node *currentNode = _head;
            while (currentNode->next_ != _end)
            {
                Node *nextNode = currentNode->next_;
                if (currentNode->value_ > nextNode->value_)
                {

                    T temp = currentNode->value_;
                    currentNode->value_ = nextNode->value_;
                    nextNode->value_ = temp;
                    swapped = true;
                }
                currentNode = nextNode;
            }
        } while (swapped);
    }

    template <typename T>
    void List<T>::remove_node(Node *node_to_remove)
    {
        if (node_to_remove == nullptr)
        {
            return;
        }

        if (node_to_remove->prev_ != nullptr)
        {
            node_to_remove->prev_->next_ = node_to_remove->next_;
        }

        if (node_to_remove->next_ != nullptr)
        {
            node_to_remove->next_->prev_ = node_to_remove->prev_;
        }

        delete node_to_remove;
        _size;
    };

    template <typename T>
    typename List<T>::const_reference List<T>::front() noexcept
    {
        if (_head)
            return _head->value_;
        return error_value;
    };
    template <typename T>
    typename List<T>::const_reference List<T>::back() noexcept
    {
        if (_tail)
            return _tail->value_;
        return error_value;
    };

}