///
/// @file Random.hpp
/// @brief Class for random number generation
/// @namespace myLib
///

#pragma once

#include <random>

namespace myLib {

    static constexpr int RANDOM_INT_MIN = -1000;
    static constexpr int RANDOM_INT_MAX = 1000;
    static constexpr float RANDOM_FLOAT_MAX = 1000.0F;

    ///
    /// @class Random
    /// @brief Class for random number generation
    ///
    class Random {

        public:

            ///
            /// @brief Generate a random integer between min and max
            /// @param min The minimum value
            /// @param max The maximum value
            /// @return int The random integer
            ///
            static int randomInt(int min, int max);
            static int randomInt() { return randomInt(-1000, 1000); };

            ///
            /// @param min The minimum value
            /// @param max The maximum value
            /// @return float The random float
            ///
            static float randomFloat(float min, float max);
            static float randomFloat() { return randomFloat(-1.0F, 1.0F); };

    }; // class Random

} // namespace myLib
