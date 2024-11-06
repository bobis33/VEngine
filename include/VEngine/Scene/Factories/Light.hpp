///
/// @file Light.hpp
/// @brief This file contains the Light Factory class
/// @namespace ven
///

#pragma once

#include <memory>

#include "VEngine/Scene/Entities/Light.hpp"

namespace ven {

    static constexpr Transform3D DEFAULT_TRANSFORM = {.translation = {0.F, 0.F, 0.F}, .scale = {0.01F, 0.F, 0.F}, .rotation = {0.F, 0.F, 0.F}};


    ///
    /// @class LightFactory
    /// @brief Class for light factory
    /// @namespace ven
    ///
    class LightFactory {

        public:

            static std::unique_ptr<Light> createLight(const Transform3D &transform = DEFAULT_TRANSFORM, glm::vec4 color = DEFAULT_LIGHT_COLOR);
            static std::unique_ptr<Light> duplicateLight(const Light &cpyLight);

        private:

            static unsigned int m_currentLightId;

    }; // class LightFactory

} // namespace ven
