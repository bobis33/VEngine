///
/// @file Transform3DComponent.hpp
/// @brief This file contains the Transform3DComponent class
/// @namespace ven
///

#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace ven {

    ///
    /// @class Transform3DComponent
    /// @brief Class for 3D transformation
    /// @namespace ven
    ///
    class Transform3DComponent {

        public:

            glm::vec3 translation{};
            glm::vec3 scale{1.F, 1.F, 1.F};
            glm::vec3 rotation{};

            [[nodiscard]] glm::mat4 mat4() const;
            [[nodiscard]] glm::mat3 normalMatrix() const;

    }; // class Transform3DComponent

} // namespace ven
