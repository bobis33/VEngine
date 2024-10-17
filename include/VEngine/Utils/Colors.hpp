///
/// @file Colors.hpp
/// @brief
/// @namespace ven
///

#pragma once

#include <array>

#include <vulkan/vulkan.h>

namespace ven {

    static constexpr float COLOR_MAX = 255.0F;

    ///
    /// @class Colors
    /// @brief Class for colors
    /// @namespace ven
    ///
    class Colors {

        public:

            static constexpr glm::vec3 WHITE_3 = glm::vec3(COLOR_MAX) / COLOR_MAX;
            static constexpr glm::vec4 WHITE_4 = { 1.0F, 1.0F, 1.0F, 1.0F };
            static constexpr VkClearColorValue WHITE_V = { { 1.0F, 1.0F, 1.0F, 1.0F } };

            static constexpr glm::vec3 BLACK_3 = glm::vec3(0.0F);
            static constexpr glm::vec4 BLACK_4 = { 0.0F, 0.0F, 0.0F, 1.0F };
            static constexpr VkClearColorValue BLACK_V = { { 0.0F, 0.0F, 0.0F, 1.0F } };

            static constexpr glm::vec3 RED_3 = glm::vec3(COLOR_MAX, 0.0F, 0.0F) / COLOR_MAX;
            static constexpr glm::vec4 RED_4 = { 1.0F, 0.0F, 0.0F, 1.0F };
            static constexpr VkClearColorValue RED_V = { { 1.0F, 0.0F, 0.0F, 1.0F } };

            static constexpr glm::vec3 GREEN_3 = glm::vec3(0.0F, COLOR_MAX, 0.0F) / COLOR_MAX;
            static constexpr glm::vec4 GREEN_4 = { 0.0F, 1.0F, 0.0F, 1.0F };
            static constexpr VkClearColorValue GREEN_V = { { 0.0F, 1.0F, 0.0F, 1.0F } };

            static constexpr glm::vec3 BLUE_3 = glm::vec3(0.0F, 0.0F, COLOR_MAX) / COLOR_MAX;
            static constexpr glm::vec4 BLUE_4 = { 0.0F, 0.0F, 1.0F, 1.0F };
            static constexpr VkClearColorValue BLUE_V = { { 0.0F, 0.0F, 1.0F, 1.0F } };

            static constexpr glm::vec3 YELLOW_3 = glm::vec3(COLOR_MAX, COLOR_MAX, 0.0F) / COLOR_MAX;
            static constexpr glm::vec4 YELLOW_4 = { 1.0F, 1.0F, 0.0F, 1.0F };
            static constexpr VkClearColorValue YELLOW_V = { { 1.0F, 1.0F, 0.0F, 1.0F } };

            static constexpr glm::vec3 CYAN_3 = glm::vec3(0.0F, COLOR_MAX, COLOR_MAX) / COLOR_MAX;
            static constexpr glm::vec4 CYAN_4 = { 0.0F, 1.0F, 1.0F, 1.0F };
            static constexpr VkClearColorValue CYAN_V = { { 0.0F, 1.0F, 1.0F, 1.0F } };

            static constexpr glm::vec3 MAGENTA_3 = glm::vec3(COLOR_MAX, 0.0F, COLOR_MAX) / COLOR_MAX;
            static constexpr glm::vec4 MAGENTA_4 = { 1.0F, 0.0F, 1.0F, 1.0F };
            static constexpr VkClearColorValue MAGENTA_V = { { 1.0F, 0.0F, 1.0F, 1.0F } };

            static constexpr glm::vec3 SILVER_3 = glm::vec3(192.0F, 192.0F, 192.0F) / COLOR_MAX;
            static constexpr glm::vec4 SILVER_4 = { 0.75F, 0.75F, 0.75F, 1.0F };
            static constexpr VkClearColorValue SILVER_V = { { 0.75F, 0.75F, 0.75F, 1.0F } };

            static constexpr glm::vec3 GRAY_3 = glm::vec3(128.0F, 128.0F, 128.0F) / COLOR_MAX;
            static constexpr glm::vec4 GRAY_4 = { 0.5F, 0.5F, 0.5F, 1.0F };
            static constexpr VkClearColorValue GRAY_V = { { 0.5F, 0.5F, 0.5F, 1.0F } };

            static constexpr glm::vec3 MAROON_3 = glm::vec3(128.0F, 0.0F, 0.0F) / COLOR_MAX;
            static constexpr glm::vec4 MAROON_4 = { 0.5F, 0.0F, 0.0F, 1.0F };
            static constexpr VkClearColorValue MAROON_V = { { 0.5F, 0.0F, 0.0F, 1.0F } };

            static constexpr glm::vec3 OLIVE_3 = glm::vec3(128.0F, 128.0F, 0.0F) / COLOR_MAX;
            static constexpr glm::vec4 OLIVE_4 = { 0.5F, 0.5F, 0.0F, 1.0F };
            static constexpr VkClearColorValue OLIVE_V = { { 0.5F, 0.5F, 0.0F, 1.0F } };

            static constexpr glm::vec3 LIME_3 = glm::vec3(0.0F, COLOR_MAX, 0.0F) / COLOR_MAX;
            static constexpr glm::vec4 LIME_4 = { 0.0F, 1.0F, 0.0F, 1.0F };
            static constexpr VkClearColorValue LIME_V = { { 0.0F, 1.0F, 0.0F, 1.0F } };

