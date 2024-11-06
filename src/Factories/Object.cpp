#include "VEngine/Factories/Object.hpp"

unsigned int ven::ObjectFactory::m_currentObjId = 0;

std::unique_ptr<ven::Object> ven::ObjectFactory::create(const std::shared_ptr<Texture>& texture, const std::shared_ptr<Model>& model, const std::string &name, const Transform3D &transform)
{
    assert(m_currentObjId < MAX_OBJECTS && "Max object count exceeded!");
    auto object = std::make_unique<Object>(++m_currentObjId);
    object->setDiffuseMap(texture);
    object->setModel(model);
    object->setName(name);
    object->transform = transform;
    return object;
}
