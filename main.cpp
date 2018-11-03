#include <iostream>

#include <chrono>
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
    const std::uint64_t size = nx * ny * nz;
    std::vector<std::int8_t> used(size, false);

    for (std::uint64_t k = 0; k < nz; ++k)
        for (std::uint64_t j = 0; j < ny; ++j)
            for (std::uint64_t i = 0; i < nx; ++i) {
                std::uint64_t idx = i + j * nx + k * nx * ny;

                if (!used[idx] && disjoint_set.count(idx)) {
                    std::uint64_t idx_right    = idx + 1;
                    std::uint64_t idx_left     = idx - 1;
                    std::uint64_t idx_up       = idx - nx;
                    std::uint64_t idx_down     = idx + nx;
                    std::uint64_t idx_forward  = idx + nx * ny;
                    std::uint64_t idx_backward = idx - nx * ny;

                    if (i < nx - 1 && !used[idx_right]) {
                        if (disjoint_set.count(idx_right)) {
                            disjoint_set.union_sets(idx, idx_right);
                        } else {
                            used[idx_right] = true;
                        }
                    }

                    if (i > 0      && !used[idx_left]) {
                        if (disjoint_set.count(idx_left)) {
                            disjoint_set.union_sets(idx, idx_left);
                        } else {
                            used[idx_right] = true;
                        }
                    }

                    if (j < ny - 1 && !used[idx_down]) {
                        if (disjoint_set.count(idx_down)) {
                            disjoint_set.union_sets(idx, idx_down);
                        } else {
                            used[idx_right] = true;
                        }
                    }

                    if (j > 0      && !used[idx_up]) {
                        if (disjoint_set.count(idx_up)) {
                            disjoint_set.union_sets(idx, idx_up);
                        } else {
                            used[idx_right] = true;
                        }
                    }

                    if (k < nz - 1 && used[idx_forward]) {
                        if (disjoint_set.count(idx_forward)) {
                            disjoint_set.union_sets(idx, idx_forward);
                        } else {
                            used[idx_right] = true;
                        }
                    }

                    if (k > 0      && !used[idx_backward]) {
                        if (disjoint_set.count(idx_backward)) {
                            disjoint_set.union_sets(idx, idx_backward);
                        } else {
                            used[idx_right] = true;
                        }
                    }
                }
                used[idx] = true;
            }
}