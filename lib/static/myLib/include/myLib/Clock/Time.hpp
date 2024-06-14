/*
** @file Time.hpp
** @brief Class for time management
** @namespace myLib
*/

#pragma once

namespace myLib {

    /*
    ** @class Time
    ** @brief Class used for time management
    */
    class Time {

        public:

            /*
            ** @brief Construct a new Time object
            */
            explicit Time(const double seconds) : m_seconds(seconds) {};

            /*
            ** @brief Transform the time to seconds
            ** @return int The time in seconds
            */
            [[nodiscard]] int asSeconds() const { return static_cast<int>(m_seconds); };

            /*
            ** @brief Transform the time to milliseconds
            ** @return int The time in milliseconds
            */
            [[nodiscard]] int asMilliseconds() const { return static_cast<int>(m_seconds * 1000); }

            /*
            ** @brief Transform the time to microseconds
            ** @return int The time in microseconds
            */
            [[nodiscard]] int asMicroseconds() const { return static_cast<int>(m_seconds * 1000000); };

        private:

            /*
            ** @property The time in seconds
            */
            double m_seconds{0.0F};

    }; // Time

} // namespace myLib
