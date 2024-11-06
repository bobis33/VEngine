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

        static std::unique_ptr<Object> createObject(const std::shared_ptr<Texture>& textureDefault);
        static std::unique_ptr<Object> duplicateObject(const Object& objSrc);

        private:

        static unsigned int m_currentObjId;

    }; // class ObjectFactory

} // namespace ven
