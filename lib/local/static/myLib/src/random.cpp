#include "myLib/Random.hpp"

int myLib::Random::randomInt(const int min, const int max)
{
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

float myLib::Random::randomFloat(const float min, const float max)
{
    return min + (static_cast<float>(randomInt(RANDOM_INT_MIN, RANDOM_INT_MAX)) / RANDOM_FLOAT_MAX * (max - min));
}
