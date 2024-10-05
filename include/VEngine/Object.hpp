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

    static constexpr float DEFAULT_LIGHT_INTENSITY = .2F;
    static constexpr float DEFAULT_LIGHT_RADIUS = 0.1F;
    static constexpr glm::vec3 DEFAULT_LIGHT_COLOR = glm::vec3(1.F);

    struct Transform3DComponent {
        glm::vec3 translation{};
        glm::vec3 scale{1.F, 1.F, 1.F};
        glm::vec3 rotation{};

        [[nodiscard]] glm::mat4 mat4() const;
        [[nodiscard]] glm::mat3 normalMatrix() const;
    };

    struct PointLightComponent {
        float lightIntensity = DEFAULT_LIGHT_INTENSITY;
    };

    ///
    /// @class Object
    /// @brief Class for object
    /// @namespace ven
    ///
    class Object {

        public:

            using Map = std::unordered_map<unsigned int, Object>;

            ~Object() = default;

            Object(const Object&) = delete;
            Object& operator=(const Object&) = delete;
            Object(Object&&) = default;
            Object& operator=(Object&&) = default;

            static Object createObject() { static unsigned int objId = 0; return Object(objId++); }
            static Object makePointLight(float intensity = DEFAULT_LIGHT_INTENSITY, float radius = DEFAULT_LIGHT_RADIUS, glm::vec3 color = DEFAULT_LIGHT_COLOR);

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
