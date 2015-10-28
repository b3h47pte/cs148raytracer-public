#include "common/Scene/SceneObject.h"
#include "common/Scene/Geometry/Mesh/MeshObject.h"
#include "common/Scene/Geometry/Ray/Ray.h"
#include "common/Intersection/IntersectionState.h"

const float SceneObject::MINIMUM_SCALE = 0.01f;

SceneObject::SceneObject():
    worldToObjectMatrix(1.f), objectToWorldMatrix(1.f), position(0.f, 0.f, 0.f, 1.f), rotation(1.f, 0.f, 0.f, 0.f), scale(1.f)
{
}

SceneObject::~SceneObject()
{
}


glm::mat4 SceneObject::GetObjectToWorldMatrix() const
{
    return objectToWorldMatrix;
}

glm::mat4 SceneObject::GetWorldToObjectMatrix() const
{
    return worldToObjectMatrix;
}

void SceneObject::UpdateTransformationMatrix()
{
    objectToWorldMatrix = glm::mat4(1.f);
    objectToWorldMatrix = glm::scale(glm::mat4(1.f), scale) * objectToWorldMatrix;
    objectToWorldMatrix = glm::mat4_cast(rotation) * objectToWorldMatrix;
    objectToWorldMatrix = glm::translate(glm::mat4(1.f), glm::vec3(position)) * objectToWorldMatrix;
    worldToObjectMatrix = glm::inverse(objectToWorldMatrix);
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
    position = glm::vec4(in, 1.f);
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

void SceneObject::CreateDefaultAccelerationData()
{
    if (!acceleration) {
        CreateAccelerationData(AccelerationTypes::NONE);
    }
    assert(acceleration);
}

void SceneObject::CreateAccelerationData(AccelerationTypes perObjectType)
{
    acceleration = AccelerationGenerator::CreateStructureFromType(perObjectType);
    assert(acceleration);
    for (size_t i = 0; i < childObjects.size(); ++i) {
        childObjects[i]->CreateAccelerationData(perObjectType);
    }
}

void SceneObject::Finalize()
{
    boundingBox.Reset();
    for (size_t i = 0; i < childObjects.size(); ++i) {
        childObjects[i]->Finalize();
        boundingBox.IncludeBox(childObjects[i]->GetBoundingBox());
    }

    // TODO: Store the obj 2 world matrix in box and do box in object space.
    // scene object bounding box is in world space ... can't think of an easy way to get it to be in object space.
    boundingBox.minVertex = glm::vec3(objectToWorldMatrix * glm::vec4(boundingBox.minVertex, 1.f));
    boundingBox.maxVertex = glm::vec3(objectToWorldMatrix * glm::vec4(boundingBox.maxVertex, 1.f));

    glm::vec3 tmp = boundingBox.minVertex;
    boundingBox.minVertex = glm::min(boundingBox.minVertex, boundingBox.maxVertex);
    boundingBox.maxVertex = glm::max(tmp, boundingBox.maxVertex);

    assert(acceleration);
    acceleration->Initialize(childObjects);
}

bool SceneObject::Trace(const SceneObject* parentObject, Ray* inputRay, IntersectionState* outputIntersection) const
{
    return acceleration->Trace(this, inputRay, outputIntersection);
}