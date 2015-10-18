#include "common/Scene/SceneObject.h"
#include "common/Scene/Geometry/Mesh/MeshObject.h"

const float SceneObject::MINIMUM_SCALE = 0.01f;

SceneObject::SceneObject():
    cachedTransformationMatrix(1.f), position(0.f, 0.f, 0.f, 1.f), rotation(1.f, 0.f, 0.f, 0.f), scale(1.f)
{
}

SceneObject::~SceneObject()
{
}


glm::mat4 SceneObject::GetTransformationMatrix() const
{
    return cachedTransformationMatrix;
}

void SceneObject::UpdateTransformationMatrix()
{
    glm::mat4 newTransformation(1.f);
    newTransformation = glm::scale(newTransformation, scale);
    newTransformation = glm::mat4_cast(rotation) * newTransformation;
    newTransformation = glm::translate(newTransformation, glm::vec3(position));
    cachedTransformationMatrix = std::move(newTransformation);
}

glm::vec4 SceneObject::GetForwardDirection() const
{
    return glm::mat4_cast(rotation) * GetWorldForward();
}

glm::vec4 SceneObject::GetRightDirection() const
{
    return glm::mat4_cast(rotation) * GetWorldRight();
}

glm::vec4 SceneObject::GetUpDirection() const
{
    return glm::mat4_cast(rotation) * GetWorldUp();
}

glm::vec4 SceneObject::GetWorldUp()
{
    return glm::vec4(0.f, 1.f, 0.f, 0.f);
}

glm::vec4 SceneObject::GetWorldRight()
{
    return glm::vec4(1.f, 0.f, 0.f, 0.f);
}

glm::vec4 SceneObject::GetWorldForward()
{
    return glm::vec4(0.f, 0.f, -1.f, 0.f);
}

void SceneObject::SetPosition(const glm::vec3& in)
{
    position = glm::vec4(in, 0.f);
    UpdateTransformationMatrix();
}

void SceneObject::Translate(const glm::vec3& translation)
{
    position += glm::vec4(translation, 0.f);
    UpdateTransformationMatrix();
}

void SceneObject::Rotate(const glm::vec3& axis, float radians)
{
    glm::quat newRotation = glm::angleAxis(radians, axis);
    rotation = newRotation * rotation;
    UpdateTransformationMatrix();
}

void SceneObject::MultScale(float inputScale)
{
    scale *= inputScale;
    scale = glm::max(scale, MINIMUM_SCALE);
    UpdateTransformationMatrix();
}

void SceneObject::AddScale(float inputScale)
{
    scale += inputScale;
    scale = glm::max(scale, MINIMUM_SCALE);
    UpdateTransformationMatrix();
}

void SceneObject::AddMeshObject(std::shared_ptr<MeshObject> object)
{
    childObjects.emplace_back(std::move(object));
}

void SceneObject::AddMeshObject(const std::vector<std::shared_ptr<MeshObject>>& objects)
{
    for (size_t i = 0; i < objects.size(); ++i) {
        AddMeshObject(objects.at(i));
    }
}