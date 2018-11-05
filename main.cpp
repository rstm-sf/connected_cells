#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "cube.h"
#include "disjoint_set.h"

void make_sets(DisjointSet<std::uint64_t> & disjoint_set, Cube & cube);
void union_sets(DisjointSet<std::uint64_t> & disjoint_set, Cube & cube);

int main() {
    using myclock_t = std::chrono::system_clock;
    using duration_t = std::chrono::duration<double>;
    using map_areas_t = std::map<std::uint64_t, std::set<std::uint64_t>>;

    Cube cube{};
    DisjointSet<std::uint64_t> disjoint_set{};


    std::cout << "Start make sets" << std::endl;
    std::chrono::time_point<myclock_t> start = myclock_t::now();

    make_sets(disjoint_set, cube);

    double time1 = duration_t(myclock_t::now() - start).count();
    std::cout << "Stop make sets" << std::endl;
    std::cout << "Time used: " << time1 << " (sec.)\n" << std::endl;


    std::cout << "Start union sets" << std::endl;
    start = myclock_t::now();

    union_sets(disjoint_set, cube);

    double time2 = duration_t(myclock_t::now() - start).count();
    std::cout << "Stop union sets" << std::endl;
    std::cout << "Time used: " << time2 << " (sec.)\n" << std::endl;


    std::cout << "Start create areas" << std::endl;
    start = myclock_t::now();

    map_areas_t areas {disjoint_set.get_areas()};

    double time3 = duration_t(myclock_t::now() - start).count();
    std::cout << "Stop union sets" << std::endl;
    std::cout << "Time used: " << time3 << " (sec.)\n" << std::endl;


    std::cout << "Time used sum(1, 2, 3): " << time1 + time2 + time3;
    std::cout << " (sec.)" << std::endl;
    return 0;
}

void make_sets(DisjointSet<std::uint64_t> & disjoint_set, Cube & cube) {
    const std::uint64_t size = cube.get_nx() * cube.get_ny() * cube.get_nz();
    // Сохранение индексов ячеек со значением равным 1
    for (std::uint64_t idx = 0; idx < size; ++idx)
        if (cube.get(idx))
            disjoint_set.make_set(idx);
}

void union_sets(DisjointSet<std::uint64_t> & disjoint_set, Cube & cube) {
    const std::uint64_t nx = cube.get_nx();
    const std::uint64_t ny = cube.get_ny();
    const std::uint64_t nz = cube.get_nz();
    std::uint64_t idx{0};
    for (std::uint64_t k = 0; k < nz; ++k)
        for (std::uint64_t j = 0; j < ny; ++j)
            for (std::uint64_t i = 0; i < nx; ++i) {
                
                if (disjoint_set.count(idx)) {
                    std::uint64_t idx_right   = idx + 1;
                    std::uint64_t idx_down    = idx + nx;
                    std::uint64_t idx_forward = idx + nx * ny;

                    if (disjoint_set.count(idx_right)   && i < nx - 1)
                        disjoint_set.union_sets(idx, idx_right);

                    if (disjoint_set.count(idx_down)    && j < ny - 1)
                        disjoint_set.union_sets(idx, idx_down);

                    if (disjoint_set.count(idx_forward) && k < nz - 1)
                        disjoint_set.union_sets(idx, idx_forward);
                }
                ++idx;
            }
}