#include "VEngine/Utils/Clock.hpp"

void ven::Clock::update()
{
    const auto newTime = std::chrono::high_resolution_clock::now();
    m_deltaTime = newTime - m_startTime;
    m_startTime = newTime;
}

void ven::Clock::stop()
{
    if (m_isStopped) {
        return;
    }

    m_stopTime = std::chrono::high_resolution_clock::now();
    m_isStopped = true;
}

void ven::Clock::resume()
{
    if (!m_isStopped) {
        return;
    }

    m_startTime += std::chrono::high_resolution_clock::now() - m_stopTime;
    m_isStopped = false;
}
