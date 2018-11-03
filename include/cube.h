#ifndef __CUBE__
#define __CUBE__

#include <array>
#include <vector>

#include "engine_rand_bool.h"

class Cube {
public:
    ~Cube() = default;
    Cube(Cube &&) = default;
    Cube(const Cube &) = default;
    Cube & operator = (Cube &&) = default;
    Cube & operator = (const Cube &) = default;

    Cube(std::uint64_t nx, std::uint64_t ny, std::uint64_t nz);

    std::uint64_t get_idx(std::uint64_t i, std::uint64_t j, std::uint64_t k);

    std::array<std::uint64_t, 3> get_ijk(std::uint64_t idx);

    bool get(std::uint64_t x, std::uint64_t y, std::uint64_t z);
    bool get(std::uint64_t idx);

    std::uint64_t get_nx();
    std::uint64_t get_ny();
    std::uint64_t get_nz();

private:
    std::uint64_t nx;
    std::uint64_t ny;
    std::uint64_t nz;
    std::vector<std::uint8_t> data;

    void random_init_data();
};

Cube::Cube(
    std::uint64_t nx = std::uint64_t(400),
    std::uint64_t ny = std::uint64_t(250),
    std::uint64_t nz = std::uint64_t(100)
) : nx{nx}, ny{ny}, nz{nz} { random_init_data(); }

std::uint64_t Cube::get_idx(std::uint64_t i, std::uint64_t j, std::uint64_t k) {
    if (i >= nx) throw std::runtime_error{"illegal nx index"};
    if (j >= ny) throw std::runtime_error{"illegal ny index"};
    if (k >= nz) throw std::runtime_error{"illegal nz index"};
    return i + j * nx + k * nx * ny;
}

std::array<std::uint64_t, 3> Cube::get_ijk(std::uint64_t idx) {
    if (idx >= data.capacity())
        throw std::runtime_error{"illegal size index"};

    const std::uint64_t k {idx / (nx * ny)};
    idx -= k * nx * ny;
    const std::uint64_t j {idx / nx};
    const std::uint64_t i {idx - j * nx};

    return std::array<std::uint64_t, 3> { {i, j, k} };
}

bool Cube::get(std::uint64_t i, std::uint64_t j, std::uint64_t k) {
    return bool(data[get_idx(i, j, k)]);
}

bool Cube::get(std::uint64_t idx) {
    if (idx >= data.capacity())
        throw std::runtime_error{"illegal size index"};
    return bool(data[idx]);
}

std::uint64_t Cube::get_nx() {
    return nx;
}

std::uint64_t Cube::get_ny() {
    return ny;
}

std::uint64_t Cube::get_nz() {
    return nz;
}

void Cube::random_init_data() {
    const std::uint64_t capacity = nx * ny * nz;
    data.reserve(capacity);
    EngineRandBool random(5);
    for (std::uint64_t i = 0; i < capacity; ++i)
        data.push_back(uint8_t(random.rand()));
}

#endif