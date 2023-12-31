#include "s21_list.h"

namespace s21 {
/**
 * @brief Создает пустой список типа List.
 *
 * Этот конструктор инициализирует пустой экземпляр списка типа List.
 * Указатели на голову, хвост и конец списка устанавливаются в nullptr,
 * размер списка устанавливается на 0.
 *
 * @tparam T Тип элементов, хранимых в списке.
 */
template <typename T>
List<T>::List() noexcept
    : _head(nullptr), _tail(nullptr), _end(nullptr), _size(0) {}

/**
 * @brief Создает контейнер типа List с заданным количеством элементов.
 *
 * Этот конструктор создает экземпляр контейнера типа List, заполняя его
 * заданным количеством элементов. Внутренние указатели на начальный и конечный
 * элементы списка инициализируются. Значение размера контейнера также
 * обновляется.
 *
 * @tparam T Тип элементов, хранимых в контейнере.
 * @param n Количество элементов для создания в списке.
 */
template <typename T>
List<T>::List(size_type n) : _head(nullptr), _tail(nullptr), _size(0) {
  // Если количество элементов равно нулю, завершаем конструктор
  if (n == 0)
    return;

  // Создаем начальный узел и связываем остальные узлы
  _head = new Node(value_type());
  Node *current = _head;
  for (size_type i = 1; i < n; ++i) {
    current->next_ = new Node(value_type());
    current->next_->prev_ = current;
    current = current->next_;
  }
  _tail = current;
  _size = n;
};

/**
 * @brief Создает связанный список и заполняет его элементами из списка
 * инициализации.
 *
 * Этот конструктор создает экземпляр связанного списка типа List,
 * используя элементы из переданного списка инициализации. Внутренние поля,
 * представляющие начало и конец списка, также инициализируются.
 *
 * @tparam T Тип элементов, хранимых в списке.
 * @param items Список инициализации, содержащий элементы для заполнения списка.
 */
template <typename T>
List<T>::List(const std::initializer_list<T> &items)
    : _head(nullptr), _tail(nullptr), _size(0) {
  // Проходим по всем элементам списка инициализации
  for (const T &item : items) {
    // Добавляем элемент в конец списка
    push_back(item);
  }
}

/**
 * @brief Конструктор копирования для контейнера типа List.
 *
 * Этот конструктор создает копию контейнера типа List на основе
 * переданного другого экземпляра List. Инициализируются все необходимые
 * члены данных, включая указатели на голову, хвост и конец списка, а также
 * размер контейнера.
 *
 * @tparam T Тип элементов, хранимых в контейнере.
 * @param other Ссылка на другой экземпляр List, из которого будет создана
 * копия.
 */
template <typename T>
List<T>::List(const List &other)
    : _head(nullptr), _tail(nullptr), _size(0), _end(nullptr) {
  // Вызов вспомогательной функции для копирования элементов
  copyList(other);
};

/**
 * @brief Move-конструктор для контейнера типа List.
 *
 * Этот конструктор осуществляет перемещение элементов из другого контейнера
 * типа List в текущий контейнер. После перемещения, другой контейнер будет
 * валиден, но состояние и его элементы могут измениться. Перемещение
 * осуществляется за константное время, поскольку контейнеры делят свои
 * внутренние структуры данных.
 *
 * @tparam T Тип элементов в контейнере.
 * @param other Другой контейнер типа List, из которого происходит перемещение.
 */
template <typename T>
List<T>::List(List &&other) noexcept
    : _head(nullptr), _tail(nullptr), _end(nullptr), _size(0) {
  // Вызов функции перемещения элементов из другого контейнера
  moveList(other);
};

/**
 * @brief Деструктор класса List.
 *
 * Деструктор освобождает ресурсы, занимаемые объектом класса List.
 * Он вызывает метод clear() для очистки всех элементов списка.
 *
 * @tparam T Тип данных, хранящихся в списке.
 */
template <typename T> List<T>::~List() noexcept { clear(); };

/**
 * @brief Оператор перемещения для класса List.
 *
 * Этот оператор позволяет перемещать содержимое одного списка в другой
 * списке. Он использует метод moveList() для выполнения перемещения.
 *
 * @tparam T Тип данных, хранящихся в списке.
 * @param other Список, содержимое которого нужно переместить.
 */
template <typename T> void List<T>::operator=(List &&other) noexcept {
  moveList(other);
};

/**
 * @brief Присваивание содержимого одного списка другому списку.
 *
 * Этот метод выполняет операцию присваивания содержимого одного списка (other)
 * другому списку. Содержимое other копируется в текущий список, используя
 * метод copyList. Операция присваивания помечена как noexcept, что означает,
 * что она не будет генерировать исключения.
 *
 * @tparam T Тип элементов, хранящихся в списке.
 * @param other Список, содержимое которого будет присвоено текущему списку.
 */
template <typename T> void List<T>::operator=(const List &other) noexcept {
  // Копируем содержимое списка other в текущий список
  copyList(std::move(other));
}

/**
 * @brief Добавляет элемент в конец списка.
 *
 * Этот метод позволяет добавить новый элемент в конец списка. Если размер
 * списка достиг максимального значения, вызывается исключение
 * std::out_of_range. Создается новый узел и добавляется в конец списка. Связи
 * между узлами обновляются, чтобы новый узел стал последним элементом.
 *
 * @tparam T Тип элементов, хранящихся в списке.
 * @param value Значение элемента, которое нужно добавить.
 * @throws std::out_of_range Если размер списка превышает максимальное значение.
 */
template <typename T> void List<T>::push_back(const_reference value) {
  // Проверка на достижение максимального размера списка
  if (size() >= max_size())
    throw std::out_of_range(
        "s21_List::push_back: Limit of the container is exceeded");

  // Создание нового узла
  Node *node = new Node(value);

  // Обновление связей узлов
  node->next_ = _end;
  node->prev_ = _tail;

  // Обновление связи последнего узла
  if (_tail) {
    _tail->next_ = node;
  }

  // Если список был пуст, обновление начального и конечного указателей
  if (_size == 0) {
    _end = nullptr;
    _head = _tail = node;
  } else {
    _tail = node;
  }

  // Увеличение размера списка
  _size++;

  // Обновление указателя на конец списка
  endAddress();
}

/**
 * @brief Вставляет элемент в начало списка.
 *
 * Этот метод позволяет вставить элемент в начало связанного списка. При этом
 * производится проверка на достижение предельного размера контейнера, и в
 * случае его превышения, будет выброшено исключение std::out_of_range.
 *
 * @tparam T Тип элементов хранящихся в списке.
 * @param value Значение элемента, который будет вставлен в начало списка.
 * @throws std::out_of_range В случае, если предельный размер контейнера
 * превышен.
 */
template <typename T> void List<T>::push_front(const_reference value) {
  // Проверка на превышение предельного размера контейнера
  if (size() >= max_size())
    throw std::out_of_range(
        "s21_List::push_front:: Limit of the container is exceeded");

  // Создание нового узла с переданным значением
  Node *node = new Node(value);
  node->next_ = _head;

  // Обновление ссылки предыдущего элемента, если голова списка существует
  if (_head) {
    _head->prev_ = node;
  }

  // Обработка случая, когда список пуст
  if (_size == 0) {
    _end = nullptr;
    _head = _tail = node;
  } else {
    _head = node;
    endAddress();
  }

  // Увеличение размера списка
  _size++;
}

/**
 * @brief Удаляет последний элемент из списка.
 *
 * Этот метод удаляет последний элемент из списка. Если список оказывается
 * пустым после удаления, обновляются соответствующие указатели, чтобы список
 * стал пустым. В противном случае, последний элемент удаляется и обновляются
 * соответствующие указатели для корректного связывания оставшихся элементов.
 * Размер списка уменьшается на 1.
 *
 * @tparam T Тип элементов, хранящихся в списке.
 */
template <class T> void List<T>::pop_back() noexcept {
  if (_head == _tail) {
    // Если в списке только один элемент
    _head = nullptr;
  }
  if (_tail) {
    Node *newHead = _tail->prev_;
    delete _tail;
    _tail = newHead;
    _end->prev_ = _tail;
    _tail->next_ = _end;
    _size--;
  }
};

/**
 * @brief Удаляет первый элемент из списка.
 *
 * Этот метод удаляет первый элемент из списка. Если список пустой, ничего не
 * делается. Если в списке только один элемент, он удаляется, и обновляются
 * указатели на голову и хвост, а также размер списка устанавливается в 0.
 * При наличии более одного элемента, первый элемент удаляется, и указатели
 * обновляются для корректного связывания оставшихся элементов. Размер списка
 * уменьшается на 1.
 *
 * @tparam T Тип элементов, хранящихся в списке.
 */
template <typename T> void List<T>::pop_front() noexcept {
  if (!_head) {
    // Если список пустой, ничего не делаем.
    return;
  }

  if (_head == _tail) {
    // Если в списке только один элемент
    delete _head;
    _head = nullptr;
    _tail = nullptr;
    _size = 0;
    return;
  }

  // Если в списке больше одного элемента
  Node *newHead = _head->next_;
  delete _head;
  _head = newHead;
  _head->prev_ = _end; // Устанавливаем новый head
  _end->next_ = _head; // Обновляем указатель у end
  _size--;
}

/**
 * @brief Копирует элементы из другого списка в текущий список.
 *
 * Этот метод выполняет копирование элементов из переданного списка `other`
 * в текущий список. Перед копированием существующие элементы в текущем списке
 * удаляются с помощью вызова метода clear(). Затем каждый элемент из списка
 * `other` добавляется в конец текущего списка с использованием метода
 * push_back().
 *
 * @tparam T Тип элементов, хранящихся в списке.
 * @param other Ссылка на другой список, из которого будут скопированы элементы.
 */
template <typename T> void List<T>::copyList(const List &other) noexcept {
  // Удаляем существующие элементы в текущем списке
  clear();

  // Проходим по всем элементам списка other и копируем их
  for (auto i = other.cBegin(); i != other.cEnd(); i++) {
    // Добавляем элемент в конец текущего списка
    push_back(*i);
  }
}

/**
 * @brief Переносит элементы из другого списка в текущий список.
 *
 * Этот метод выполняет перенос элементов из переданного списка `other`
 * в текущий список. Перед переносом существующие элементы в текущем списке
 * удаляются с помощью вызова метода clear(). Затем происходит перенос
 * внутренних указателей и размера из списка `other` в текущий список.
 * Указатели в списке `other` обнуляются, и его размер обнуляется.
 *
 * @tparam T Тип элементов, хранящихся в списке.
 * @param other Ссылка на другой список, из которого будут перенесены элементы.
 */
template <typename T> void List<T>::moveList(List &other) noexcept {
  // Удаляем существующие элементы в текущем списке
  clear();

  // Переносим внутренние данные из списка other в текущий список
  _head = other._head;
  _tail = other._tail;
  _size = other._size;
  _end = other._end;

  // Обнуляем внутренние данные в списке other
  other._end = nullptr;
  other._head = nullptr;
  other._tail = nullptr;
  other._size = 0;
}

/**
 * @brief Задает адрес конца списка для двусвязной структуры.
 *
 * Этот метод задает адрес конца списка для двусвязной структуры. Если адрес
 * конца списка уже задан (не равен nullptr), то ничего не происходит.
 * В противном случае, создается новый узел и он связывается с концом списка.
 * Указатель prev_ последнего элемента списка указывает на конечный узел,
 * и значение в конечном узле устанавливается равным размеру списка.
 *
 * @tparam T Тип элементов, хранящихся в списке.
 */
template <typename T> void s21::List<T>::endAddress() noexcept {
  // Проверяем, не задан ли уже адрес конца списка
  if (_end == nullptr) {
    // Создаем новый узел для конечного адреса
    _end = new Node(0);
    _end->next_ = nullptr;
  }

  // Связываем элементы и конечный узел
  _head->prev_ = _end;
  _tail->next_ = _end;
  _end->prev_ = _tail;

  // Устанавливаем значение конечного узла равным размеру списка
  _end->value_ = _size;
}

/**
 * @brief Возвращает размер списка.
 *
 * Этот метод возвращает текущий размер списка, который представляет собой
 * количество элементов в списке.
 *
 * @tparam T Тип элементов в списке.
 * @return Размер списка.
 */
template <typename T> size_t List<T>::size() const noexcept { return _size; }

/**
 * @brief Проверяет, является ли список пустым.
 *
 * Этот метод проверяет, пуст ли список, путем проверки наличия указателя на
 * голову списка. Если указатель на голову существует, список считается
 * непустым.
 *
 * @tparam T Тип элементов в списке.
 * @return true, если список пуст, иначе false.
 */
template <typename T> bool List<T>::empty() const noexcept {
  if (_head) {
    return false;
  } else {
    return true;
  }
}

/**
 * @brief Возвращает максимально возможный размер списка.
 *
 * Этот метод возвращает максимально возможный размер списка, который
 * определяется на основе размера типа T и ограничений памяти.
 *
 * @tparam T Тип элементов в списке.
 * @return Максимальный размер списка.
 */
template <typename T>
typename List<T>::size_type List<T>::max_size() const noexcept {
  return std::numeric_limits<size_type>::max() / sizeof(T);
}

/**
 * @brief Очищает список, удаляя все элементы.
 *
 * Этот метод освобождает память, выделенную для элементов списка, и сбрасывает
 * головные и хвостовые указатели, а также счетчик размера списка.
 *
 * @tparam T Тип элементов в списке.
 */
template <typename T> void List<T>::clear() {
  while (_head != _end && _head) {
    pop_front();
  }
  delete _end;
  _head = _tail = _end = nullptr;
  _size = 0;
}

/**
 * @brief Возвращает ссылку на значение, на которое указывает итератор.
 *
 * Этот метод возвращает ссылку на значение, на которое указывает текущий
 * итератор типа ConstListIterator. Если указатель на узел (_node) не равен
 * нулю, метод возвращает значение, хранящееся в этом узле. В противном случае,
 * возвращается значение error_value (значение по умолчанию для ошибок).
 *
 * @tparam T Тип элементов в списке.
 * @return Ссылка на значение, на которое указывает итератор, или значение по
 * умолчанию для ошибок.
 */
template <class T>
typename List<T>::const_reference
List<T>::ConstListIterator::operator*() noexcept {
  if (_node) {
    return _node->value_;
  }
  return error_value;
};

/**
 * @brief Перемещает итератор на следующий элемент в списке.
 *
 * Этот метод инкрементирует указатель на узел (_node) и перемещает итератор
 * на следующий элемент в списке. Возвращается обновленный итератор типа
 * ConstListIterator. Если указатель на узел (_node) не равен нулю, итератор
 * перемещается на следующий элемент. В противном случае, остается на месте, что
 * соответствует попытке перемещения за конец списка.
 *
 * @tparam T Тип элементов в списке.
 * @return Обновленный итератор, указывающий на следующий элемент в списке
 * или оставшийся на месте при попытке перемещения за конец списка.
 */
template <typename T>
typename List<T>::ConstListIterator
List<T>::ConstListIterator::operator++() noexcept {
  if (_node) {
    _node = _node->next_;
  }
  return *this;
};

/**
 * @brief Постфиксный оператор инкремента для константного итератора списка.
 *
 * Этот метод перегружает оператор постфиксного инкремента для константного
 * итератора списка. Он возвращает текущий итератор (копию) и затем
 * перемещает итератор на следующий элемент списка, если таковой существует.
 *
 * @param int Параметр-заглушка, обозначающий постфиксный инкремент.
 * @return Копия текущего итератора до инкремента.
 */
template <typename T>
typename List<T>::ConstListIterator
List<T>::ConstListIterator::operator++(int) noexcept {
  List<T>::ConstListIterator tmp(*this); // Создание копии текущего итератора
  if (_node) {
    _node = _node->next_; // Перемещение итератора на следующий элемент
  }
  return tmp; // Возвращение копии итератора до инкремента
};

/**
 * @brief Префиксный оператор декремента для константного итератора списка.
 *
 * Этот метод перегружает оператор префиксного декремента для константного
 * итератора списка. Он перемещает итератор на предыдущий элемент списка,
 * если текущий итератор не является нулевым (указывает на начало списка).
 *
 * @return Итератор на предыдущий элемент списка (если возможно).
 */
template <typename T>
typename List<T>::ConstListIterator
List<T>::ConstListIterator::operator--() noexcept {
  if (_node) {
    _node = _node->prev_; // Перемещение итератора на предыдущий элемент
  }
  return _node; // Возвращение итератора после декремента
};

/**
 * @brief Постфиксный оператор декремента для константного итератора списка.
 *
 * Этот метод перегружает оператор постфиксного декремента для константного
 * итератора списка. Он возвращает текущий итератор, а затем перемещает итератор
 * на предыдущий элемент списка, если возможно.
 *
 * @tparam T Тип элементов списка.
 * @return Копия текущего итератора перед декрементом.
 */
template <typename T>
typename List<T>::ConstListIterator
List<T>::ConstListIterator::operator--(int) noexcept {
  List<T>::ConstListIterator tmp(*this);
  if (_node) {
    _node = _node->prev_;
  }
  return tmp;
};

/**
 * @brief Оператор разыменования для итератора списка.
 *
 * Этот метод перегружает оператор разыменования для обычного итератора списка.
 * Он возвращает ссылку на значение элемента, на который указывает итератор,
 * или на "error_value" в случае отсутствия элемента.
 *
 * @tparam T Тип элементов списка.
 * @return Ссылка на значение элемента списка или на "error_value".
 */
template <typename T>
typename List<T>::value_type &List<T>::ListIterator::operator*() noexcept {
  if (this->_node)
    return this->_node->value_;
  return this->error_value;
};
/**
 * @brief Перегружает оператор сравнения на равенство для константных
 * итераторов.
 *
 * Этот метод перегружает оператор сравнения на равенство для константных
 * итераторов. Он сравнивает текущий итератор с другим константным итератором и
 * возвращает true, если они указывают на один и тот же элемент, либо если оба
 * итератора указывают на конец списка (сравнивая себя с другим). Метод работает
 * в режиме noexcept.
 *
 * @param other Другой константный итератор, с которым сравнивается текущий.
 * @return True, если итераторы равны, иначе false.
 */
template <typename T>
bool List<T>::ConstListIterator::operator==(
    const ConstListIterator other) const noexcept {
  if (this == &other) {
    return true;
  }
  return _node == other._node;
};

/**
 * @brief Возвращает константный итератор, указывающий на начало списка.
 *
 * Этот метод возвращает константный итератор, указывающий на начало списка.
 * Итератор создается с использованием конструктора класса iterator, который
 * принимает указатель на начальный элемент списка. Метод работает в режиме
 * noexcept.
 *
 * @return Константный итератор, указывающий на начало списка.
 */
template <typename T>
typename List<T>::iterator List<T>::cBegin() const noexcept {
  return iterator(_head);
};

/**
 * @brief Возвращает константный итератор, указывающий на конец списка.
 *
 * Этот метод возвращает константный итератор, указывающий на конец списка.
 * Итератор создается с использованием конструктора класса iterator, который
 * принимает указатель на элемент, следующий за последним элементом списка.
 * Метод работает в режиме noexcept.
 *
 * @return Константный итератор, указывающий на конец списка.
 */
template <typename T>
typename List<T>::iterator List<T>::cEnd() const noexcept {
  return iterator(_end);
};
/**
 * @brief Вставляет элемент в список на указанную позицию.
 *
 * Этот метод вставляет элемент на определенную позицию в списке. Позиция
 * задается итератором, который указывает на элемент, перед которым нужно
 * вставить новый элемент. Метод проверяет существование указанной позиции,
 * проходя по списку с помощью итераторов. Если позиция не найдена,
 * выбрасывается исключение std::out_of_range.
 *
 * Если список пуст, новый элемент добавляется в начало списка, и позиция
 * обновляется на новый элемент. Если указанная позиция не является началом
 * списка, создается новый узел и вставляется на указанную позицию, обновляя
 * указатели соседних узлов.
 *
 * @tparam T Тип элементов списка.
 * @param position Итератор, указывающий на позицию, перед которой вставляется
 * элемент.
 * @param value Значение элемента, который нужно вставить.
 * @return Итератор на вставленный элемент.
 * @throws std::out_of_range Если позиция не найдена в списке.
 */
template <typename T>
typename List<T>::iterator List<T>::insert(iterator position,
                                           const_reference value) {
  Node *currentNode = _head;
  for (auto i = cBegin(); i != position;
       currentNode = currentNode->next_, ++i) {
    if (!currentNode) {
      throw std::out_of_range("s21_List::Insert:: список не найден!");
    }
  }
  if (!_head) {
    push_front(value);
    endAddress();
    position = _end;
  } else if (currentNode && currentNode->prev_ != _end) {
    Node *newNode = new Node(value);
    Node *leftNode = currentNode->prev_;
    newNode->prev_ = leftNode;
    newNode->next_ = currentNode;
    leftNode->next_ = newNode;
    currentNode->prev_ = newNode;
    _size++;
  } else {
    push_front(value);
  }

  --position;
  return position;
};
/**
 * @brief Обменивает содержимое данного списка с содержимым другого списка.
 *
 * Метод обменивает содержимое данного списка с содержимым другого списка.
 * Для безопасной операции обмена используется временный список, который
 * копирует содержимое данного списка. Затем содержимое данного списка
 * копируется из другого списка, а содержимое другого списка заменяется
 * скопированным содержимым временного списка.
 *
 * @tparam T Тип элементов, хранящихся в списке.
 * @param other Ссылка на другой список, с которым производится обмен.
 */
template <typename T> void List<T>::swap(List &other) noexcept {
  // Создаем временный список и копируем содержимое данного списка в него
  List<T> tempList(*this);

  // Копируем содержимое другого списка в данную
  this->copyList(other);

  // Заменяем содержимое другого списка скопированным содержимым временного
  // списка
  other.copyList(tempList);
}

/**
 * @brief Объединяет содержимое данного списка с содержимым другого списка.
 *
 * Метод объединяет содержимое данного списка с содержимым другого списка.
 * Элементы из другого списка вставляются в правильные позиции данного списка,
 * чтобы сохранить упорядоченность. После успешного объединения содержимого
 * другого списка, он очищается.
 *
 * @tparam T Тип элементов, хранящихся в списке.
 * @param other Ссылка на другой список, который будет объединен с данным
 * списком.
 */
template <typename T> void List<T>::merge(List &other) {
  // Итераторы для прохода по текущему и другому списку
  iterator current = cBegin();
  iterator otherIter = other.cBegin();

  // Если данный список пуст, просто добавляем элементы из другого списка
  if (empty()) {
    while (otherIter != other.cEnd()) {
      push_back(*otherIter);
      ++otherIter;
    }
    other.clear();
    return;
  }

  // Проходим по обоим спискам, объединяя их элементы
  while (current != cEnd() && otherIter != other.cEnd()) {
    if (*otherIter <= *current) {
      insert(current, *otherIter);
      ++otherIter;
    } else {
      ++current;
    }
  }

  // Добавляем оставшиеся элементы из другого списка
  while (otherIter != other.cEnd()) {
    push_back(*otherIter);
    ++otherIter;
  }

  // Очищаем другой список после успешного объединения
  other.clear();
}
/**
 * @brief Инвертирует порядок элементов в списке.
 *
 * Этот метод изменяет порядок элементов в связанном списке типа List.
 * Он проходит через список, меняя местами значения элементов от начала к концу
 * и от конца к началу. Метод работает в линейном времени и не бросает
 * исключений.
 *
 * @tparam T Тип элементов, хранящихся в списке.
 */
template <typename T> void List<T>::reverse() noexcept {
  Node *current = _head;
  Node *last = _tail;
  T tmp;
  while (current != last && current != last->next_) {
    tmp = current->value_;
    current->value_ = last->value_;
    last->value_ = tmp;
    current = current->next_;
    last = last->prev_;
  }
}

/**
 * @brief Сортирует элементы в списке по возрастанию.
 *
 * Этот метод сортирует элементы в связанном списке типа List по возрастанию.
 * Он использует алгоритм сортировки пузырьком. Метод работает в линейном
 * времени в лучшем случае и квадратичном времени в худшем случае, но не бросает
 * исключений.
 *
 * @tparam T Тип элементов, хранящихся в списке.
 */
template <typename T> void List<T>::sort() noexcept {
  if (_head == nullptr || _head->next_ == nullptr) {
    return; // Возврат, если список пуст или содержит только один элемент
  }

  bool swapped;
  do {
    swapped = false;
    Node *currentNode = _head;
    while (currentNode->next_ != _end) {
      Node *nextNode = currentNode->next_;
      if (currentNode->value_ > nextNode->value_) {
        // Меняем местами значения текущего и следующего элементов
        T temp = currentNode->value_;
        currentNode->value_ = nextNode->value_;
        nextNode->value_ = temp;
        swapped = true;
      }
      currentNode = nextNode;
    }
  } while (swapped); // Повторяем, пока есть элементы для обмена
};

/**
 * @brief Удаляет узел из списка.
 *
 * Этот метод удаляет заданный узел из связанного списка. Если передан узел,
 * который равен nullptr, метод завершает выполнение без изменений.
 *
 * @param node_to_remove Указатель на узел, который необходимо удалить.
 *                       Если nullptr, то метод завершает выполнение.
 * @tparam T Тип данных, хранящихся в списке.
 */
template <typename T> void List<T>::remove_node(Node *node_to_remove) {
  // Если передан nullptr, завершаем выполнение метода
  if (node_to_remove == nullptr) {
    return;
  }

  // Обновляем связи соседних узлов
  if (node_to_remove->prev_) {
    node_to_remove->prev_->next_ = node_to_remove->next_;
  } else {
    // Удаляемый узел был первым - обновляем начало списка
    _head = node_to_remove->next_;
  }

  if (node_to_remove->next_) {
    node_to_remove->next_->prev_ = node_to_remove->prev_;
  } else {
    // Удаляемый узел был последним - обновляем конец списка
    _tail = node_to_remove->prev_;
  }

  // Удаляем узел и уменьшаем размер списка
  delete node_to_remove;
  _size--;
}
/**
 * @brief Возвращает константную ссылку на первый элемент списка.
 *
 * Этот метод возвращает константную ссылку на значение первого элемента в
 * списке. Если список не пуст, возвращается значение, хранящееся в голове
 * списка. В противном случае возвращается значение error_value.
 *
 * @return Константная ссылка на первый элемент списка или значение error_value,
 * если список пуст.
 */
template <typename T>
typename List<T>::const_reference List<T>::front() noexcept {
  if (_head)
    return _head->value_;
  return error_value;
};

/**
 * @brief Возвращает константную ссылку на последний элемент списка.
 *
 * Этот метод возвращает константную ссылку на значение последнего элемента в
 * списке. Если список не пуст, возвращается значение, хранящееся в хвосте
 * списка. В противном случае возвращается значение error_value.
 *
 * @return Константная ссылка на последний элемент списка или значение
 * error_value, если список пуст.
 */
template <typename T>
typename List<T>::const_reference List<T>::back() noexcept {
  if (_tail)
    return _tail->value_;
  return error_value;
};

/**
 * @brief Вставляет несколько элементов перед указанным итератором.
 *
 * Этот метод позволяет вставить несколько элементов в список перед заданным
 * итератором. Параметры Args... args представляют собой переменное количество
 * аргументов, которые будут вставлены. Метод использует переданный итератор pos
 * и вставляет новые элементы перед ним, используя метод insert_range.
 *
 * @tparam Args Типы аргументов для вставки.
 * @param pos Итератор, перед которым следует вставить элементы.
 * @param args Аргументы для вставки.
 * @return Итератор на первый вставленный элемент.
 */
template <typename T>
template <typename... Args>
typename List<T>::iterator List<T>::insert_many(const_iterator pos,
                                                Args &&...args) {
  auto values = {std::forward<Args>(args)...};
  return insert_range(pos, values.begin(), values.end());
}
/**
 * @brief Вставляет диапазон элементов перед указанной позицией.
 *
 * Данный метод вставляет диапазон элементов, заданных итераторами `first` и
 * `last`, перед позицией `pos` в списке. Итератор `pos` указывает на элемент,
 * перед которым будет вставлен диапазон. При этом увеличивается размер списка
 * на количество вставленных элементов.
 *
 * @tparam T Тип элементов в списке.
 * @tparam InputIt Тип итератора для диапазона элементов.
 * @param pos Итератор на позицию, перед которой следует вставить диапазон.
 * @param first Итератор на начало диапазона элементов.
 * @param last Итератор на конец диапазона элементов.
 * @return Итератор на первый вставленный элемент.
 */
template <typename T>
template <typename InputIt>
typename List<T>::iterator List<T>::insert_range(const_iterator pos,
                                                 InputIt first, InputIt last) {
  Node *nodeBeforeInsertPosition = pos._node->prev_;
  size_t inserted_elements_count = std::distance(first, last);
  _size += inserted_elements_count;

  for (; first != last; ++first) {
    Node *newNode = new Node(std::forward<decltype(*first)>(*first));

    if (nodeBeforeInsertPosition) {
      newNode->next_ = nodeBeforeInsertPosition->next_;
      newNode->prev_ = nodeBeforeInsertPosition;
      if (nodeBeforeInsertPosition->next_)
        nodeBeforeInsertPosition->next_->prev_ = newNode;
      nodeBeforeInsertPosition->next_ = newNode;
      if (nodeBeforeInsertPosition == _tail) {
        _tail = newNode;
      }
    } else {
      if (_head) {
        _head->prev_ = newNode;
      }
      newNode->next_ = _head;
      _head = newNode;
    }
    nodeBeforeInsertPosition = newNode;
  }

  return iterator(nodeBeforeInsertPosition);
}

/**
 * @brief Вспомогательный метод для вставки элементов в начало списка.
 *
 * Данный метод рекурсивно вставляет элементы в начало списка, используя
 * параметры шаблона Pack Expansions. Метод используется в методе
 * `insert_many_front`.
 *
 * @tparam T Тип элементов в списке.
 * @tparam First Тип первого элемента для вставки.
 * @tparam Rest Остальные типы элементов для вставки.
 * @param first Первый элемент для вставки.
 * @param rest Остальные элементы для вставки.
 */
template <typename T>
template <typename First, typename... Rest>
void List<T>::insert_front_helper(First &&first, Rest &&...rest) {
  if constexpr (sizeof...(rest) > 0) {
    insert_front_helper(std::forward<Rest>(rest)...);
  }
  push_front(std::forward<First>(first));
}

/**
 * @brief Вставляет несколько элементов в начало списка.
 *
 * Данный метод позволяет вставить произвольное количество элементов
 * в начало списка. Элементы передаются в метод как параметры.
 * Используется вспомогательный метод `_insert_front_helper` для вставки.
 *
 * @tparam T Тип элементов в списке.
 * @tparam Args Типы элементов для вставки.
 * @param args Элементы для вставки.
 */
template <typename T>
template <typename... Args>
void List<T>::insert_many_front(Args &&...args) {
  insert_front_helper(std::forward<Args>(args)...);
}

} // namespace s21