            static constexpr glm::vec3 AQUA_3 = glm::vec3(0.0F, COLOR_MAX, COLOR_MAX) / COLOR_MAX;
            static constexpr glm::vec4 AQUA_4 = { 0.0F, 1.0F, 1.0F, 1.0F };
            static constexpr VkClearColorValue AQUA_V = { { 0.0F, 1.0F, 1.0F, 1.0F } };

            static constexpr glm::vec3 TEAL_3 = glm::vec3(0.0F, 128.0F, 128.0F) / COLOR_MAX;
            static constexpr glm::vec4 TEAL_4 = { 0.0F, 0.5F, 0.5F, 1.0F };
            static constexpr VkClearColorValue TEAL_V = { { 0.0F, 0.5F, 0.5F, 1.0F } };

            static constexpr glm::vec3 NAVY_3 = glm::vec3(0.0F, 0.0F, 128.0F) / COLOR_MAX;
            static constexpr glm::vec4 NAVY_4 = { 0.0F, 0.0F, 0.5F, 1.0F };
            static constexpr VkClearColorValue NAVY_V = { { 0.0F, 0.0F, 0.5F, 1.0F } };

            static constexpr glm::vec3 FUCHSIA_3 = glm::vec3(COLOR_MAX, 0.0F, COLOR_MAX) / COLOR_MAX;
            static constexpr glm::vec4 FUCHSIA_4 = { 1.0F, 0.0F, 1.0F, 1.0F };
            static constexpr VkClearColorValue FUCHSIA_V = { { 1.0F, 0.0F, 1.0F, 1.0F } };

            static constexpr glm::vec3 NIGHT_BLUE_3 = glm::vec3(25.0F, 25.0F, 112.0F) / COLOR_MAX;
            static constexpr glm::vec4 NIGHT_BLUE_4 = { 0.098F, 0.098F, 0.439F, 1.0F };
            static constexpr VkClearColorValue NIGHT_BLUE_V = { { 0.098F, 0.098F, 0.439F, 1.0F } };

            static constexpr glm::vec3 SKY_BLUE_3 = glm::vec3(102.0F, 178.0F, 255.0F) / COLOR_MAX;
            static constexpr glm::vec4 SKY_BLUE_4 = { 0.4F, 0.698F, 1.0F, 1.0F };
            static constexpr VkClearColorValue SKY_BLUE_V = { { 0.4F, 0.698F, 1.0F, 1.0F } };

            static constexpr glm::vec3 SUNSET_3 = glm::vec3(255.0F, 128.0F, 0.0F) / COLOR_MAX;
            static constexpr glm::vec4 SUNSET_4 = { 1.0F, 0.5F, 0.0F, 1.0F };
            static constexpr VkClearColorValue SUNSET_V = { { 1.0F, 0.5F, 0.0F, 1.0F } };


            static constexpr std::array<std::pair<const char *, glm::vec3>, 20> COLOR_PRESETS_3 = {{
                {"White", WHITE_3},
                {"Black", BLACK_3},
                {"Red", RED_3},
                {"Green", GREEN_3},
                {"Blue", BLUE_3},
                {"Yellow", YELLOW_3},
                {"Cyan", CYAN_3},
                {"Magenta", MAGENTA_3},
                {"Silver", SILVER_3},
                {"Gray", GRAY_3},
                {"Maroon", MAROON_3},
                {"Olive", OLIVE_3},
                {"Lime", LIME_3},
                {"Aqua", AQUA_3},
                {"Teal", TEAL_3},
                {"Navy", NAVY_3},
                {"Fuchsia", FUCHSIA_3},
                {"Night Blue", NIGHT_BLUE_3},
                {"Sky Blue", SKY_BLUE_3},
                {"Sunset", SUNSET_3}
            }};

            static constexpr std::array<std::pair<const char *, glm::vec4>, 20> COLOR_PRESETS_4 = {{
                {"White", WHITE_4},
                {"Black", BLACK_4},
                {"Red", RED_4},
                {"Green", GREEN_4},
                {"Blue", BLUE_4},
                {"Yellow", YELLOW_4},
                {"Cyan", CYAN_4},
                {"Magenta", MAGENTA_4},
                {"Silver", SILVER_4},
                {"Gray", GRAY_4},
                {"Maroon", MAROON_4},
                {"Olive", OLIVE_4},
                {"Lime", LIME_4},
                {"Aqua", AQUA_4},
                {"Teal", TEAL_4},
                {"Navy", NAVY_4},
                {"Fuchsia", FUCHSIA_4},
                {"Night Blue", NIGHT_BLUE_4},
                {"Sky Blue", SKY_BLUE_4},
                {"Sunset", SUNSET_4}
            }};

            static constexpr std::array<std::pair<const char *, VkClearColorValue>, 20> COLOR_PRESETS_VK = {{
                {"White", WHITE_V},
                {"Black", BLACK_V},
                {"Red", RED_V},
                {"Green", GREEN_V},
                {"Blue", BLUE_V},
                {"Yellow", YELLOW_V},
                {"Cyan", CYAN_V},
                {"Magenta", MAGENTA_V},
                {"Silver", SILVER_V},
                {"Gray", GRAY_V},
                {"Maroon", MAROON_V},
                {"Olive", OLIVE_V},
                {"Lime", LIME_V},
                {"Aqua", AQUA_V},
                {"Teal", TEAL_V},
                {"Navy", NAVY_V},
                {"Fuchsia", FUCHSIA_V},
                {"Night Blue", NIGHT_BLUE_V},
                {"Sky Blue", SKY_BLUE_V},
                {"Sunset", SUNSET_V}
            }};

    }; // class Colors

} // namespace ven
