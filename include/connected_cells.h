#ifndef __CONNECTED_CELLS__
#define __CONNECTED_CELLS__

#include <algorithm>
#include <vector>

#include "cube.h"

class ConnectedCells {
public:
    ConnectedCells() = delete;                                      //!< Конструктор по умолчанию.
    ~ConnectedCells() = default;                                    //!< Деструктор.
    ConnectedCells(ConnectedCells &&) = default;                    //!< Конструктор перемещения.
    ConnectedCells(const ConnectedCells &) = default;               //!< Конструктор копирования.
    ConnectedCells & operator = (ConnectedCells &&) = default;      //!< Оператор перемещения.
    ConnectedCells & operator = (const ConnectedCells &) = default; //!< Оператор присваивания.

    ConnectedCells(const Cube & cube);

    const std::vector<std::uint64_t> & get_set(std::uint64_t idx) const;

    std::uint64_t size();
    
private:
    std::vector<std::vector<std::uint64_t>> sets;

    std::vector<std::int8_t> used;
    Cube cube;

    void dfs(std::uint64_t i, std::uint64_t j, std::uint64_t k);
};

ConnectedCells::ConnectedCells(const Cube & cube)
    : sets{}, cube{cube} {
    const std::uint64_t nx = this->cube.get_nx();
    const std::uint64_t ny = this->cube.get_ny();
    const std::uint64_t nz = this->cube.get_nz();
    used = std::vector<std::int8_t>(nx * ny * nz, false);

    for (std::uint64_t k = 0; k < nz; ++k)
        for (std::uint64_t j = 0; j < ny; ++j)
            for (std::uint64_t i = 0; i < nx; ++i) {
                std::uint64_t idx = i + j * nx + k * nx * ny;
                if (!used[idx]) {
                    if (this->cube.get(idx) == 0) {
                        used[idx] = true;
                        continue;
                    }
                    sets.push_back(std::vector<std::uint64_t>{});
                    dfs(i, j, k);
                }
            }

    used.clear();
    for (auto & set : sets)
        std::sort(set.begin(), set.end());
}

const std::vector<std::uint64_t> & ConnectedCells::get_set(
    std::uint64_t idx) const {
    return sets.at(idx);
}

std::uint64_t ConnectedCells::size() { return sets.size(); }

void ConnectedCells::dfs(std::uint64_t i, std::uint64_t j, std::uint64_t k) {
    const std::uint64_t nx = cube.get_nx();
    const std::uint64_t ny = cube.get_ny();
    const std::uint64_t idx = i + j * nx + k * nx * ny;
    used[idx] = true;

    if (cube.get(idx) == 0)
        return;
    sets[sets.size() - 1].push_back(idx);

    if (k < cube.get_nz() - 1 && !used[idx + nx * ny]) dfs(  i,   j, k+1);
    if (j < ny - 1            && !used[idx + nx])      dfs(  i, j+1,   k);
    if (i < nx - 1            && !used[idx + 1])       dfs(i+1,   j,   k);
    if (k > 0                 && !used[idx - nx * ny]) dfs(  i,   j, k-1);
    if (j > 0                 && !used[idx - nx])      dfs(  i, j-1,   k);
    if (i > 0                 && !used[idx - 1])       dfs(i-1,   j,   k);
}

#endif // __CONNECTED_CELLS__