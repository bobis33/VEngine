///
/// @file Light.hpp
/// @brief This file contains the Light Factory class
/// @namespace ven
///

#pragma once

#include "VEngine/Scene/Entities/Light.hpp"

#include <memory>

namespace ven {

    ///
    /// @class LightFactory
    /// @brief Class for light factory
    /// @namespace ven
    ///
    class LightFactory {

        public:

            static std::unique_ptr<ven::Light> createLight(float radius = DEFAULT_LIGHT_RADIUS, glm::vec4 color = DEFAULT_LIGHT_COLOR);
            static std::unique_ptr<ven::Light> duplicateLight(const Light &cpyLight);

        private:

            static unsigned int m_currentLightId;

    }; // class LightFactory

} // namespace ven
