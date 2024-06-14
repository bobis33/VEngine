/*
** @file Clock.hpp
** @brief Clock class for time management
** @namespace myLib
*/

#pragma once

#include <chrono>

#include "myLib/Clock/Time.hpp"

/*
** @brief TimePoint is a type alias for a time point which is a very long and complicated type in the standard library
*/
using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

namespace myLib {

    /*
    ** @brief Class for time management
    */
    class Clock {

        public:

            Clock() : m_start(std::chrono::high_resolution_clock::now()) {};

            ~Clock() = default;

            /*
            ** @brief Restart the clock
            */
            void restart() { m_start = std::chrono::high_resolution_clock::now(); };

            /*
            ** @brief Pause the clock
            */
            void pause();

            /*
            ** @brief Resume the clock
            */
            void resume();

            /*
            ** @brief Get the elapsed time since the last restart
            ** @return Time The elapsed time
            */
            [[nodiscard]] Time getElapsedTime() const;

        private:

            /*
            ** @property The start time
            */
            TimePoint m_start;

            /*
            ** @property The pause time
            */
            TimePoint m_pause;

            /*
            ** @property The "is in pause" boolean variable
            */
            bool m_paused{false};

    }; // Clock

} // namespace myLib
