
#include "../src/list/s21_list.h"



int main() {
    // Создаем список и заполняем его данными
    s21::list<int> testList;
    for (int i = 0; i < 10; ++i) {
        testList.push_back(i);
    }

    // Создаем итератор и пробуем обойти список
    s21::list<int>::iterator it = testList.begin();
    while (it != testList.end()) {
        std::cout << *it << " ";
        ++it;
    }
    std::cout << std::endl;

    // Тестируем операторы + и -
    it = testList.begin();
    it = it + 5;
    std::cout << "Element at position 5: " << *it << std::endl; // Должно быть 5

    it = it - 3;
    std::cout << "Element at position 2: " << *it << std::endl; // Должно быть 2

    // Тестируем константный итератор
    s21::list<int>::const_iterator const_it = testList.cbegin();
    while (const_it != testList.cend()) {
        std::cout << *const_it << " ";
        ++const_it;
    }
    std::cout << std::endl;

    const_it = testList.cbegin();
    const_it = const_it + 7;
    std::cout << "Element at position 7: " << *const_it << std::endl; // Должно быть 7

    const_it = const_it - 5;
    std::cout << "Element at position 2: " << *const_it << std::endl; // Должно быть 2

    return 0;
}