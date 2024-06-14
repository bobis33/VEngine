#include "myLib/Clock/Clock.hpp"

void myLib::Clock::pause()
{
    if (m_paused) {
        return;
    }
    m_pause = std::chrono::high_resolution_clock::now();
    m_paused = true;
}

void myLib::Clock::resume()
{
    if (!m_paused) {
        return;
    }

    m_start += std::chrono::high_resolution_clock::now() - m_pause;
    m_paused = false;
}

myLib::Time myLib::Clock::getElapsedTime() const
{
    TimePoint now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed_time{};
    if (m_paused) {
        elapsed_time = m_pause - m_start;
    } else {
        elapsed_time = now - m_start;
    }
    return Time(elapsed_time.count());
}
