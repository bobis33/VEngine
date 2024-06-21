/*
** @file Object.hpp
** @brief This file contains the Object class
** @namespace ven
*/

#pragma once

#include <memory>

#include "VEngine/Model.hpp"

namespace ven {

    using id_t = unsigned int;


    struct Transform2dComponent {
        glm::vec2 translation{};
        glm::vec2 scale{1.F, 1.F};
        float rotation{};

        [[nodiscard]] glm::mat2 mat2() const {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMat{{c, s}, {-s, c}};
            glm::mat2 scaleMat{{scale.x, .0F}, {.0F, scale.y}};
            return scaleMat * rotMat;
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
            Transform2dComponent transform2d{};

    private:
            Object(id_t objId) : m_objId(objId) {}

            id_t m_objId;


    }; // class Object

} // namespace ven
