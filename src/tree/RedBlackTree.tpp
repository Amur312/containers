#include "RedBlackTree.h"

namespace s21 {

/**
 * @brief Конструктор по умолчанию для красно-черного дерева.
 * Создает пустое красно-черное дерево.
 */
template <typename Key, typename Comparator>
RedBlackTree<Key, Comparator>::RedBlackTree()
    : head_(new RedBlackTreeNode), size_(0U) {}

/**
 * @brief Конструктор копирования для красно-черного дерева.
 * Создает красно-черное дерево, являющееся копией другого дерева.
 *
 * @param other Дерево, которое нужно скопировать.
 */
template <typename Key, typename Comparator>
RedBlackTree<Key, Comparator>::RedBlackTree(const RedBlackTree &other)
    : RedBlackTree() {
  if (other.Size() > 0) {
    CopyTreeFromOther(other);
  }
}

/**
 * @brief Перемещающий конструктор для красно-черного дерева.
 *
 * Инициализирует текущий объект пустым деревом, а затем обменивает его
 * содержимое с переданным объектом, позволяя эффективно "переместить" данные.
 *
 * @param other Дерево, содержимое которого будет перемещено в текущий объект.
 * @return Ничего не возвращает, так как это конструктор.
 */
template <typename Key, typename Comparator>
RedBlackTree<Key, Comparator>::RedBlackTree(RedBlackTree &&other) noexcept
    : RedBlackTree() {
  // Проверка на самоприсваивание: если other и this указывают на один и тот же
  // объект
  if (this != &other) {
    Swap(other);
  }
}
/**
 * @brief Оператор присваивания копирует содержимое другого красно-черного
 * дерева в текущее. Если текущее дерево и другое дерево являются одним и тем же
 * объектом, ничего не происходит. В противном случае текущее дерево очищается,
 * и в него копируются все элементы из другого дерева.
 *
 * @param other Другое красно-черное дерево, содержимое которого нужно
 * скопировать
 * @return Ссылка на текущее дерево после копирования
 */
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTree &
RedBlackTree<Key, Comparator>::operator=(const RedBlackTree &other) {
  if (this != &other) { // Проверка на самоприсваивание
    Clear();
    if (other.Size() > 0) {
      CopyTreeFromOther(other);
    }
  }

  return *this;
}

/**
 * @brief Перемещающий оператор присваивания.
 * Очищает текущее дерево и обменивает его содержимое с другим деревом.
 * После этой операции, другое дерево будет в состоянии, которое было у текущего
 * дерева до операции.
 *
 * @param other Другое дерево, с которым происходит обмен содержимым.
 * @return Ссылка на текущий экземпляр дерева после обмена.
 */
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTree &
RedBlackTree<Key, Comparator>::operator=(RedBlackTree &&other) noexcept {
  Clear();
  Swap(other); // Обмениваем содержимое текущего дерева с другим деревом.
  return *this; // Возвращаем ссылку на текущий экземпляр дерева.
}

/**
 * @brief Деструктор класса красно-черного дерева
 *
 * Уничтожает все узлы дерева, освобождая занимаемую память,
 * и удаляет фиктивный узел (head_).
 */
template <typename Key, typename Comparator>
RedBlackTree<Key, Comparator>::~RedBlackTree() {
  Clear();
  delete head_;
  head_ = nullptr;
}

/**
 * @brief Очистка дерева
 *
 * Этот метод удаляет все узлы дерева и устанавливает его размер равным нулю.
 * Он использует вспомогательный метод Destroy для удаления всех узлов, затем
 * инициализирует "голову" дерева с помощью метода InitializeHead.
 */
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Clear() noexcept {
  // Удаление всех узлов, начиная с корневого узла
  Destroy(head_->parent_);

  // Инициализация "головы" дерева
  InitializeHead();

  // Установка размера дерева равным нулю
  size_ = 0;
}

/**
 * @brief Возвращает размер красно-черного дерева.
 *
 * @return Количество элементов в дереве.
 */
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::size_type
RedBlackTree<Key, Comparator>::Size() const noexcept {
  return size_;
}
/**
 * @brief Проверяет, пусто ли красно-черное дерево.
 *
 * @return Возвращает true, если дерево пусто, иначе false.
 */
template <typename Key, typename Comparator>
bool RedBlackTree<Key, Comparator>::Empty() const noexcept {
  return size_ == 0;
}

/**
 * @brief Возвращает максимально возможный размер красно-черного дерева.
 *
 * @return Максимальное количество элементов, которое можно хранить в дереве.
 */
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::size_type
RedBlackTree<Key, Comparator>::MaxSize() const noexcept {
  return ((std::numeric_limits<size_type>::max() / 2) - sizeof(RedBlackTree) -
          sizeof(RedBlackTreeNode)) /
         sizeof(RedBlackTreeNode);
}

/**
 * @brief Возвращает итератор к началу дерева (самому левому узлу)
 *
 * @return Итератор к началу дерева
 */
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::Begin() noexcept {
  return iterator(head_->left_);
}
/**
 * @brief Возвращает константный итератор к началу дерева (самому левому узлу)
 *
 * @return Константный итератор к началу дерева
 */
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::const_iterator
RedBlackTree<Key, Comparator>::Begin() const noexcept {
  return const_iterator(head_->left_);
}

/**
 * @brief Возвращает итератор к концу дерева (фиктивному узлу head_)
 *
 * @return Итератор к концу дерева
 */
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::End() noexcept {
  return iterator(head_);
}

/**
 * @brief Возвращает константный итератор к концу дерева (фиктивному узлу head_)
 *
 * @return Константный итератор к концу дерева
 */
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::const_iterator
RedBlackTree<Key, Comparator>::End() const noexcept {
  return const_iterator(head_);
}

/**
 * @brief Сливает дерево other с текущим деревом.
 *
 * @param other Дерево, которое будет объединено с текущим деревом.
 */
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Merge(RedBlackTree &other) {
  // Проверяем, что дерево other не является текущим деревом (this).
  if (this != &other) {
    // Если дерево other пустое, нет необходимости делать слияние.
    if (other.Empty())
      return;
    // Если текущее дерево пустое, можно просто переместить содержимое other в
    // текущее дерево.
    if (this->Empty()) {
      *this = std::move(other);
      return;
    }
    // Итерируемся через все элементы дерева other.
    for (auto it = other.Begin(); it != other.End(); ++it) {
      // Вставляем каждый элемент в текущее дерево.
      this->Insert(*it);
    }
    // Очищаем дерево other, так как его элементы уже были перемещены в текущее
    // дерево.
    other.Clear();
  }
}

/**
 * @brief Объединяет текущее дерево с другим деревом, добавляя только уникальные
 * значения. Вставляет элементы из другого дерева, которых нет в текущем дереве.
 *
 * @param other Другое дерево, с которым происходит объединение.
 */
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::MergeUnique(RedBlackTree &other) {
  if (this != &other) {
    iterator other_iter = other.Begin();
    const iterator other_end = other.End();

    // Итерируемся по элементам дерева other.
    while (other_iter != other_end) {
      const Key &current_key = other_iter.node_->key_;

      // Ищем текущий ключ в текущем дереве.
      iterator found_iter = Find(current_key);

      // Если ключ не найден, перемещаем его из other в текущее дерево.
      if (found_iter == End()) {
        iterator tmp = other_iter;
        ++other_iter;

        // Извлекаем узел из дерева other и вставляем его в текущее дерево.
        RedBlackTreeNode *moving_node = other.ExtractNode(tmp);
        Insert(head_->parent_, moving_node, false);
      } else {
        ++other_iter;
      }
    }

    // После перемещения уникальных элементов, очищаем дерево other.
    other.Clear();
  }
}

/**
 * @brief Вставляет элемент с заданным ключом в дерево.
 *
 * @param key Ключ для вставки.
 * @return Итератор, указывающий на вставленный элемент, либо на существующий
 * элемент с таким же ключом. Если ключ уже существует, то возвращается также
 * флаг "false".
 */
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::Insert(const key_type &key) {
  RedBlackTreeNode *newNode = new RedBlackTreeNode{key};
  return Insert(head_->parent_, newNode, false).first;
}

/**
 * @brief Вставляет уникальный элемент с заданным ключом в дерево.
 *
 * @param key Ключ для вставки.
 * @return Пара, содержащая итератор, указывающий на вставленный элемент (если
 * вставлен), и флаг, указывающий на успешность операции вставки. Если ключ уже
 * существует, возвращается итератор на существующий элемент и "false".
 */
template <typename Key, typename Comparator>
std::pair<typename RedBlackTree<Key, Comparator>::iterator, bool>
RedBlackTree<Key, Comparator>::InsertUnique(const key_type &key) {
  iterator it = Find(key);
  if (it != End()) {
    return {it, false};
  }
  RedBlackTreeNode *newNode = new RedBlackTreeNode{key};
  std::pair<iterator, bool> result = Insert(head_->parent_, newNode, true);
  return result;
}

/**
 * @brief Вставляет элементы в дерево, используя переданные аргументы, и
 * возвращает вектор пар итераторов и флагов успешной вставки для каждого
 * элемента.
 *
 * @tparam Key Тип ключа элемента.
 * @tparam Comparator Функция сравнения ключей.
 * @tparam Args Параметры элементов для вставки.
 * @param args Аргументы для создания элементов.
 * @return Вектор пар итераторов и флагов успешной вставки для каждого элемента.
 */
template <typename Key, typename Comparator>
template <typename... Args>
std::vector<std::pair<typename RedBlackTree<Key, Comparator>::iterator, bool>>
RedBlackTree<Key, Comparator>::Emplace(Args &&...args) {
  std::vector<std::pair<iterator, bool>>
      insertion_results; // Результаты вставки будут храниться здесь.
  insertion_results.reserve(sizeof...(
      args)); // Резервируем место для ожидаемого количества элементов.

  // Лямбда-функция для вставки элемента в дерево.
  auto emplaceItem = [&](auto &&item) {
    RedBlackTreeNode *newNode = new RedBlackTreeNode(
        std::forward<decltype(item)>(item)); // Создаем новый узел.
    std::pair<iterator, bool> insertion_result =
        Insert(head_->parent_, newNode, false); // Вставляем узел в дерево.
    insertion_results.push_back(
        insertion_result); // Добавляем результат в вектор.
  };

  // Используем оператор запятой и шаблон развертывания параметров для вставки
  // каждого элемента.
  (emplaceItem(std::forward<Args>(args)), ...);

  return insertion_results; // Возвращаем вектор с результатами вставки.
}

/**
 * @brief Вставляет уникальные элементы в дерево, используя переданные
 * аргументы, и возвращает вектор пар итераторов и флагов успешной вставки для
 * каждого элемента.
 *
 * @tparam Key Тип ключа элемента.
 * @tparam Comparator Функция сравнения ключей.
 * @tparam Args Параметры элементов для вставки.
 * @param args Аргументы для создания элементов.
 * @return Вектор пар итераторов и флагов успешной вставки для каждого элемента.
 */
template <typename Key, typename Comparator>
template <typename... Args>
std::vector<std::pair<typename RedBlackTree<Key, Comparator>::iterator, bool>>
RedBlackTree<Key, Comparator>::EmplaceUnique(Args &&...args) {
  std::vector<std::pair<iterator, bool>>
      insertion_results; // Результаты вставки будут храниться здесь.
  insertion_results.reserve(sizeof...(
      args)); // Резервируем место для ожидаемого количества элементов.

  // Лямбда-функция для вставки уникального элемента в дерево.
  auto emplaceUniqueItem = [&](auto &&item) {
    RedBlackTreeNode *newNode = new RedBlackTreeNode(
        std::forward<decltype(item)>(item)); // Создаем новый узел.
    std::pair<iterator, bool> insertion_result =
        Insert(head_->parent_, newNode,
               true); // Вставляем узел в дерево с уникальностью.
    if (!insertion_result.second) {
      delete newNode; // Если вставка не удалась из-за дубликата, удаляем узел.
    }
    insertion_results.push_back(
        insertion_result); // Добавляем результат в вектор.
  };

  // Используем оператор запятой и шаблон развертывания параметров для вставки
  // каждого элемента.
  (emplaceUniqueItem(std::forward<Args>(args)), ...);

  return insertion_results; // Возвращаем вектор с результатами вставки.
}

/**
 * @brief Находит элемент в дереве по заданному ключу.
 *
 * @param key Ключ, по которому выполняется поиск элемента.
 * @return Итератор на найденный элемент, если найден, или итератор к концу
 * дерева, если не найден.
 */
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::Find(const_reference key) {
  RedBlackTreeNode *current_node = head_->parent_;
  while (current_node) {
    if (key_comparator_(
            key, current_node->key_)) { // Если key меньше ключа текущего узла.
      current_node = current_node->left_; // Переходим к левому потомку.
    } else if (key_comparator_(current_node->key_,
                               key)) { // Если ключ текущего узла меньше key.
      current_node = current_node->right_; // Переходим к правому потомку.
    } else {
      // Найдено точное совпадение ключей, возвращаем итератор на этот узел.
      return iterator(current_node);
    }
  }

  // Ключ не найден, возвращаем итератор к концу дерева.
  return End();
}

/**
 * @brief Находит итератор на ближайший элемент дерева, не меньший заданному
 * ключу.
 *
 * Этот метод выполняет бинарный поиск по дереву для нахождения первого
 * элемента, который больше или равен заданному ключу.
 *
 * @param key Ключ, для которого ищется ближайший элемент не меньший него.
 * @return Итератор на ближайший элемент дерева, не меньший заданному ключу.
 */
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::LowerBound(const_reference key) {
  RedBlackTreeNode *current_node = head_->parent_;
  RedBlackTreeNode *result_node = End().node_;

  while (current_node) {
    // Если ключ текущего узла не меньше заданного ключа
    if (!key_comparator_(current_node->key_, key)) {
      result_node = current_node; // Обновляем ближайший найденный узел
      current_node = current_node->left_; // Двигаемся влево
    } else {
      current_node = current_node->right_; // Двигаемся вправо
    }
  }

  // Возвращаем итератор на ближайший найденный узел
  return iterator(result_node);
}

/**
 * @brief Находит итератор на следующий элемент, который больше указанного
 * ключа.
 *
 * Находит наименьший элемент, ключ которого больше заданного ключа.
 *
 * @param key Ключ для поиска верхней границы.
 * @return Итератор на элемент, ключ которого больше заданного, либо End(), если
 * такого элемента нет.
 */
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::iterator
RedBlackTree<Key, Comparator>::UpperBound(const_reference key) {
  RedBlackTreeNode *current = head_->parent_; // Текущий узел, начиная с корня.
  RedBlackTreeNode *result =
      End().node_; // Результат - итератор на следующий элемент.

  while (current != nullptr) {
    if (key_comparator_(
            key,
            current->key_)) { // Если ключ меньше текущего узла, идем налево.
      result = current; // Обновляем итератор на следующий элемент.
      current = current->left_;
    } else { // Если ключ больше или равен текущему узлу, идем направо.
      current = current->right_;
    }
  }

  return iterator(result); // Возвращаем итератор на следующий элемент.
}

/**
 * @brief Удаляет элемент из дерева по переданному итератору.
 * Извлекает узел, соответствующий переданному итератору, удаляет его и
 * освобождает память.
 *
 * @param position Итератор, указывающий на удаляемый элемент.
 */
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Erase(iterator position) noexcept {
  // Извлекаем узел по переданному итератору.
  RedBlackTreeNode *result = ExtractNode(position);

  // Удаляем извлеченный узел и освобождаем память.
  delete result;
}

/**
 * @brief Меняет местами содержимое текущего дерева с содержимым другого дерева.
 * Меняет указатели на голову, размер и компаратор между текущим и другим
 * деревом.
 *
 * @param other Другое дерево, с которым происходит обмен содержимым.
 */
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Swap(RedBlackTree &other) noexcept {
  std::swap(head_, other.head_); // Меняем указатели на голову дерева.
  std::swap(size_, other.size_); // Меняем размеры деревьев.
  std::swap(key_comparator_,
            other.key_comparator_); // Меняем компараторы деревьев.
}

/**
 * @brief Копирует структуру и содержимое дерева из другого дерева.
 * Очищает текущее дерево и создает его копию на основе другого дерева.
 * Затем обновляет указатели на наибольший и наименьший элементы,
 * размер, и компаратор для текущего дерева.
 *
 * @param other Другое дерево, из которого копируется структура и содержимое.
 */
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::CopyTreeFromOther(
    const RedBlackTree &other) {
  // Очищаем текущее дерево.
  Clear();

  // Копируем структуру дерева из другого дерева.
  head_->parent_ = CopyTree(other.head_->parent_, nullptr);
  head_->parent_->parent_ = head_;

  // Обновляем указатели на наибольший и наименьший элементы.
  RedBlackTreeNode *node = head_->parent_;
  while (node->left_)
    node = node->left_;
  head_->left_ = node;

  node = head_->parent_;
  while (node->right_)
    node = node->right_;
  head_->right_ = node;

  // Обновляем размер и компаратор для текущего дерева.
  size_ = other.size_;
  key_comparator_ = other.key_comparator_;
}

/**

@brief Создает копию поддерева, начиная с указанного узла.
    * @param node Указатель на корень поддерева,
    * которое нужно скопировать.
    *
    * @param parent Указатель на родительский узел в новой копии (по умолчанию
    * nullptr).
    *
    * @return Указатель на корень новой копии поддерева.
    * Этот метод выполняет глубокую копию поддерева, начиная с заданного узла.
    *
    * Для каждого узла из исходного поддерева создается новый узел в новой
    * ыкопии,
    * копируются ключ и цвет. При этом сохраняется структура дерева.
    *
    * Возвращается указатель на корень новой копии поддерева.
*/
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::CopyTree(const RedBlackTreeNode *source_node,
                                        RedBlackTreeNode *new_parent) {
  if (!source_node)
    return nullptr;

  // Используем стек для обхода узлов поддерева в глубину
  std::stack<std::pair<const RedBlackTreeNode *, RedBlackTreeNode **>>
      node_stack;
  RedBlackTreeNode *new_subtree_root = nullptr;
  node_stack.push({source_node, &new_subtree_root});

  while (!node_stack.empty()) {
    auto [current_source_node, new_node_ptr] = node_stack.top();
    node_stack.pop();

    if (current_source_node) {
      // Создаем новый узел в новой копии
      *new_node_ptr = new RedBlackTreeNode{current_source_node->key_,
                                           current_source_node->color_};
      (*new_node_ptr)->parent_ = new_parent;

      // Добавляем дочерние узлы и соответствующие указатели на них в стек
      node_stack.push({current_source_node->right_, &(*new_node_ptr)->right_});
      node_stack.push({current_source_node->left_, &(*new_node_ptr)->left_});
    }
  }

  return new_subtree_root;
}
/**
 * @brief Рекурсивно удаляет узлы дерева, начиная с заданного узла.
 * Использует стек для обхода узлов и их последовательного удаления.
 *
 * @param node Узел, с которого начинается удаление.
 */
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::Destroy(RedBlackTreeNode *node) noexcept {
  std::stack<RedBlackTreeNode *> nodes;

  while (node != nullptr || !nodes.empty()) {
    if (node != nullptr) {
      // Добавляем текущий узел в стек и переходим к левому потомку
      nodes.push(node);
      node = node->left_;
    } else {
      // Если левых потомков нет, извлекаем узел из стека,
      // удаляем его и переходим к его правому потомку
      node = nodes.top();
      nodes.pop();

      RedBlackTreeNode *right_child = node->right_;
      delete node;

      node = right_child;
    }
  }
}

/**
 * @brief Инициализирует фиктивный узел head_ и связанные с ним указатели.
 *
 * Устанавливает указатели в head_ таким образом, что дерево будет пустым.
 *
 * Функция не принимает никаких аргументов и ничего не возвращает.
 * Этот метод обеспечивает корректное начальное состояние дерева.
 */
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::InitializeHead() noexcept {
  // Создание фиктивного узла head_, если он не был создан ранее.
  if (!head_) {
    head_ = new RedBlackTreeNode();
  }
  head_->parent_ = nullptr;
  head_->left_ = head_;
  head_->right_ = head_;
}
/**
 * @brief Проверяет корректность структуры и свойств красно-черного дерева.
 *
 * @return `true`, если дерево корректно; `false`, если есть нарушения.
 */
template <typename Key, typename Comparator>
bool RedBlackTree<Key, Comparator>::CheckTree() const noexcept {
  // Проверка корректности корневого узла
  if (head_->color_ == BLACK) {
    return false;
  }
  // Получение корневого узла дерева
  RedBlackTreeNode *root = head_->parent_;

  // Пустое дерево считается корректным
  if (root == nullptr) {
    return true;
  }

  // Проверка свойств красно-черного дерева:
  // 1. Корневой узел не может быть красным
  // 2. Для каждого красного узла его дети должны быть черными
  // 3. Чёрные высоты поддеревьев должны быть равны

  // Если корневой узел красный или нарушено условие для красных узлов,
  // возвращаем false
  if (root->color_ == RED || !checkRedNodes(root) ||
      ComputeBlackHeight(root) == -1) {
    return false;
  }

  // Дерево прошло все проверки и считается корректным
  return true;
}

/**
 * @brief Вставляет новый узел в красно-черное дерево, начиная от указанного
 * корневого узла.
 *
 * @param root Корневой узел, с которого начинается вставка.
 * @param newNnode Новый узел для вставки.
 * @param check_duplicates Если true, то вставляются только уникальные ключи.
 * @return Пара, содержащая итератор на вставленный узел и флаг, указывающий на
 * успешность вставки.
 */
template <typename KeyType, typename Compare>
std::pair<typename RedBlackTree<KeyType, Compare>::iterator, bool>
RedBlackTree<KeyType, Compare>::Insert(RedBlackTreeNode *root,
                                       RedBlackTreeNode *newNnode,
                                       bool check_duplicates) {
  RedBlackTreeNode *parent = head_;
  RedBlackTreeNode **node = &root;

  // Находим место для вставки нового узла.
  while (*node != nullptr) {
    parent = *node;
    bool is_less = key_comparator_(newNnode->key_, (*node)->key_);
    if (is_less) {
      node = &(*node)->left_;
    } else {
      if (check_duplicates && !key_comparator_((*node)->key_, newNnode->key_)) {
        return {iterator(*node), false}; // Уже существующий ключ.
      }
      node = &(*node)->right_;
    }
  }

  // Производим вставку нового узла.
  newNnode->parent_ = parent;
  *node = newNnode;

  // Обновляем связи в родительском узле.
  if (parent == head_ || parent == nullptr) {
    newNnode->color_ = BLACK;
    head_->parent_ = newNnode;
    head_->left_ = newNnode;
    head_->right_ = newNnode;
  } else {
    if (key_comparator_(newNnode->key_, head_->left_->key_)) {
      head_->left_ = newNnode;
    }
    if (!key_comparator_(newNnode->key_, head_->right_->key_)) {
      head_->right_ = newNnode;
    }
  }

  ++size_;
  BalancingInsert(newNnode); // Выполняем балансировку после вставки.

  return {
      iterator(newNnode),
      true}; // Возвращаем итератор на вставленный узел и флаг успешной вставки.
}
/**
 * @brief Выполняет балансировку красно-черного дерева после вставки нового
 * узла. Метод проверяет и корректирует баланс дерева, чтобы сохранить его
 * свойства.
 *
 * @param node Узел, который был только что вставлен в дерево.
 */
template <typename KeyType, typename Compare>
void RedBlackTree<KeyType, Compare>::BalancingInsert(RedBlackTreeNode *node) {
  while (node != head_->parent_ && node->parent_->color_ == RED) {
    if (node->parent_->parent_->left_ == node->parent_) {
      HandleLeftCase(node); // Вызываем метод для обработки левого случая.
    } else {
      HandleRightCase(
          node); // Иначе вызываем метод для обработки правого случая.
    }
  }

  head_->parent_->color_ = BLACK; // Корень всегда должен быть черным.
}

/**
 * @brief Обрабатывает левый случай при балансировке красно-черного дерева после
 * вставки узла. В данном случае узел, его родитель и дедушка образуют
 * "зигзагообразную" структуру, которая требует коррекции для сохранения баланса
 * и свойств дерева.
 *
 * @param node Узел, который был только что вставлен в дерево.
 */
template <typename KeyType, typename Compare>
void RedBlackTree<KeyType, Compare>::HandleLeftCase(RedBlackTreeNode *node) {
  RedBlackTreeNode *parent = node->parent_;
  RedBlackTreeNode *gparent = parent->parent_;
  RedBlackTreeNode *uncle = gparent->right_;

  if (uncle != nullptr && uncle->color_ == RED) {
    HandleRedUncle(
        parent, uncle,
        gparent); // Вызываем метод для обработки случая с красным дядей.
  } else {
    if (parent->right_ == node) { // Если узел находится справа от родителя...
      RotateLeft(parent); // ...поворачиваем родителя влево.
      std::swap(parent, node); // Меняем местами узел и родителя.
    }
    RotateRight(gparent); // Поворачиваем дедушку вправо.
    std::swap(parent->color_,
              gparent->color_); // Меняем цвет родителя и дедушки.
  }
}

/**
 * @brief Обрабатывает правый случай при балансировке красно-черного дерева
 * после вставки узла. В данном случае узел, его родитель и дедушка образуют
 * "зигзагообразную" структуру, которая требует коррекции для сохранения баланса
 * и свойств дерева.
 *
 * @param node Узел, который был только что вставлен в дерево.
 */
template <typename KeyType, typename Compare>
void RedBlackTree<KeyType, Compare>::HandleRightCase(RedBlackTreeNode *node) {
  RedBlackTreeNode *parent = node->parent_;
  RedBlackTreeNode *gparent = parent->parent_;
  RedBlackTreeNode *uncle = gparent->left_;

  if (uncle != nullptr && uncle->color_ == RED) {
    HandleRedUncle(
        parent, uncle,
        gparent); // Вызываем метод для обработки случая с красным дядей.
  } else {
    if (parent->left_ == node) { // Если узел находится слева от родителя...
      RotateRight(parent); // ...поворачиваем родителя вправо.
      std::swap(parent, node); // Меняем местами узел и родителя.
    }
    RotateLeft(gparent); // Поворачиваем дедушку влево.
    std::swap(parent->color_,
              gparent->color_); // Меняем цвет родителя и дедушки.
  }
}

/**
 * @brief Обрабатывает случай, когда родитель и дядя вставленного узла являются
 * красными узлами. В этом случае, цвета родителя, дяди и дедушки меняются,
 * чтобы поддержать баланс красно-черного дерева.
 *
 * @param parent Родитель вставленного узла.
 * @param uncle Дядя вставленного узла (брат родителя).
 * @param gparent Дедушка вставленного узла (родитель родителя).
 */
template <typename KeyType, typename Compare>
void RedBlackTree<KeyType, Compare>::HandleRedUncle(RedBlackTreeNode *parent,
                                                    RedBlackTreeNode *uncle,
                                                    RedBlackTreeNode *gparent) {
  parent->color_ = BLACK; // Родитель и дядя становятся черными.
  uncle->color_ = BLACK;
  gparent->color_ =
      RED; // Дедушка становится красным, чтобы сохранить свойства дерева.
}
/**
 * @brief Выполняет операцию вращения узла влево или вправо.
 *
 * @param node Узел, который будет вращаться.
 * @param rotateRight Если true, выполняется вращение вправо, иначе - влево.
 *                    Определяет направление вращения.
 */
template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::Rotate(RedBlackTreeNode *node,
                                               bool rotateRight) noexcept {
  RedBlackTreeNode *pivot = rotateRight ? node->left_ : node->right_;

  RedBlackTreeNode *parentBeforeRotation = node->parent_;

  if (node == head_->parent_) {
    head_->parent_ = pivot; // Обновляем корень дерева.
  } else if (parentBeforeRotation->left_ == node) {
    parentBeforeRotation->left_ = pivot; // Обновляем левый указатель родителя.
  } else {
    parentBeforeRotation->right_ =
        pivot; // Обновляем правый указатель родителя.
  }

  pivot->parent_ = parentBeforeRotation;

  if (rotateRight) {
    // Вращение вправо.
    node->left_ = pivot->right_;
    if (pivot->right_ != nullptr) {
      pivot->right_->parent_ = node;
    }
    pivot->right_ = node;
  } else {
    // Вращение влево.
    node->right_ = pivot->left_;
    if (pivot->left_ != nullptr) {
      pivot->left_->parent_ = node;
    }
    pivot->left_ = node;
  }

  node->parent_ = pivot;
}

/**
 * @brief Выполняет операцию вращения узла вправо.
 *
 * @param node Узел, который будет вращаться вправо.
 */
template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::RotateRight(
    RedBlackTreeNode *node) noexcept {
  Rotate(node,
         true); // Вызов общей функции Rotate с параметром rotateRight = true.
}

/**
 * @brief Выполняет операцию вращения узла влево.
 *
 * @param node Узел, который будет вращаться влево.
 */
template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::RotateLeft(
    RedBlackTreeNode *node) noexcept {
  Rotate(node,
         false); // Вызов общей функции Rotate с параметром rotateRight = false.
}

/**
 * @brief Извлекает узел из дерева по заданному итератору и выполняет
 * необходимые действия.
 *
 * @param position Итератор, указывающий на узел, который нужно извлечь.
 * @return Указатель на извлеченный узел или nullptr, если итератор указывает на
 * конец дерева.
 */
template <typename Key, typename Comparator>
typename RedBlackTree<Key, Comparator>::RedBlackTreeNode *
RedBlackTree<Key, Comparator>::ExtractNode(iterator position) noexcept {
  if (position == End()) {
    return nullptr;
  }
  RedBlackTreeNode *deleted_node = position.node_;
  // Обрабатываем случаи удаления узла с учетом его цвета и наличия потомков.
  HandleDeletionCases(deleted_node);
  // Обновляем связи с родителями и фиктивным узлом head.
  UpdateParentAndHeadLinks(deleted_node);
  // Уменьшаем размер дерева и обнуляем данные узла.
  --size_;
  deleted_node->ToDefault();
  return deleted_node;
}
/**
 * @brief Обрабатывает случаи удаления узла из красно-черного дерева.
 *
 * Если удаляемый узел имеет два потомка, вызывается метод HandleK2Case
 * для обработки данного случая. В противном случае, если цвет удаляемого узла
 * - черный, вызывается метод HandleBlackCases для обработки других вариантов
 * удаления.
 *
 * @param deleted_node Удаляемый узел.
 */
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::HandleDeletionCases(
    RedBlackTreeNode *deleted_node) {
  // Если есть оба потомка, обрабатываем случай K2.
  if (deleted_node->left_ != nullptr && deleted_node->right_ != nullptr) {
    HandleK2Case(deleted_node);
  }
  // Если удаляемый узел черный, обрабатываем случаи удаления с черными узлами.
  else if (deleted_node->color_ == BLACK) {
    HandleBlackCases(deleted_node);
  }
}

/**
 * @brief Обновляет связи родителя и head_ после удаления узла из дерева.
 *
 * @param deleted_node Узел, который был удален.
 */
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::UpdateParentAndHeadLinks(
    RedBlackTreeNode *deleted_node) {
  if (deleted_node == head_->parent_) {
    // Если удаляемый узел был корневым элементом, инициализируем head_.
    InitializeHead();
  } else {
    RedBlackTreeNode *parent = deleted_node->parent_;
    if (deleted_node == parent->left_)
      parent->left_ = nullptr;
    else
      parent->right_ = nullptr;

    // Обновляем ссылки head_ на самый левый и самый правый узлы, если они были
    // удалены.
    if (head_->left_ == deleted_node)
      head_->left_ = SearchMinimum(head_->parent_);
    if (head_->right_ == deleted_node)
      head_->right_ = SearchMaximum(head_->parent_);
  }
}

/**
 * @brief Обработка случая "K2" для балансировки дерева при удалении узла.
 *
 * При удалении узла, если у него есть правый потомок, этот метод находит
 * наименьший узел в правом поддереве удаляемого узла и меняет его местами с
 * удаляемым узлом. Это помогает сохранить свойства красно-черного дерева после
 * удаления.
 *
 * @param deleted_node Удаляемый узел, для которого выполняется обработка K2.
 */
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::HandleK2Case(
    RedBlackTreeNode *deleted_node) {
  // Находим наименьший узел в правом поддереве удаляемого узла.
  RedBlackTreeNode *replacement_node = SearchMinimum(deleted_node->right_);

  // Меняем местами удаляемый узел и наименьший узел в правом поддереве.
  SwapNodesAndUpdateAfterErase(deleted_node, replacement_node);
}

/**
 * @brief Обрабатывает случаи, когда удаляемый узел имеет черный цвет.
 * Если удаленный узел имеет черного родителя, метод решает, как корректно
 * поддерживать свойства красно-черного дерева.
 *
 * @param deleted_node Узел, который был удален из дерева.
 */
template <typename Key, typename Comparator>
void RedBlackTree<Key, Comparator>::HandleBlackCases(
    RedBlackTreeNode *deleted_node) {
  if ((deleted_node->left_ != nullptr && deleted_node->right_ == nullptr) ||
      (deleted_node->left_ == nullptr && deleted_node->right_ != nullptr)) {
    // Случай 1: У удаляемого узла есть один потомок. Заменяем его потомком.
    RedBlackTreeNode *replacement =
        deleted_node->left_ ? deleted_node->left_ : deleted_node->right_;
    SwapNodesAndUpdateAfterErase(deleted_node, replacement);
  } else if (deleted_node->left_ == nullptr &&
             deleted_node->right_ == nullptr) {
    // Случай 2: У удаляемого узла нет потомков. Балансируем дерево.
    EraseBalancing(deleted_node);
  }
}
/**
 * @brief Обменивает местами два узла в красно-черном дереве.
 *
 * Обменивает местами позиции и свойства (родителей, потомков, цвета) двух узлов
 * в красно-черном дереве.
 *
 * @param node_a Указатель на первый узел для обмена.
 * @param node_b Указатель на второй узел для обмена.
 */
template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::SwapNodes(
    RedBlackTreeNode *node_a, RedBlackTreeNode *node_b) noexcept {
  std::swap(node_a->parent_, node_b->parent_);
  std::swap(node_a->left_, node_b->left_);
  std::swap(node_a->right_, node_b->right_);
  std::swap(node_a->color_, node_b->color_);
}

/**
 * @brief Обновляет родителя узла на новый узел.
 *
 * Обновляет указатель на родительский узел, заменяя его на новый узел,
 * при этом учитывает, с какой стороны родитель был связан с текущим узлом.
 *
 * @param node     Указатель на узел, родителя которого нужно обновить.
 * @param newNode  Указатель на новый узел, который станет родителем.
 */
template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::UpdateParent(
    RedBlackTreeNode *node, RedBlackTreeNode *newNode) noexcept {
  if (node->parent_->left_ == node) {
    node->parent_->left_ = newNode;
  } else {
    node->parent_->right_ = newNode;
  }
}
/**
 * @brief Обменивает местами два узла и обновляет их свойства после удаления
 * одного из них.
 *
 * Обменивает местами позиции и свойства (родителей, потомков, цвета) двух узлов
 * в красно-черном дереве. При этом учитываются нюансы после удаления одного из
 * узлов, включая обновление указателей на потомков и родителей.
 *
 * @param node    Указатель на первый узел для обмена и удаления.
 * @param survivor Указатель на второй узел, который останется после обмена
 *                и который выжил после удаления.
 */
template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::SwapNodesAndUpdateAfterErase(
    RedBlackTreeNode *node, RedBlackTreeNode *survivor) noexcept {
  if (node == survivor)
    return;

  UpdateParent(node, survivor);
  UpdateParent(survivor, node);

  if (node == head_->parent_) {
    head_->parent_ = survivor;
  }

  SwapNodes(node, survivor);

  if (node->left_)
    node->left_->parent_ = node;
  if (node->right_)
    node->right_->parent_ = node;
  if (survivor->left_)
    survivor->left_->parent_ = survivor;
  if (survivor->right_)
    survivor->right_->parent_ = survivor;
}
/**
 * @brief Обрабатывает ситуацию, когда брат узла красный.
 * Метод выполняет перебалансировку для поддержания свойств красно-черного
 * дерева.
 *
 * @param parent Родительский узел для проверяемого узла.
 * @param check_node Узел, для которого выполняется перебалансировка.
 */
template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::HandleRedSibling(
    RedBlackTreeNode *parent, RedBlackTreeNode *&check_node) {
  RedBlackTreeNode *sibling =
      (check_node == parent->left_) ? parent->right_ : parent->left_;
  std::swap(sibling->color_, parent->color_);

  // Поворачиваем родительский узел влево или вправо в зависимости от позиции
  // проверяемого узла.
  if (check_node == parent->left_) {
    RotateLeft(parent);
  } else {
    RotateRight(parent);
  }

  parent = check_node->parent_;
}

/**
 * @brief Обрабатывает ситуацию, когда брат узла черный, и у него нет красных
 * детей. Метод выполняет перебалансировку для поддержания свойств
 * красно-черного дерева.
 *
 * @param parent Родительский узел для проверяемого узла.
 * @param check_node Узел, для которого выполняется перебалансировка.
 */
template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::HandleBlackSiblingWithBlackChildren(
    RedBlackTreeNode *parent, RedBlackTreeNode *&check_node) {
  RedBlackTreeNode *sibling =
      (check_node == parent->left_) ? parent->right_ : parent->left_;
  sibling->color_ = RED;

  // Если цвет родителя также красный, то меняем цвет родителя на черный и
  // завершаем обработку.
  if (parent->color_ == RED) {
    parent->color_ = BLACK;
    return;
  }

  // Переопределяем проверяемый узел и его родителя для продолжения балансировки
  // вверх по дереву.
  check_node = parent;
  parent = check_node->parent_;
}

/**
 * @brief Обрабатывает ситуацию, когда брат узла черный, а у него есть красный
 * ребенок. Этот метод выполняет перебалансировку для поддержания свойств
 * красно-черного дерева.
 *
 * @param parent Родительский узел для проверяемого узла.
 * @param check_node Узел, для которого выполняется перебалансировка.
 */
template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::HandleBlackSiblingWithRedChild(
    RedBlackTreeNode *parent, RedBlackTreeNode *&check_node) {
  RedBlackTreeNode *sibling =
      (check_node == parent->left_) ? parent->right_ : parent->left_;

  // Если проверяемый узел - левый ребенок родителя и левый ребенок брата
  // красный.
  if (check_node == parent->left_ && sibling->left_ != nullptr &&
      sibling->left_->color_ == RED &&
      (sibling->right_ == nullptr || sibling->right_->color_ == BLACK)) {
    std::swap(sibling->color_, sibling->left_->color_);
    RotateRight(sibling);
    sibling = parent->right_;
  }
  // Если проверяемый узел - правый ребенок родителя и правый ребенок брата
  // красный.
  else if (sibling->right_ != nullptr && sibling->right_->color_ == RED &&
           (sibling->left_ == nullptr || sibling->left_->color_ == BLACK)) {
    std::swap(sibling->color_, sibling->right_->color_);
    RotateLeft(sibling);
    sibling = parent->left_;
  }

  sibling->left_->color_ = BLACK;
  sibling->color_ = parent->color_;
  parent->color_ = BLACK;

  // Поворачиваем родительский узел влево или вправо в зависимости от позиции
  // проверяемого узла.
  if (check_node == parent->left_) {
    RotateLeft(parent);
  } else {
    RotateRight(parent);
  }
}

/**
 * @brief Балансирует дерево после удаления узла, чтобы сохранить свойства
 * красно-черного дерева.
 *
 * @param deleted_node Узел, который был удален и требует балансировки.
 */
template <typename KeyType, typename Comparator>
void RedBlackTree<KeyType, Comparator>::EraseBalancing(
    RedBlackTreeNode *deleted_node) noexcept {
  RedBlackTreeNode *node_to_check = deleted_node;
  RedBlackTreeNode *parent_node = deleted_node->parent_;

  // Пока проверяемый узел не достигнет корня и его цвет черный.
  while (node_to_check != head_->parent_ && node_to_check->color_ == BLACK) {
    if (node_to_check == parent_node->left_ ||
        node_to_check == parent_node->right_) {
      // Получаем брата проверяемого узла.
      RedBlackTreeNode *sibling_node = (node_to_check == parent_node->left_)
                                           ? parent_node->right_
                                           : parent_node->left_;

      // Если брат красный, выполняем балансировку для этой ситуации.
      if (sibling_node->color_ == RED) {
        HandleRedSibling(parent_node, node_to_check);
      }

      // Обновляем указатель на брата, так как он мог измениться.
      sibling_node = (node_to_check == parent_node->left_) ? parent_node->right_
                                                           : parent_node->left_;

      // Если брат черный и у него также черные дети, выполняем балансировку.
      if (sibling_node->color_ == BLACK &&
          (sibling_node->left_ == nullptr ||
           sibling_node->left_->color_ == BLACK) &&
          (sibling_node->right_ == nullptr ||
           sibling_node->right_->color_ == BLACK)) {
        HandleBlackSiblingWithBlackChildren(parent_node, node_to_check);
      } else {
        // В противном случае, выполняем балансировку для черного брата с
        // красным ребенком.
        HandleBlackSiblingWithRedChild(parent_node, node_to_check);
        break;
      }
    }
  }
}

/**
 * @brief Перемещается к левому дочернему узлу заданного узла.
 *
 * @param node Узел, для которого нужно найти левого потомка.
 * @return Указатель на левого потомка заданного узла.
 */
template <typename KeyType, typename Comparator>
typename RedBlackTree<KeyType, Comparator>::RedBlackTreeNode *
RedBlackTree<KeyType, Comparator>::GoLeft(
    typename RedBlackTree<KeyType, Comparator>::RedBlackTreeNode *node)
    const noexcept {
  return node->left_;
}

/**
 * @brief Перемещается к правому дочернему узлу заданного узла.
 *
 * @param node Узел, для которого нужно найти правого потомка.
 * @return Указатель на правого потомка заданного узла.
 */
template <typename KeyType, typename Comparator>
typename RedBlackTree<KeyType, Comparator>::RedBlackTreeNode *
RedBlackTree<KeyType, Comparator>::GoRight(
    typename RedBlackTree<KeyType, Comparator>::RedBlackTreeNode *node)
    const noexcept {
  return node->right_;
}

/**
 * @brief Ищет узел с минимальным ключом, начиная с заданного узла.
 *
 * @param node Узел, с которого начинается поиск.
 * @return Указатель на узел с минимальным ключом.
 */
template <typename KeyType, typename Comparator>
typename RedBlackTree<KeyType, Comparator>::RedBlackTreeNode *
RedBlackTree<KeyType, Comparator>::SearchMinimum(
    typename RedBlackTree<KeyType, Comparator>::RedBlackTreeNode *node)
    const noexcept {
  while (GoLeft(node) != nullptr) {
    node = GoLeft(node);
  };
  return node;
}

/**
 * @brief Ищет узел с максимальным ключом, начиная с заданного узла.
 *
 * @param node Узел, с которого начинается поиск.
 * @return Указатель на узел с максимальным ключом.
 */
template <typename KeyType, typename Comparator>
typename RedBlackTree<KeyType, Comparator>::RedBlackTreeNode *
RedBlackTree<KeyType, Comparator>::SearchMaximum(
    typename RedBlackTree<KeyType, Comparator>::RedBlackTreeNode *node)
    const noexcept {
  while (GoRight(node) != nullptr) {
    node = GoRight(node);
  };
  return node;
}

/**
 * @brief Вычисляет черную высоту поддерева, начиная с заданного узла.
 *
 * @param node Узел, с которого начинается вычисление.
 * @return Черная высота поддерева, если она корректна; -1, если есть нарушение.
 */
template <typename KeyType, typename Compare>
int RedBlackTree<KeyType, Compare>::ComputeBlackHeight(
    const RedBlackTreeNode *node) const noexcept {
  // Базовый случай: пустое поддерево имеет черную высоту 0
  if (node == nullptr) {
    return 0;
  }

  // Рекурсивно вычисляем черные высоты для левого и правого поддеревьев
  int left_height = ComputeBlackHeight(node->left_);
  int right_height = ComputeBlackHeight(node->right_);

  // Если черные высоты поддеревьев не равны или есть красный узел с двумя
  // красными детьми, то возвращаем -1, что сигнализирует об ошибке
  if (left_height == -1 || right_height == -1 || left_height != right_height) {
    return -1;
  }

  // Возвращаем черную высоту текущего узла, добавляя 1, если он черный
  return left_height + (node->color_ == BLACK ? 1 : 0);
}

/**
 * @brief Проверяет корректность раскраски красно-черного дерева, начиная с
 * заданного узла.
 *
 * @param Node Узел, с которого начинается проверка.
 * @return true, если раскраска корректна; false, если есть нарушение.
 */
template <typename KeyType, typename Compare>
bool RedBlackTree<KeyType, Compare>::checkRedNodes(
    const RedBlackTreeNode *Node) const noexcept {
  // Базовый случай: пустое поддерево имеет корректную раскраску
  if (Node == nullptr) {
    return true;
  }

  // Если текущий узел красный, проверяем, чтобы его дети были черными
  if (Node->color_ == RED) {
    if ((Node->left_ != nullptr && Node->left_->color_ == RED) ||
        (Node->right_ != nullptr && Node->right_->color_ == RED)) {
      return false;
    }
  }

  // Рекурсивно проверяем корректность раскраски для левого и правого
  // поддеревьев
  return checkRedNodes(Node->left_) && checkRedNodes(Node->right_);
}

} // namespace s21
