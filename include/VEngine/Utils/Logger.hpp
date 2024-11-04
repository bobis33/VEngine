///
/// @file Logger.hpp
/// @brief This file contains the Logger class
/// @namespace ven
///

#pragma once

#include <iostream>
#include <array>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
    #include <windows.h>
#endif

#include "VEngine/Utils/Clock.hpp"


namespace ven {

    enum class LogLevel : uint8_t {
        INFO,
        WARNING
    };

    class Logger {

        public:

            static Logger& getInstance() { static Logger instance; return instance; }

            template <typename Func>
            static void logExecutionTime(const std::string& message, Func&& func)
            {
                Clock clock;
                func();
                clock.update();
                const float duration = clock.getDeltaTimeMS();

                std::cout << getColorForDuration(duration) << formatLogMessage(LogLevel::INFO, message + " took " + std::to_string(duration) + " ms") << LOG_LEVEL_COLOR.at(3);
            }

        private:

            static constexpr std::array<const char*, 2> LOG_LEVEL_STRING = {"INFO", "WARNING"};
            static constexpr std::array<const char*, 4> LOG_LEVEL_COLOR = {"\033[31m", "\033[32m", "\033[33m", "\033[0m\n"};

            Logger();

            static const char* getColorForDuration(const float duration) { return duration < 20.0F ? LOG_LEVEL_COLOR.at(1) : (duration < 90.0F ? LOG_LEVEL_COLOR.at(2) : LOG_LEVEL_COLOR.at(0)); }

            [[nodiscard]] static std::string formatLogMessage(LogLevel level, const std::string& message);

    }; // class Logger

}  // namespace ven
