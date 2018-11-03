#ifndef __ENGINE_RANDOM___
#define __ENGINE_RANDOM___

#include <random>

class EngineRandBool {
public:
    ~EngineRandBool() = default;
    EngineRandBool(EngineRandBool &&) = delete;
    EngineRandBool(const EngineRandBool &) = delete;
    EngineRandBool & operator = (EngineRandBool &&) = delete;
    EngineRandBool & operator = (const EngineRandBool &) = delete;

    EngineRandBool(std::uint32_t seed);

    bool rand();

private:
    std::mt19937 mt;
    std::uniform_int_distribution<std::uint8_t> dist;
};

EngineRandBool::EngineRandBool(std::uint32_t seed = 1) : mt{seed}, dist{0, 1} {}

bool EngineRandBool::rand() { 
    return bool(dist(mt));
}

#endif // __ENGINE_RANDOM___