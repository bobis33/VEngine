///
/// @file Logger.hpp
/// @brief This file contains the Logger class
/// @namespace utl
///

#pragma once

#include <iostream>
#include <iomanip>

#include "Utils/Clock.hpp"

namespace utl {

    enum class LogLevel : uint8_t {
        INFO,
        WARNING
    };

    ///
    /// @class Logger
    /// @brief Class for logger
    /// @namespace utl
    ///
    class Logger {

        public:

            Logger(const Logger &) = delete;
            Logger &operator=(const Logger &) = delete;
            Logger(Logger &&) = delete;
            Logger &operator=(Logger &&) = delete;

            template <typename Func>
            static void logExecutionTime(const std::string& message, const Func&& func) {
                const Clock clock;
                func();
                const float duration = clock.getDeltaSeconds() * 1000.0F;
                std::cout << getColorForDuration(duration) << formatLogMessage(LogLevel::INFO, message + " took " + std::to_string(duration) + " ms") << LOG_LEVEL_COLOR.at(3);
            }
            static void logWarning(const std::string& message) { std::cout << LOG_LEVEL_COLOR.at(2) << formatLogMessage(LogLevel::WARNING, message) << LOG_LEVEL_COLOR.at(3); }
            static void logInfo(const std::string& message) { std::cout << LOG_LEVEL_COLOR.at(1) << formatLogMessage(LogLevel::INFO, message) << LOG_LEVEL_COLOR.at(3); }

        private:

            static constexpr std::array<const char*, 2> LOG_LEVEL_STRING = {"INFO", "WARNING"};
            static constexpr std::array<const char*, 4> LOG_LEVEL_COLOR = {"\033[31m", "\033[32m", "\033[33m", "\033[0m\n"};

            Logger();
            ~Logger() = default;

            [[nodiscard]] static const char* getColorForDuration(const float duration) { return duration < 20.0F ? LOG_LEVEL_COLOR.at(1) : (duration < 90.0F ? LOG_LEVEL_COLOR.at(2) : LOG_LEVEL_COLOR.at(0)); }
            [[nodiscard]] static std::string formatLogMessage(LogLevel level, const std::string& message);

    }; // class Logger

}  // namespace utl
