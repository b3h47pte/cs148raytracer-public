#pragma once

#include "common/common.h"

class SceneObject : public std::enable_shared_from_this<SceneObject>
{
public:
    SceneObject();
    virtual ~SceneObject();

    virtual glm::mat4 GetTransformationMatrix() const;

    virtual glm::vec4 GetForwardDirection() const;
    virtual glm::vec4 GetRightDirection() const;
    virtual glm::vec4 GetUpDirection() const;

    static glm::vec4 GetWorldUp();
    static glm::vec4 GetWorldRight();
    static glm::vec4 GetWorldForward();

    //
    // Functions to transform the object.
    //
    void Translate(const glm::vec3& translation);
    void Rotate(const glm::vec3& axis, float radians);
    void MultScale(float inputScale);
    void AddScale(float inputScale);

    void SetPosition(const glm::vec3& in);

    //
    // Individual transform retrieval.
    //
    glm::vec4 GetPosition() const { return position; }

    virtual void AddMeshObject(std::shared_ptr<class MeshObject> object);
    virtual void AddMeshObject(const std::vector<std::shared_ptr<MeshObject>>& objects);
    virtual void Finalize() {}

protected:
    static const float MINIMUM_SCALE;

    virtual void UpdateTransformationMatrix();
    glm::mat4 cachedTransformationMatrix;

    glm::vec4 position;
    glm::quat rotation;
    glm::vec3 scale;

    class std::shared_ptr<class AccelerationStructure> acceleration;
    std::vector<std::shared_ptr<class MeshObject>> childObjects;
};