///
/// @file Colors.hpp
/// @brief
/// @namespace ven
///

#pragma once

#include <array>

#include <vulkan/vulkan.h>

namespace ven {

    ///
    /// @class Colors
    /// @brief Class for colors
    /// @namespace ven
    ///
    class Colors {

        static constexpr float COLOR_MAX = 255.0F;

        public:

            static constexpr glm::vec3 WHITE = glm::vec3(COLOR_MAX, COLOR_MAX, COLOR_MAX) / COLOR_MAX;
            static constexpr glm::vec3 BLACK = glm::vec3(0.0F);
            static constexpr glm::vec3 RED = glm::vec3(COLOR_MAX, 0.0F, 0.0F) / COLOR_MAX;
            static constexpr glm::vec3 GREEN = glm::vec3(0.0F, COLOR_MAX, 0.0F) / COLOR_MAX;
            static constexpr glm::vec3 BLUE = glm::vec3(0.0F, 0.0F, COLOR_MAX) / COLOR_MAX;
            static constexpr glm::vec3 YELLOW = glm::vec3(COLOR_MAX, COLOR_MAX, 0.0F) / COLOR_MAX;
            static constexpr glm::vec3 CYAN = glm::vec3(0.0F, COLOR_MAX, COLOR_MAX) / COLOR_MAX;
            static constexpr glm::vec3 MAGENTA = glm::vec3(COLOR_MAX, 0.0F, COLOR_MAX) / COLOR_MAX;
            static constexpr glm::vec3 SILVER = glm::vec3(192.0F, 192.0F, 192.0F) / COLOR_MAX;
            static constexpr glm::vec3 GRAY = glm::vec3(128.0F, 128.0F, 128.0F) / COLOR_MAX;
            static constexpr glm::vec3 MAROON = glm::vec3(128.0F, 0.0F, 0.0F) / COLOR_MAX;
            static constexpr glm::vec3 OLIVE = glm::vec3(128.0F, 128.0F, 0.0F) / COLOR_MAX;
            static constexpr glm::vec3 LIME = glm::vec3(0.0F, COLOR_MAX, 0.0F) / COLOR_MAX;
            static constexpr glm::vec3 AQUA = glm::vec3(0.0F, COLOR_MAX, COLOR_MAX) / COLOR_MAX;
            static constexpr glm::vec3 TEAL = glm::vec3(0.0F, 128.0F, 128.0F) / COLOR_MAX;
            static constexpr glm::vec3 NAVY = glm::vec3(0.0F, 0.0F, 128.0F) / COLOR_MAX;
            static constexpr glm::vec3 FUCHSIA = glm::vec3(COLOR_MAX, 0.0F, COLOR_MAX) / COLOR_MAX;
            static constexpr glm::vec3 NIGHT_BLUE = glm::vec3(25.0F, 25.0F, 112.0F) / COLOR_MAX;
            static constexpr glm::vec3 SKY_BLUE = glm::vec3(102.0F, 178.0F, 255.0F) / COLOR_MAX;
            static constexpr glm::vec3 SUNSET = glm::vec3(255.0F, 128.0F, 0.0F) / COLOR_MAX;

