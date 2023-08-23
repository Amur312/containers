

#include <stdexcept>

#include "../tree/RedBlackTree.h"

namespace s21 {
/**
 * @brief Конструктор по умолчанию. Инициализирует пустое дерево.
 *
 * Создает фиктивный узел head_ и связанные с ним указатели,
 * чтобы обеспечить пустое дерево.
 */
template <typename Key, typename Type>
map<Key, Type>::map() : tree_(new tree_type{}) {}
/**
 * @brief Конструктор инициализации на основе списка значений.
 *
 * Инициализирует дерево с помощью переданного списка значений.
 * Для каждого элемента в списке вызывается метод insert() для добавления его в
 * дерево.
 *
 * @param items Список значений для инициализации дерева.
 */
template <typename Key, typename Type>
map<Key, Type>::map(std::initializer_list<value_type> const &items) : map() {
  for (const auto &item : items) {
    insert(item);
  }
}

/**
 * @brief Конструктор копирования. Создает копию дерева из другого объекта map.
 *
 * Создает глубокую копию дерева, на которое указывает другой объект map.
 *
 * @param otherMap Другой объект map, из которого будет скопировано дерево.
 */
template <typename Key, typename Type>
map<Key, Type>::map(const map &otherMap)
    : tree_(new tree_type(*otherMap.tree_)) {}

/**
 * @brief Конструктор перемещения. Перемещает дерево из другого объекта map.
 *
 * Перемещает дерево из другого объекта map, используя конструктор перемещения
 * для внутреннего дерева.
 *
 * @param otherMap Другой объект map, из которого будет перемещено дерево.
 */
template <typename Key, typename Type>
map<Key, Type>::map(map &&otherMap) noexcept
    : tree_(new tree_type(std::move(*otherMap.tree_))) {}

/**
 * @brief Присваивание содержимого другой карты текущей карте.
 *
 * Создает копию структуры дерева и данных из другой карты,
 * заменяя текущее содержимое этой карты на содержимое другой.
 *
 * @param otherMap Карта, содержимое которой будет скопировано.
 * @return Ссылка на текущую карту после присваивания.
 */
template <typename Key, typename Type>
map<Key, Type> &map<Key, Type>::operator=(const map &otherMap) {
  if (this != &otherMap) {
    auto *copiedTree = new tree_type(*otherMap.tree_);
    std::swap(tree_, copiedTree);
    delete copiedTree;
  }
  return *this;
}

/**
 * @brief Перемещающее присваивание содержимого другой карты текущей карте.
 *
 * Перемещает структуру дерева и данных из другой карты в текущую карту,
 * заменяя текущее содержимое этой карты на содержимое другой с минимальными
 * затратами.
 *
 * @param otherMap Карта, содержимое которой будет перемещено.
 * @return Ссылка на текущую карту после перемещающего присваивания.
 */
template <typename Key, typename Type>
map<Key, Type> &map<Key, Type>::operator=(map &&otherMap) noexcept {
  if (this != &otherMap) {
    std::swap(tree_, otherMap.tree_);
  }
  return *this;
}

/**
 * @brief Проверка на равенство двух карт.
 *
 * Сравнивает текущую карту с другой картой по их содержимому.
 * Карты считаются равными, если их размеры и содержимое идентичны.
 *
 * @param otherMap Другая карта, с которой производится сравнение.
 * @return true, если карты равны, иначе false.
 */
template <typename Key, typename Type>
inline bool map<Key, Type>::operator==(const map &otherMap) const {
  if (this == &otherMap)
    return true;
  if (size() != otherMap.size())
    return false;

  auto it1 = begin();
  auto it2 = otherMap.begin();
  for (; it1 != end(); ++it1, ++it2) {
    if (*it1 != *it2)
      return false;
  }
  return true;
}

/**
 * @brief Проверяет, не равна ли текущая карта другой карте.
 *
 * Сравнивает текущую карту с другой картой на неравенство.
 *
 * @param other Другая карта, с которой выполняется сравнение.
 * @return `true`, если карты не равны, иначе `false`.
 */
template <typename Key, typename Type>
bool map<Key, Type>::operator!=(const map &otherMap) const {
  return !(*this == otherMap);
}

/**
 * @brief Деструктор класса карты.
 *
 * Освобождает память, занимаемую деревом карты.
 * Этот метод вызывается при уничтожении объекта карты.
 */
template <typename Key, typename Type> map<Key, Type>::~map() { delete tree_; }

/**
 * @brief Получение значения элемента по ключу с проверкой на наличие.
 *
 * Возвращает ссылку на значение элемента, связанного с заданным ключом.
 * Если ключ отсутствует в карте, генерируется исключение std::out_of_range.
 *
 * @param key Ключ элемента, значение которого необходимо получить.
 * @return Ссылка на значение элемента.
 * @throws std::out_of_range Если ключ отсутствует в карте.
 */
template <typename Key, typename Type>
typename map<Key, Type>::mapped_type &map<Key, Type>::at(const key_type &key) {
  iterator searchIterator = tree_->Find(value_type(key, mapped_type{}));

  if (searchIterator == end()) {
    throw std::out_of_range(
        "s21::map::at: Элемент с указанным ключом отсутствует.");
  }

  return (*searchIterator).second;
}

/**
 * @brief Получение значения элемента по ключу с проверкой на наличие
 * (константная версия).
 *
 * Возвращает ссылку на константное значение элемента, связанного с заданным
 * ключом. Если ключ отсутствует в карте, генерируется исключение
 * std::out_of_range.
 *
 * @param key Ключ элемента, значение которого необходимо получить.
 * @return Ссылка на константное значение элемента.
 * @throws std::out_of_range Если ключ отсутствует в карте.
 */
template <typename Key, typename Type>
const typename map<Key, Type>::mapped_type &
map<Key, Type>::at(const key_type &key) const {
  const_iterator searchIterator = tree_->Find(value_type(key, mapped_type{}));

  if (searchIterator == end()) {
    throw std::out_of_range(
        "s21::map::at: Элемент с указанным ключом отсутствует.");
  }

  return (*searchIterator).second;
}

/**
 * @brief Перегруженный оператор индексации для доступа к элементу по ключу.
 *
 * Этот оператор позволяет получить доступ к значению элемента по указанному
 * ключу. Если элемент с указанным ключом уже существует в карте, возвращается
 * ссылка на его значение. В противном случае, элемент с заданным ключом
 * добавляется в карту, и возвращается ссылка на его значение.
 *
 * @param key Ключ элемента, значение которого необходимо получить или добавить.
 * @return Ссылка на значение элемента.
 */
template <typename Key, typename Type>
typename map<Key, Type>::mapped_type &
map<Key, Type>::operator[](const key_type &key) {
  // Создание временного значения для поиска элемента по ключу
  value_type temp_value_to_search(key, mapped_type{});

  // Поиск элемента в карте по временному значению
  iterator search_iterator = tree_->Find(temp_value_to_search);

  // Если элемент существует, возвращаем ссылку на его значение
  if (search_iterator != end()) {
    return (*search_iterator).second;
  }

  // Вставляем новый элемент в карту и получаем результат операции
  std::pair<iterator, bool> insertion_result =
      tree_->InsertUnique(temp_value_to_search);

  // Возвращаем ссылку на значение нового элемента
  return (*insertion_result.first).second;
}
/**
 * @brief Возвращает итератор, указывающий на начало контейнера.
 *
 * Возвращает итератор, указывающий на первый элемент в контейнере.
 *
 * @return Итератор, указывающий на начало контейнера.
 */
template <typename Key, typename Type>
typename map<Key, Type>::iterator map<Key, Type>::begin() noexcept {
  return tree_->Begin();
}

/**
 * @brief Возвращает константный итератор, указывающий на начало контейнера.
 *
 * Возвращает константный итератор, указывающий на первый элемент в контейнере.
 *
 * @return Константный итератор, указывающий на начало контейнера.
 */
template <typename Key, typename Type>
typename map<Key, Type>::const_iterator map<Key, Type>::begin() const noexcept {
  return tree_->Begin();
}

/**
 * @brief Возвращает итератор, указывающий на конец контейнера.
 *
 * Возвращает итератор, указывающий на элемент, следующий за последним в
 * контейнере.
 *
 * @return Итератор, указывающий на конец контейнера.
 */
template <typename Key, typename Type>
typename map<Key, Type>::iterator map<Key, Type>::end() noexcept {
  return tree_->End();
}

/**
 * @brief Возвращает константный итератор, указывающий на конец контейнера.
 *
 * Возвращает константный итератор, указывающий на элемент, следующий за
 * последним в контейнере.
 *
 * @return Константный итератор, указывающий на конец контейнера.
 */
template <typename Key, typename Type>
typename map<Key, Type>::const_iterator map<Key, Type>::end() const noexcept {
  return tree_->End();
}

/**
 * @brief Проверяет, пуст ли контейнер.
 *
 * Проверяет, содержит ли контейнер элементы или является пустым.
 *
 * @return true, если контейнер пуст, иначе false.
 */
template <typename Key, typename Type> bool map<Key, Type>::empty() const noexcept {
  return tree_->Empty();
}

/**
 * @brief Возвращает количество элементов в контейнере.
 *
 * Возвращает количество элементов, содержащихся в контейнере.
 *
 * @return Количество элементов в контейнере.
 */
template <typename Key, typename Type>
typename map<Key, Type>::size_type map<Key, Type>::size() const noexcept {
  return tree_->Size();
}

/**
 * @brief Возвращает максимальное возможное количество элементов в контейнере.
 *
 * Возвращает максимальное возможное количество элементов, которое может
 * содержаться в контейнере на текущей платформе.
 *
 * @return Максимальное количество элементов в контейнере.
 */
template <typename Key, typename Type>
typename map<Key, Type>::size_type map<Key, Type>::max_size() const noexcept {
  return tree_->MaxSize();
}

/**
 * @brief Удаляет все элементы из контейнера.
 *
 * Удаляет все элементы, содержащиеся в контейнере, оставляя его пустым.
 */
template <typename Key, typename Type> void map<Key, Type>::clear() noexcept {
  tree_->Clear();
}
/**
 * @brief Вставляет элемент в карту, если его ключ отсутствует.
 *
 * Пытается вставить заданный элемент в карту. Если элемент с таким ключом
 * уже существует, операция вставки не производится и возвращается итератор на
 * существующий элемент и значение false. В противном случае, элемент
 * добавляется в карту, и возвращается итератор на новый элемент и значение
 * true.
 *
 * @param element_to_insert Значение элемента, которое необходимо вставить.
 * @return Пара, содержащая итератор на элемент и флаг успешности вставки.
 */
template <typename Key, typename Type>
std::pair<typename map<Key, Type>::iterator, bool>
map<Key, Type>::insert(const value_type &element_to_insert) {
  return tree_->InsertUnique(element_to_insert);
}

/**
 * @brief Вставляет элемент в карту, если его ключ отсутствует.
 *
 * Пытается вставить элемент с заданным ключом и значением в карту.
 * Если элемент с таким ключом уже существует, операция вставки не производится
 * и возвращается итератор на существующий элемент и значение false.
 * В противном случае, элемент добавляется в карту, и возвращается итератор
 * на новый элемент и значение true.
 *
 * @param key Ключ элемента, который необходимо вставить.
 * @param value Значение элемента, которое необходимо вставить.
 * @return Пара, содержащая итератор на элемент и флаг успешности вставки.
 */
template <typename Key, typename Type>
std::pair<typename map<Key, Type>::iterator, bool>
map<Key, Type>::insert(const key_type &key, const mapped_type &value) {
  return tree_->InsertUnique(value_type{key, value});
}
/**
 * @brief Вставляет элемент или обновляет значение элемента в карте.
 *
 * Пытается вставить элемент с заданным ключом и значением в карту.
 * Если элемент с таким ключом уже существует, его значение обновляется
 * заданным значением. Возвращается итератор на элемент и флаг, указывающий
 * на успешность операции.
 *
 * @param key Ключ элемента, который необходимо вставить или обновить.
 * @param value Значение элемента, которое необходимо вставить или установить.
 * @return Пара, содержащая итератор на элемент и флаг успешности операции.
 */
template <typename Key, typename Type>
std::pair<typename map<Key, Type>::iterator, bool>
map<Key, Type>::insert_or_assign(const key_type &key,
                                 const mapped_type &value) {
  auto [it, inserted] = tree_->InsertUnique(value_type{key, value});

  if (!inserted) {
    (*it).second = value;
  }

  return {it, inserted};
}

/**
 * @brief Удаляет элемент по указанному итератору из карты.
 *
 * Удаляет элемент, на который указывает переданный итератор.
 *
 * @param pos Итератор, указывающий на элемент, который необходимо удалить.
 */
template <typename Key, typename Type>
void map<Key, Type>::erase(iterator pos) noexcept {
  tree_->Erase(pos);
}

/**
 * @brief Обменивает содержимое двух карт.
 *
 * Обменивает содержимое текущей карты с содержимым другой карты.
 *
 * @param other Карта, с которой необходимо обменять содержимое.
 */
template <typename Key, typename Type>
void map<Key, Type>::swap(map &other) noexcept {
  tree_->Swap(*other.tree_);
}

/**
 * @brief Объединяет две карты, удаляя дубликаты.
 *
 * Объединяет текущую карту с другой картой, удаляя дубликаты ключей.
 * Значения с одинаковыми ключами будут игнорироваться.
 *
 * @param other Карта, с которой необходимо объединить текущую карту.
 */
template <typename Key, typename Type>
void map<Key, Type>::merge(map &other) noexcept {
  tree_->MergeUnique(*other.tree_);
}
/**
 * @brief Проверяет наличие элемента по ключу в карте.
 *
 * Этот метод позволяет проверить, существует ли элемент в карте с заданным
 * ключом. Он создает временную пару ключ-значение и использует внутренний метод
 * поиска дерева, чтобы определить, существует ли такой элемент в карте.
 *
 * @param key Ключ, который нужно проверить на наличие в карте.
 * @return true, если элемент с данным ключом существует, иначе false.
 */
template <typename Key, typename Type>
bool map<Key, Type>::contains(const key_type &key) const noexcept {
  // Создаем временную пару ключ-значение с заданным ключом и пустым значением
  value_type keyValuePair(key, mapped_type{});
  // Используем метод поиска дерева, чтобы найти пару в карте
  // Если пара найдена, значит, элемент с данным ключом существует
  // Возвращаем результат сравнения с итератором, указывающим за конец
  // контейнера
  return tree_->Find(keyValuePair) != end();
}
/**
 * @brief Вставляет элемент в карту, используя перемещение аргументов.
 *
 * Этот метод позволяет вставить элемент в карту, используя переданные аргументы
 * для создания пары ключ-значение. Если элемент с таким ключом уже существует в
 * карте, вставка не происходит, и возвращается итератор, указывающий на
 * существующий элемент. Если вставка успешно выполнена, возвращается итератор
 * на вновь вставленный элемент и флаг, указывающий на успешность вставки.
 *
 * @tparam Args Типы аргументов, необходимых для создания нового элемента
 * (ключ-значение).
 * @param args Аргументы, используемые для создания нового элемента.
 * @return Пара, содержащая итератор на вставленный элемент и флаг успешности
 * вставки.
 */
template <typename Key, typename Type>
template <typename... Args>
std::pair<typename map<Key, Type>::iterator, bool>
map<Key, Type>::emplace(Args &&...args) {
  // Создаем новый элемент (ключ-значение) из переданных аргументов
  value_type newEntry{std::forward<Args>(args)...};

  // Ищем существующий элемент с таким же ключом
  iterator existingEntry = find(newEntry.first);

  if (existingEntry == end()) {
    // Если элемент с данным ключом не найден, выполняем вставку
    auto insertResult = tree_->InsertUnique(newEntry);
    return std::make_pair(insertResult.first, insertResult.second);
  } else {
    // Если элемент с данным ключом уже существует, возвращаем итератор на него
    // и флаг false
    return std::make_pair(existingEntry, false);
  }
}

/**
 * @brief Вставляет несколько элементов из диапазона в карту.
 *
 * Этот метод позволяет вставить несколько элементов из заданного диапазона
 * в текущую карту. Для каждого элемента в диапазоне вызывается метод вставки
 * `insert`.
 *
 * @tparam InputIt Тип итератора для диапазона элементов.
 * @param first Итератор, указывающий на начало диапазона элементов для вставки.
 * @param last Итератор, указывающий на конец диапазона элементов для вставки.
 */
template <typename Key, typename Type>
template <typename InputIt>
void map<Key, Type>::insert_many(InputIt first, InputIt last) {
  // Проходим по диапазону элементов, вызывая метод вставки для каждого элемента
  for (; first != last; ++first) {
    insert(*first);
  }
}
/**
 * @brief Находит элемент по ключу в карте.
 *
 * Этот метод позволяет найти элемент в карте по заданному ключу.
 * Создается временная пара ключ-значение с заданным ключом и пустым значением,
 * которая используется для выполнения поиска через внутренний метод дерева.
 *
 * @param key Ключ, по которому нужно найти элемент.
 * @return Итератор на найденный элемент, либо итератор, указывающий за конец,
 * если элемент не найден.
 */
template <typename Key, typename Type>
typename map<Key, Type>::iterator
map<Key, Type>::find(const key_type &key) noexcept {
  // Создаем временную пару ключ-значение с заданным ключом и пустым значением
  value_type temporary_entry(key, mapped_type{});

  // Используем метод поиска дерева для выполнения поиска элемента
  return tree_->Find(temporary_entry);
}

/**
 * @brief Находит константный элемент по ключу в карте.
 *
 * Этот метод позволяет найти константный элемент в карте по заданному ключу.
 * Создается временная пара ключ-значение с заданным ключом и пустым значением,
 * которая используется для выполнения поиска через внутренний метод дерева.
 *
 * @param key Ключ, по которому нужно найти константный элемент.
 * @return Константный итератор на найденный элемент, либо константный итератор,
 *         указывающий за конец, если элемент не найден.
 */
template <typename Key, typename Type>
typename map<Key, Type>::const_iterator
map<Key, Type>::find(const key_type &key) const noexcept {
  // Создаем временную пару ключ-значение с заданным ключом и пустым значением
  value_type temporary_entry(key, mapped_type{});

  // Используем метод поиска дерева для выполнения поиска константного элемента
  return tree_->Find(temporary_entry);
}

/**
 * @brief Подсчитывает количество элементов с заданным ключом в карте.
 *
 * Этот метод позволяет подсчитать количество элементов в карте с заданным
 * ключом. Он использует метод поиска для определения наличия элемента с
 * заданным ключом и возвращает 1, если элемент найден, и 0 в противном случае.
 *
 * @param key Ключ, для которого нужно подсчитать количество элементов.
 * @return Количество элементов с заданным ключом (0 или 1).
 */
template <typename Key, typename Type>
typename map<Key, Type>::size_type
map<Key, Type>::count(const key_type &key) const noexcept {
  // Используем метод поиска для определения наличия элемента с заданным ключом
  // Возвращаем 1, если элемент найден, и 0 в противном случае
  return find(key) != end() ? 1 : 0;
}

} // namespace s21
