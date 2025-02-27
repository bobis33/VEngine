#include <unistd.h>
#include <fstream>

#include "Utils/MemoryMonitor.hpp"

static constexpr double KB_TO_MB = 1024.0;

void ven::MemoryMonitor::updateSystemMemory() {
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.starts_with("MemTotal:")) {
            total_memory = std::stod(line.substr(line.find(':') + 1)) / KB_TO_MB;
        } else if (line.starts_with("MemAvailable:")) {
            available_memory = std::stod(line.substr(line.find(':') + 1)) / KB_TO_MB;
        } else if (line.starts_with("SwapTotal:")) {
            swap_total = std::stod(line.substr(line.find(':') + 1)) / KB_TO_MB;
        } else if (line.starts_with("SwapFree:")) {
            swap_free = std::stod(line.substr(line.find(':') + 1)) / KB_TO_MB;
        }
    }
}

void ven::MemoryMonitor::updateProcessMemory() {
    std::ifstream file("/proc/self/statm");
    if (!file.is_open()) return;
    long rss = 0;
    file >> rss;
    process_memory_usage = rss * sysconf(_SC_PAGE_SIZE) / (KB_TO_MB * KB_TO_MB);
}
