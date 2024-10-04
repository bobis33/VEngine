///
/// @file Object.hpp
/// @brief This file contains the Object class
/// @namespace ven
///

#pragma once

#include <memory>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

#include "VEngine/Model.hpp"

namespace ven {

    struct Transform3DComponent {
        glm::vec3 translation{};
        glm::vec3 scale{1.F, 1.F, 1.F};
        glm::vec3 rotation{};

        [[nodiscard]] glm::mat4 mat4() const;
        [[nodiscard]] glm::mat3 normalMatrix() const;
    };

    struct PointLightComponent {
        float lightIntensity = 1.0F;
    };

    class Object {

        public:

         using Map = std::unordered_map<unsigned int, Object>;


            static Object createObject() { static unsigned int objId = 0; return Object(objId++); }

            ~Object() = default;

            static Object makePointLight(float intensity = 10.F, float radius = 0.1F, glm::vec3 color = glm::vec3(1.F));

            Object(const Object&) = delete;
            Object& operator=(const Object&) = delete;
            Object(Object&&) = default;
            Object& operator=(Object&&) = default;

            [[nodiscard]] unsigned int getId() const { return m_objId; }

            std::shared_ptr<Model> model{};
            glm::vec3 color{};
            Transform3DComponent transform3D{};
            std::string name{""};

            std::unique_ptr<PointLightComponent> pointLight = nullptr;

        private:

            explicit Object(const unsigned int objId) : m_objId(objId) {}

            unsigned int m_objId;

    }; // class Object

} // namespace ven
