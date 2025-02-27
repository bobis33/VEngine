#include <algorithm>
#include <fstream>

#include "Utils/FrameStats.hpp"

std::vector<float> ven::FrameStats::getDisplayFrameTimes() const {
    std::vector<float> displayFrameTimes(MAX_FRAME_TIMES);
    for (size_t i = 0; i < MAX_FRAME_TIMES; ++i) {
        displayFrameTimes[i] = m_data.frameTimes[(m_data.frameTimeIndex + i) % MAX_FRAME_TIMES];
    }
    return displayFrameTimes;
}

std::vector<float> ven::FrameStats::calculateFPS(const std::vector<float>& frameTimes) {
    std::vector<float> fpsTimes(frameTimes.size());
    for (size_t i = 0; i < frameTimes.size(); ++i) {
        fpsTimes[i] = frameTimes[i] > 0 ? 1.0F / frameTimes[i] : 0.0F;
    }
    return fpsTimes;
}

void ven::FrameStats::exportDataToCSV(const std::vector<float>& frameTimes, const std::vector<float>& fpsTimes, const std::string& filename) {
    std::ofstream outFile(filename);
    for (size_t i = 0; i < frameTimes.size(); ++i) {
        outFile << frameTimes[i] * 1000.0F << ',' << fpsTimes[i] << '\n';
    }
    outFile.close();
}

float ven::FrameStats::calculateUpperBound(const std::vector<float>& data) {
    const float maxValue = *std::ranges::max_element(data);
    const float upperBound = maxValue * 1.2F;
    return upperBound < 0.0005F ? 0.0005F : upperBound;
}
