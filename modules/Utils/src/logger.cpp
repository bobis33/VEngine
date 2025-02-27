#ifdef _WIN32
    #include <windows.h>
#endif

#include "Utils/Logger.hpp"

utl::Logger::Logger() {
#ifdef _WIN32
    const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (hOut != INVALID_HANDLE_VALUE && (GetConsoleMode(hOut, &dwMode) != 0)) {
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

std::string utl::Logger::formatLogMessage(const LogLevel level, const std::string& message) {
    const auto inTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::ostringstream ss;
    ss << "[" << std::put_time(std::localtime(&inTime), "%Y-%m-%d %X") << "] ";
    ss << "[" << LOG_LEVEL_STRING.at(static_cast<uint8_t>(level)) << "] " << message;
    return ss.str();
}
