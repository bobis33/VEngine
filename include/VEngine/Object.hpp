/*
** @file Object.hpp
** @brief This file contains the Object class
** @namespace ven
*/

#pragma once

#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include "VEngine/Model.hpp"

namespace ven {

    using id_t = unsigned int;

    struct Transform3DComponent {
        glm::vec3 translation{};
        glm::vec3 scale{1.F, 1.F, 1.F};
        glm::vec3 rotation{};

        [[nodiscard]] glm::mat4 mat4() const {
            const float c3 = glm::cos(rotation.z);
            const float s3 = glm::sin(rotation.z);
            const float c2 = glm::cos(rotation.x);
            const float s2 = glm::sin(rotation.x);
            const float c1 = glm::cos(rotation.y);
            const float s1 = glm::sin(rotation.y);
            return glm::mat4{
                {
                    scale.x * (c1 * c3 + s1 * s2 * s3),
                    scale.x * (c2 * s3),
                    scale.x * (c1 * s2 * s3 - c3 * s1),
                    0.0F,
                },
                {
                    scale.y * (c3 * s1 * s2 - c1 * s3),
                    scale.y * (c2 * c3),
                    scale.y * (c1 * c3 * s2 + s1 * s3),
                    0.0F,
                },
                {
                    scale.z * (c2 * s1),
                    scale.z * (-s2),
                    scale.z * (c1 * c2),
                    0.0F,
                },
                {
                    translation.x,
                    translation.y,
                    translation.z,
                    1.0F
                }
            };
        }
    };

    class Object {

        public:

            static Object createObject() { static id_t objId = 0; return Object(objId++); }

            ~Object() = default;

            Object(const Object&) = delete;
            Object& operator=(const Object&) = delete;
            Object(Object&&) = default;
            Object& operator=(Object&&) = default;

            [[nodiscard]] id_t getId() const { return m_objId; }

            std::shared_ptr<ven::Model> model{};
            glm::vec3 color{};
            Transform3DComponent transform3D{};

    private:
            explicit Object(id_t objId) : m_objId(objId) {}

            id_t m_objId;

    }; // class Object

} // namespace ven
