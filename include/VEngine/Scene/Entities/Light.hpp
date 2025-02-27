///
/// @file Light.hpp
/// @brief This file contains the Light class
/// @namespace ven
///

#pragma once

#include "VEngine/Scene/Transform3D.hpp"

namespace ven {

    static constexpr float DEFAULT_LIGHT_INTENSITY = .2F;
    static constexpr float DEFAULT_LIGHT_RADIUS = 0.1F;
    static constexpr float DEFAULT_SHININESS = 32.F;
    static constexpr glm::vec4 DEFAULT_LIGHT_COLOR = {glm::vec3(1.F), DEFAULT_LIGHT_INTENSITY};

    static constexpr uint8_t MAX_LIGHTS = 10;

    ///
    /// @class Light
    /// @brief Class for light
    /// @namespace ven
    ///
    class Light {

        public:

            using Map = std::unordered_map<unsigned int, Light>;

            explicit Light(const unsigned int objId) : m_lightId{objId} {}

            ~Light() = default;

            Light(const Light&) = delete;
            Light& operator=(const Light&) = delete;
            Light(Light&&) = default;
            Light& operator=(Light&&) = default;

            [[nodiscard]] unsigned int getId() const { return m_lightId; }
            [[nodiscard]] const std::string& getName() const { return m_name; }
            [[nodiscard]] float getShininess() const { return m_shininess; }

            void setName(const std::string &name) { m_name = name; }
            void setShininess(const float shininess) { m_shininess = shininess; }

            glm::vec4 color{DEFAULT_LIGHT_COLOR};
            Transform3D transform{};

        private:

            unsigned int m_lightId;
            std::string m_name{"point light"};
            float m_shininess{DEFAULT_SHININESS};

    }; // class Light

} // namespace ven
