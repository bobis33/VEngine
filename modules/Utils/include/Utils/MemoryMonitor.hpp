///
/// @file MemoryMonitor.hpp
/// @brief This file contains the MemoryMonitor class
/// @namespace ven
///

#pragma once

namespace ven {

    ///
    /// @class MemoryMonitor
    /// @brief Class for memory monitor
    /// @namespace ven
    ///
    class MemoryMonitor {

        public:

            MemoryMonitor() { update(); }
            ~MemoryMonitor() = default;

            MemoryMonitor(const MemoryMonitor&) = delete;
            MemoryMonitor& operator=(const MemoryMonitor&) = delete;
            MemoryMonitor(MemoryMonitor&&) = delete;
            MemoryMonitor& operator=(MemoryMonitor&&) = delete;

            void update() { updateSystemMemory(); updateProcessMemory(); }

            [[nodiscard]] double getMemoryUsage() const { return total_memory - available_memory; }
            [[nodiscard]] double getTotalMemory() const { return total_memory; }
            [[nodiscard]] double getAvailableMemory() const { return available_memory; }
            [[nodiscard]] double getSwapUsage() const { return swap_total - swap_free; }
            [[nodiscard]] double getTotalSwap() const { return swap_total; }
            [[nodiscard]] double getFreeSwap() const { return swap_free; }
            [[nodiscard]] double getProcessMemoryUsage() const { return process_memory_usage; }

        private:

            void updateSystemMemory();
            void updateProcessMemory();

            double total_memory = 0.0;
            double available_memory = 0.0;
            double swap_total = 0.0;
            double swap_free = 0.0;
            double process_memory_usage = 0.0;

    }; // class MemoryMonitor

} // namespace ven
