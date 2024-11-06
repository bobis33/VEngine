///
/// @file Object.hpp
/// @brief This file contains the Object Factory class
/// @namespace ven
///

#pragma once

#include "VEngine/Scene/Entities/Object.hpp"

namespace ven {

    ///
    /// @class ObjectFactory
    /// @brief Class for object factory
    /// @namespace ven
    ///
    class ObjectFactory {

        public:

            static std::unique_ptr<Object> create(const std::shared_ptr<Texture>& texture, const std::shared_ptr<Model>& model, const std::string &name, const Transform3D &transform);
            static std::unique_ptr<Object> duplicate(const Object& objSrc) { return create(objSrc.getDiffuseMap(), objSrc.getModel(), objSrc.getName(), objSrc.transform); }

        private:

            static unsigned int m_currentObjId;

    }; // class ObjectFactory

} // namespace ven
