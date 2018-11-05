#ifndef __DISJOINT_SET__
#define __DISJOINT_SET__

#include <set>
#include <map>
#include <utility>

#include "engine_rand_bool.h"

template <class T>
class DisjointSet {
public:
    DisjointSet<T>() = default;
    ~DisjointSet<T>() = default;
    DisjointSet<T>(DisjointSet<T> &&) = default;
    DisjointSet<T>(const DisjointSet<T> &) = default;
    DisjointSet<T> & operator = (DisjointSet<T> &&) = default;
    DisjointSet<T> & operator = (const DisjointSet<T> &) = default;

    void make_set(T a);
    T find_set(T a);
    T find_set_s(T a);
    void union_sets(T a, T b);
    void union_sets_s(T a, T b);

    std::size_t count(T a);
    std::vector<T> get_keys();
    std::set<T> get_leaders();
    std::map<T, std::set<T>> get_areas();

private:
    std::map<T, T> parent;
    EngineRandBool rank;
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
std::vector<T> DisjointSet<T>::get_keys() {
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
std::map<T, std::set<T>> DisjointSet<T>::get_areas() {
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