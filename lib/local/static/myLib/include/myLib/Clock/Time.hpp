///
/// @file Time.hpp
/// @brief Class for time management
/// @namespace myLib
///

#pragma once

namespace myLib {

    static constexpr unsigned int MICROSECONDS_PER_SECOND = 1000000;
    static constexpr unsigned int MILLISECONDS_PER_SECOND = 1000;

    ///
    /// @class Time
    /// @brief Class used for time management
    ///
    class Time {

        public:

            ///
            /// @brief Construct a new Time object
            ///
            explicit Time(const double seconds) : m_seconds(seconds) {};

            ///
            /// @brief Transform the time to seconds
            /// @return int The time in seconds
            ///
            [[nodiscard]] int asSeconds() const { return static_cast<int>(m_seconds); };

            ///
            /// @brief Transform the time to milliseconds
            /// @return int The time in milliseconds
            ///
            [[nodiscard]] int asMilliseconds() const { return static_cast<int>(m_seconds * MILLISECONDS_PER_SECOND); }

            ///
            /// @brief Transform the time to microseconds
            /// @return int The time in microseconds
            ///
            [[nodiscard]] int asMicroseconds() const { return static_cast<int>(m_seconds * MICROSECONDS_PER_SECOND); };

        private:

            ///
            /// @property The time in seconds
            ///
            double m_seconds{0.0F};

    }; // Time

} // namespace myLib
