///
/// @file Clock.hpp
/// @brief This file contains the Clock class
/// @namespace utl
///

#pragma once

#include <ostream>
#include <chrono>

namespace utl {

    ///
    /// @class Clock
    /// @brief Class for clock
    /// @namespace utl
    ///
    class Clock {

        public:

            using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

            explicit Clock(const bool startNow = true) : m_start{startNow ? now() : TimePoint()}, m_pausedDuration{0} {}
            ~Clock() = default;

            Clock(const Clock&) = delete;
            Clock& operator=(const Clock&) = delete;
            Clock(Clock&&) = delete;
            Clock& operator=(Clock&&) = delete;

            friend std::ostream& operator<<(std::ostream& os, const Clock& clock) { os << "Elapsed time: " << clock.getDeltaSeconds() << " seconds"; return os; }

            static TimePoint now() { return std::chrono::high_resolution_clock::now(); }
            void restart() { m_start = now(); m_pausedDuration = Duration(0); m_isPaused = false; }
            void pause() { if (!m_isPaused) { m_pausedTime = now(); m_isPaused = true; } }
            void resume() { if (m_isPaused) { m_pausedDuration += now() - m_pausedTime; m_isPaused = false; } }
            [[nodiscard]] float getDeltaSeconds() const {
                if (m_isPaused) {
                    return std::chrono::duration<float>(m_pausedTime - m_start - m_pausedDuration).count();
                }
                return std::chrono::duration<float>(now() - m_start - m_pausedDuration).count();
            }

            template <typename Duration = std::chrono::seconds>
            [[nodiscard]] auto getElapsed() const { return std::chrono::duration_cast<Duration>(now() - m_start - m_pausedDuration); }

        private:

            using Duration = std::chrono::high_resolution_clock::duration;

            TimePoint m_start;
            TimePoint m_pausedTime;
            Duration m_pausedDuration;
            bool m_isPaused{false};

    }; // class Clock

} // namespace utl
