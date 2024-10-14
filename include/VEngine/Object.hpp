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
#include "VEngine/Transform3DComponent.hpp"

namespace ven {

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

            [[nodiscard]] unsigned int getId() const { return m_objId; }
            [[nodiscard]] std::string getName() const { return m_name; }
            [[nodiscard]] std::shared_ptr<Model> getModel() const { return m_model; }

            void setName(const std::string &name) { m_name = name; }
            void setModel(const std::shared_ptr<Model> &model) { m_model = model; }

            Transform3DComponent transform3D{};

        private:

            explicit Object(const unsigned int objId) : m_objId(objId) {}

            unsigned int m_objId;
            std::string m_name;
            std::shared_ptr<Model> m_model;

    }; // class Object

} // namespace ven
