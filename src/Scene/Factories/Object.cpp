#include "VEngine/Scene/Factories/Object.hpp"

unsigned int ven::ObjectFactory::m_currentObjId = 0;

std::unique_ptr<ven::Object> ven::ObjectFactory::createObject(const std::shared_ptr<Texture>& texture, const std::shared_ptr<Model>& model, const std::string &name, const Transform3D &transform)
{
    assert(m_currentObjId < MAX_OBJECTS && "Max object count exceeded!");
    auto object = std::make_unique<Object>(++m_currentObjId);
    object->setDiffuseMap(texture);
    object->setModel(model);
    object->setName(name);
    object->transform = transform;
    return object;
}

std::unique_ptr<ven::Object> ven::ObjectFactory::duplicateObject(const Object& objSrc)
{
    auto object = createObject(objSrc.getDiffuseMap(), objSrc.getModel(), objSrc.getName(), objSrc.transform);
    object->setName(objSrc.getName());
    object->setModel(objSrc.getModel());
    object->transform = objSrc.transform;
    object->setDiffuseMap(objSrc.getDiffuseMap());
    return object;
}