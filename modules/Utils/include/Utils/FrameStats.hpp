///
/// @file FrameStats.hpp
/// @brief This file contains the FrameStats class
/// @namespace ven
///

#pragma once

#include <vector>
#include <string>

namespace ven {

    ///
    /// @class FrameStats
    /// @brief Class for frame statistics
    /// @namespace ven
    ///
    class FrameStats {

        public:

            static constexpr size_t MAX_FRAME_TIMES = 100;

            struct data {
                std::vector<float> frameTimes;
                size_t frameTimeIndex{0};
            };

            FrameStats() { m_data.frameTimes.resize(MAX_FRAME_TIMES); }
            ~FrameStats() = default;

            FrameStats(const FrameStats&) = delete;
            FrameStats& operator=(const FrameStats&) = delete;
            FrameStats(FrameStats&&) = delete;
            FrameStats& operator=(FrameStats&&) = delete;

            void updateFrameTimes(const float frameTime) {
                m_data.frameTimes[m_data.frameTimeIndex] = frameTime;
                m_data.frameTimeIndex = (m_data.frameTimeIndex + 1) % MAX_FRAME_TIMES;
            }

            [[nodiscard]] std::vector<float> getDisplayFrameTimes() const;
            [[nodiscard]] static std::vector<float> calculateFPS(const std::vector<float>& frameTimes);
            [[nodiscard]] static float calculateUpperBound(const std::vector<float>& data);
            static void exportDataToCSV(const std::vector<float>& frameTimes, const std::vector<float>& fpsTimes, const std::string& filename);

        private:

            data m_data;

    }; // class FrameStats

} // namespace ven
