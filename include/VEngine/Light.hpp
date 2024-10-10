///
/// @file Light.hpp
/// @brief This file contains the Light class
/// @namespace ven
///

#pragma once

#include <memory>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

#include "VEngine/Transform3DComponent.hpp"

namespace ven {

    static constexpr float DEFAULT_LIGHT_INTENSITY = .2F;
    static constexpr float DEFAULT_LIGHT_RADIUS = 0.1F;
    static constexpr glm::vec4 DEFAULT_LIGHT_COLOR = {glm::vec3(1.F), DEFAULT_LIGHT_INTENSITY};

    ///
    /// @class Light
    /// @brief Class for light
    /// @namespace ven
    ///
    class Light {

        public:

            using Map = std::unordered_map<unsigned int, Light>;

            ~Light() = default;

            Light(const Light&) = delete;
            Light& operator=(const Light&) = delete;
            Light(Light&&) = default;
            Light& operator=(Light&&) = default;

            static Light createLight(float radius = DEFAULT_LIGHT_RADIUS, glm::vec4 color = DEFAULT_LIGHT_COLOR);

            glm::vec4 color{DEFAULT_LIGHT_COLOR};
            Transform3DComponent transform3D{};

            [[nodiscard]] unsigned int getId() const { return m_lightId; }
            [[nodiscard]] std::string getName() const { return m_name; }

            void setName(const std::string &name) { m_name = name; }

        private:

            explicit Light(const unsigned int lightId) : m_lightId(lightId) {}

            unsigned int m_lightId;
            std::string m_name{"point light"};

    }; // class Light

} // namespace ven