            static constexpr VkClearColorValue WHITE_V = {{1.0F, 1.0F, 1.0F, 1.0F}};
            static constexpr VkClearColorValue BLACK_V = {{0.0F, 0.0F, 0.0F, 1.0F}};
            static constexpr VkClearColorValue RED_V = {{1.0F, 0.0F, 0.0F, 1.0F}};
            static constexpr VkClearColorValue GREEN_V = {{0.0F, 1.0F, 0.0F, 1.0F}};
            static constexpr VkClearColorValue BLUE_V = {{0.0F, 0.0F, 1.0F, 1.0F}};
            static constexpr VkClearColorValue YELLOW_V = {{1.0F, 1.0F, 0.0F, 1.0F}};
            static constexpr VkClearColorValue CYAN_V = {{0.0F, 1.0F, 1.0F, 1.0F}};
            static constexpr VkClearColorValue MAGENTA_V = {{1.0F, 0.0F, 1.0F, 1.0F}};
            static constexpr VkClearColorValue SILVER_V = {{0.75F, 0.75F, 0.75F, 1.0F}};
            static constexpr VkClearColorValue GRAY_V = {{0.5F, 0.5F, 0.5F, 1.0F}};
            static constexpr VkClearColorValue MAROON_V = {{0.5F, 0.0F, 0.0F, 1.0F}};
            static constexpr VkClearColorValue OLIVE_V = {{0.5F, 0.5F, 0.0F, 1.0F}};
            static constexpr VkClearColorValue LIME_V = {{0.0F, 1.0F, 0.0F, 1.0F}};
            static constexpr VkClearColorValue AQUA_V = {{0.0F, 1.0F, 1.0F, 1.0F}};
            static constexpr VkClearColorValue TEAL_V = {{0.0F, 0.5F, 0.5F, 1.0F}};
            static constexpr VkClearColorValue NAVY_V = {{0.0F, 0.0F, 0.5F, 1.0F}};
            static constexpr VkClearColorValue FUCHSIA_V = {{1.0F, 0.0F, 1.0F, 1.0F}};
            static constexpr VkClearColorValue NIGHT_BLUE_V = {{0.1F, 0.1F, 0.44F, 1.0F}};
            static constexpr VkClearColorValue SKY_BLUE_V = {{0.4F, 0.6F, 0.9F, 1.0F}};
            static constexpr VkClearColorValue SUNSET_V = {{1.0F, 0.5F, 0.0F, 1.0F}};
            static constexpr VkClearColorValue NIGHT_MODE_V = {{0.0F, 0.0F, 0.0F, 1.0F}};

        static constexpr std::array<std::pair<const char*, glm::vec3>, 20> COLORS = {{
                {"White", Colors::WHITE},
                {"Black", Colors::BLACK},
                {"Red", Colors::RED},
                {"Green", Colors::GREEN},
                {"Blue", Colors::BLUE},
                {"Yellow", Colors::YELLOW},
                {"Cyan", Colors::CYAN},
                {"Magenta", Colors::MAGENTA},
                {"Silver", Colors::SILVER},
                {"Gray", Colors::GRAY},
                {"Maroon", Colors::MAROON},
                {"Olive", Colors::OLIVE},
                {"Lime", Colors::LIME},
                {"Aqua", Colors::AQUA},
                {"Teal", Colors::TEAL},
                {"Navy", Colors::NAVY},
                {"Fuchsia", Colors::FUCHSIA},
                {"Night Blue", ven::Colors::NIGHT_BLUE},
                {"Sky Blue", Colors::SKY_BLUE},
                {"Sunset", Colors::SUNSET}
        }};

        static constexpr std::array<std::pair<const char*, VkClearColorValue>, 20> CLEAR_COLORS = {{
                {"White", Colors::WHITE_V},
                {"Black", Colors::BLACK_V},
                {"Red", Colors::RED_V},
                {"Green", Colors::GREEN_V},
                {"Blue", Colors::BLUE_V},
                {"Yellow", Colors::YELLOW_V},
                {"Cyan", Colors::CYAN_V},
                {"Magenta", Colors::MAGENTA_V},
                {"Silver", Colors::SILVER_V},
                {"Gray", Colors::GRAY_V},
                {"Maroon", Colors::MAROON_V},
                {"Olive", Colors::OLIVE_V},
                {"Lime", Colors::LIME_V},
                {"Aqua", Colors::AQUA_V},
                {"Teal", Colors::TEAL_V},
                {"Navy", Colors::NAVY_V},
                {"Fuchsia", Colors::FUCHSIA_V},
                {"Night Blue", Colors::NIGHT_BLUE_V},
                {"Sky Blue", Colors::SKY_BLUE_V},
                {"Sunset", Colors::SUNSET_V}
        }};

    }; // class Colors

} // namespace ven
