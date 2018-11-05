#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "cube.h"
#include "disjoint_set.h"

void make_union_sets(DisjointSet<std::uint64_t> & disjoint_set, Cube & cube);

int main() {
    using myclock_t = std::chrono::system_clock;
    using duration_t = std::chrono::duration<double>;
    using map_areas_t = std::map<std::uint64_t, std::set<std::uint64_t>>;

    Cube cube{};
    DisjointSet<std::uint64_t> disjoint_set{};

    // Создание структуры из множеств связынных индексов
    std::cout << "Start make and union sets" << std::endl;
    std::chrono::time_point<myclock_t> start = myclock_t::now();

    make_union_sets(disjoint_set, cube);

    double time1 = duration_t(myclock_t::now() - start).count();
    std::cout << "Stop make and union sets" << std::endl;
    std::cout << "Time used: " << time1 << " (sec.)\n" << std::endl;

    // Получение множеств связанных индексов в виде map
    std::cout << "Start get areas" << std::endl;
    start = myclock_t::now();

    map_areas_t areas {disjoint_set.get_areas()};

    double time2 = duration_t(myclock_t::now() - start).count();
    std::cout << "Stop get areas" << std::endl;
    std::cout << "Time used: " << time2 << " (sec.)\n" << std::endl;


    std::cout << "Time used sum(1, 2): " << time1 + time2;
    std::cout << " (sec.)" << std::endl;

    return 0;
}

void make_union_sets(DisjointSet<std::uint64_t> & disjoint_set, Cube & cube) {
    const std::uint64_t nx = cube.get_nx();
    const std::uint64_t ny = cube.get_ny();
    const std::uint64_t nz = cube.get_nz();

    // Заполнение и объединение плоскости XZ при j = 0
    for (std::uint64_t k = 0; k < nz; ++k)
        for (std::uint64_t i = 0; i < nx; ++i) {
            std::uint64_t idx = i + k * nx * ny;
            if (cube.get(idx)) {
                disjoint_set.make_set(idx);

                if (i > 0) {
                    std::uint64_t idx_left = idx - 1;
                    if (disjoint_set.count(idx_left))
                        disjoint_set.union_sets(idx, idx_left);
                }

                if (k > 0) {
                    std::uint64_t idx_backward = idx - nx * ny; 
                    if (disjoint_set.count(idx_backward))
                        disjoint_set.union_sets(idx, idx_backward);
                }
            }
        }

    // Заполнение и объединение плоскости XY при k = 0
    for (std::uint64_t j = 1; j < ny; ++j)
        for (std::uint64_t i = 0; i < nx; ++i) {
            std::uint64_t idx = i + j * nx;
            if (cube.get(idx)) {
                disjoint_set.make_set(idx);

                if (i > 0) {
                    std::uint64_t idx_left = idx - 1; 
                    if (disjoint_set.count(idx_left))
                        disjoint_set.union_sets(idx, idx_left);
                }

                std::uint64_t idx_up = idx - nx;
                if (disjoint_set.count(idx_up))
                    disjoint_set.union_sets(idx, idx_up);
            }
        }

    // Заполнение и объединение плоскости YZ при i = 0
    for (std::uint64_t k = 1; k < nz; ++k)
        for (std::uint64_t j = 1; j < ny; ++j) {
            std::uint64_t idx = j * nx + k * nx * ny;
            if (cube.get(idx)) {
                disjoint_set.make_set(idx);

                std::uint64_t idx_up       = idx - nx;
                std::uint64_t idx_backward = idx - nx * ny;

                if (disjoint_set.count(idx_up))
                    disjoint_set.union_sets(idx, idx_up);

                if (disjoint_set.count(idx_backward))
                    disjoint_set.union_sets(idx, idx_backward);
            }
        }

    // Заполнение и объединение оставшейся части куба
    for (std::uint64_t k = 1; k < nz; ++k)
        for (std::uint64_t j = 1; j < ny; ++j)
            for (std::uint64_t i = 1; i < nx; ++i) {
                std::uint64_t idx = i + j * nx + k * nx * ny;
                if (cube.get(idx)) {
                    disjoint_set.make_set(idx);

                    std::uint64_t idx_left     = idx - 1;
                    std::uint64_t idx_up       = idx - nx;
                    std::uint64_t idx_backward = idx - nx * ny;

                    if (disjoint_set.count(idx_left))
                        disjoint_set.union_sets(idx, idx_left);

                    if (disjoint_set.count(idx_up))
                        disjoint_set.union_sets(idx, idx_up);

                    if (disjoint_set.count(idx_backward))
                        disjoint_set.union_sets(idx, idx_backward);
                }
            }
}