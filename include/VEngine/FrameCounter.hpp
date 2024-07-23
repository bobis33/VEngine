///
/// @file FrameCounter.hpp
/// @brief This file contains the FrameCounter class
/// @namespace ven
///

#pragma once

#include <iostream>

namespace ven {

    class FrameCounter {

        public:

            FrameCounter() = default;
            ~FrameCounter() = default;

            void update(const float deltaTime) {
                m_frameCounter += 1.F;
                m_timeCounter += deltaTime;

                if (m_timeCounter >= 1.F) {
                    std::cout << "FPS: " << m_frameCounter << '\n';
                    m_fps = m_frameCounter;
                    m_frameTime = 1000.F / m_fps;
                    m_frameCounter = 0.F;
                    m_timeCounter = 0.F;
                }
            }

            [[nodiscard]] float getFps() const { return m_fps; }
            [[nodiscard]] float getFrameTime() const { return m_frameTime; }

        private:

            float m_fps{0.F};
            float m_frameTime{0.F};
            float m_frameCounter{0.F};
            float m_timeCounter{0.F};

    }; // class FrameCounter

} // namespace ven