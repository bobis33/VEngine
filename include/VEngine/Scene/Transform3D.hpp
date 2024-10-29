///
/// @file Transform3D.hpp
/// @brief This file contains the Transform3D class
/// @namespace ven
///

#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace ven {

    ///
    /// @class Transform3D
    /// @brief Class for 3D transformation
    /// @namespace ven
    ///
    class Transform3D {

        public:

            [[nodiscard]] glm::mat4 transformMatrix() const {
                auto rotationMatrix = glm::mat4(1.0F);

                rotationMatrix = rotate(rotationMatrix, rotation.x, glm::vec3(1.0F, 0.0F, 0.0F));
                rotationMatrix = rotate(rotationMatrix, rotation.y, glm::vec3(0.0F, 1.0F, 0.0F));
                rotationMatrix = rotate(rotationMatrix, rotation.z, glm::vec3(0.0F, 0.0F, 1.0F));

                const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0F), scale);
                const glm::mat4 translationMatrix = translate(glm::mat4(1.0F), translation);

                return translationMatrix * rotationMatrix * scaleMatrix;
            }
            [[nodiscard]] glm::mat3 normalMatrix() const { return transpose(inverse(glm::mat3(transformMatrix()))); }

            glm::vec3 translation{};
            glm::vec3 scale{1.F, 1.F, 1.F};
            glm::vec3 rotation{};

    }; // class Transform3D

} // namespace ven
