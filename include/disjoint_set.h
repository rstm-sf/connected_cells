#ifndef __DISJOINT_SET__
#define __DISJOINT_SET__

#include <set>
#include <map>
#include <utility>

#include "engine_rand_bool.h"

/**
    Шаблонный класс, описывающий систему непересекающихся множеств, состоящих
    из элементов множества целых неотрицательных чисел.

    Шаблон зависит от типа <T> переменных в множестве.
    Структрура данных описывается в виде леса.
    Используется Map:вершина->предок для хранения непосредственного
    предка вершины (лидера множества).
    Объединение двух множеств происходит случайным образом.
*/
template <class T>
class DisjointSet {
public:

    DisjointSet<T>() = default;                                     //!< Конструктор по умолчанию.
    ~DisjointSet<T>() = default;                                    //!< Деструктор.
    DisjointSet<T>(DisjointSet<T> &&) = default;                    //!< Конструктор перемещения.
    DisjointSet<T>(const DisjointSet<T> &) = default;               //!< Конструктор копирования.
    DisjointSet<T> & operator = (DisjointSet<T> &&) = default;      //!< Оператор перемещения.
    DisjointSet<T> & operator = (const DisjointSet<T> &) = default; //!< Оператор присваивания.

    /**
        Создает новое множество из данного элемента.

        @param a Элемент множества типа <T>.
    */
    void make_set(T a);

    /**
        Возвращает лидера множества, в котором находится данный элемент.

        @param a Элемент множества типа <T>.
        @return Лидер множества типа <T>.
    */
    T find_set(T a);

    /**
        Возвращает или лидера множества, в котором находится данный элемент,
        или значение -1, в случае если такого элемента нет ни в одном множестве.

        @param a Элемент множества типа <T>.
        @return Значение типа <T>.
    */
    T find_set_s(T a);

    /**
        Объединяет два множества, в которых находятся данные элементы.

        @param a Элемент множества типа <T>.
        @param b Элемент множества типа <T>.
    */
    void union_sets(T a, T b);

    /**
        Объединяет два множества, в которых находятся данные элементы.
        
        В данном методе происходит прежде проверка вхождения элемента.

        @param a Элемент множества типа <T>.
        @param b Элемент множества типа <T>.
    */
    void union_sets_s(T a, T b);

    /**
        Возвращает количество множеств, которым принадлежит данный элемент.

        Если элемент не принадлежит никакому множеству, то 0, иначе 1.

        @param a Элемент множества типа <T>.
        @return Значение типа std::size_t.
    */
    std::size_t count(T a);

    /**
        Возвращает все элементы данной системы.

        @return Элементы системы, хранящиейся в std::vector<T>.
    */
    std::vector<T> get_elements();

    /**
        Возвращает всех лидеров данной системы.

        @return Лидеры системы, хранящиейся в std::set<T>.
    */
    std::set<T> get_leaders();

    /**
        Возвращает все пронумерованные множества данной системы.

        @return Map:номер->(упорядоченное множество элементов)
                типа std::map<T, std::set<T>>.
    */
    std::map<T, std::set<T>> get_sets();

private:

    std::map<T, T> parent; /*!< Map:вершина->предок */
    EngineRandBool rank;   /*!< Движок для рандомного булевого числа */
};

template <class T>
void DisjointSet<T>::make_set(T a) {
    parent.insert(std::make_pair(a, a));
}

template <class T>
T DisjointSet<T>::find_set(T a) {
    if (a == parent[a]) {
        return a;
    } else {
        parent[a] = find_set(parent[a]);
        return parent[a];
    }
}

template <class T>
T DisjointSet<T>::find_set_s(T a) {
    if (parent.count(a)) {
        return find_set(a);
    } else {
        return -1;
    }
}

template <class T>
void DisjointSet<T>::union_sets(T a, T b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (rank.rand())
            std::swap(a, b);
        parent[b] = a;
    }
}

template <class T>
void DisjointSet<T>::union_sets_s(T a, T b) {
    a = find_set_s(a);
    b = find_set_s(b);
    if (a == -1 || b == -1) {
        return;
    } else if (a != b) {
        if (rank.rand())
            std::swap(a, b);
        parent[b] = a;
    }
}

template <class T>
std::size_t DisjointSet<T>::count(T a) { return parent.count(a); }

template <class T>
std::vector<T> DisjointSet<T>::get_elements() {
    std::vector<T> keys(parent.size());
    for (auto const & kv : parent)
        keys.push_back(kv.first);
    return keys;
}

template <class T>
std::set<T> DisjointSet<T>::get_leaders() {
    std::set<T> leaders{};
    for (auto const & kv : parent)
        leaders.insert(find_set(kv.first));
    return leaders;
}

template <class T>
std::map<T, std::set<T>> DisjointSet<T>::get_sets() {
    std::map<T, std::set<T>> areas;
    std::map<T, T> rename;
    T i{1};
    for (auto const & kv : parent) {
        T leader = find_set(kv.first);
        if (rename.count(leader)) {
            areas[rename[leader]].insert(kv.first);
        } else {
            rename.insert(std::make_pair(leader, i));
            areas.insert(std::make_pair(i, std::set<T> {kv.first}));
            ++i;
        } 
    }
    return areas;
}

#endif // __DISJOINT_SET__