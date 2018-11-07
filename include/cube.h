#ifndef __CUBE__
#define __CUBE__

#include <array>
#include <vector>

#include "engine_rand_bool.h"

/**
    Класс описывает куб, состоящий из ячеек значений 0 или 1.

    Куб описывается вдоль осей X, Y, Z.
    Ячейки нумеруются в порядке X -> Y -> Z.
*/
class Cube {
public:

    ~Cube() = default;                          //!< Деструктор.
    Cube(Cube &&) = default;                    //!< Конструктор перемещения.
    Cube(const Cube &) = default;               //!< Конструктор копирования.
    Cube & operator = (Cube &&) = default;      //!< Оператор перемещения.
    Cube & operator = (const Cube &) = default; //!< Оператор присваивания.

    /**
        Конструктор, создающий куб с рандомными значениями ячеек.

        @param nx Количество ячеек вдоль оси X типа std::uint64_t.
                  По умолчанию равно 400.
        @param ny Количество ячеек вдоль оси Y типа std::uint64_t.
                  По умолчанию равно 250.
        @param nz Количество ячеек вдоль оси Z типа std::uint64_t.
                  По умолчанию равно 100.
    */
    Cube(std::uint64_t nx, std::uint64_t ny, std::uint64_t nz);

    /**
        Возвращает индекс ячейки в кубе.

        В случае невозможной координаты выбрасывает искючение.

        @param i Координата вдоль оси X типа std::uint64_t.
        @param j Координата вдоль оси Y типа std::uint64_t.
        @param k Координата вдоль оси Z типа std::uint64_t.
        @return Индекс ячейки в кубе типа std::uint64_t.
        @throw std::runtime_error
    */
    std::uint64_t get_idx(std::uint64_t i, std::uint64_t j, std::uint64_t k);

    /**
        Возвращает координаты ячейки в кубе.

        В случае невозможного индекса выбрасывает искючение.

        @param idx Индекс ячейки в кубе типа std::uint64_t.
        @return Координаты ячейки в кубе в виде std::array<std::uint64_t, 3>.
        @throw std::runtime_error
    */
    std::array<std::uint64_t, 3> get_ijk(std::uint64_t idx);

    /**
        Возвращает значение ячейки в кубе по координатам.

        В случае невозможного индекса выбрасывает искючение.

        @param i Координата вдоль оси X типа std::uint64_t.
        @param j Координата вдоль оси Y типа std::uint64_t.
        @param k Координата вдоль оси Z типа std::uint64_t.
        @return Значение ячейки типа bool.
        @throw std::runtime_error
    */
    bool get(std::uint64_t x, std::uint64_t y, std::uint64_t z);

    /**
        Возвращает значение индекса ячейки в кубе по индексу.

        В случае невозможного индекса выбрасывает искючение.

        @param idx Индекс ячейки в кубе типа std::uint64_t.
        @return Значение ячейки типа bool.
        @throw std::runtime_error
    */
    bool get(std::uint64_t idx);

    /**
        Возвращает количества ячеек в кубе вдоль оси X.

        @return Количество ячеек в кубе типа std::uint64_t.
    */
    std::uint64_t get_nx();

    /**
        Возвращает количества ячеек в кубе вдоль оси Y.

        @return Количество ячеек в кубе типа std::uint64_t.
    */
    std::uint64_t get_ny();

    /**
        Возвращает количества ячеек в кубе вдоль оси Z.

        @return Количество ячеек в кубе типа std::uint64_t.
    */
    std::uint64_t get_nz();

private:

    /**
        Количество ячеек в кубе вдоль оси X типа std::uint64_t.
    */
    std::uint64_t nx;

    /**
        Количество ячеек в кубе вдоль оси Y типа std::uint64_t.
    */
    std::uint64_t ny;

    /**
        Количество ячеек в кубе вдоль оси Z типа std::uint64_t.
    */
    std::uint64_t nz;

    /**
        Значения ячеек, хранящиеся в std::vector<std::uint8_t>.
    */
    std::vector<std::uint8_t> data;

    /**
        Рандомная инициализация ячеек куба.
    */
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