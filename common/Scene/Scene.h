#pragma once

#include "common/common.h"
#include "common/Intersection/IntersectionState.h"

enum class AccelerationTypes;
class Light;
class SceneObject;

class Scene : public std::enable_shared_from_this<Scene>
{
public:
    void GenerateDefaultAccelerationData();
    class AccelerationStructure* GenerateAccelerationData(AccelerationTypes inputType);

    // if outputIntersection is NULL, this merely checks whether or not the inputRay hits something.
    // if outputIntersection is NOT NULL, then this will check whether or not the inputRay hits something,
    //      and if it does, it will store that information and perform reflection/refraction and keep going.
    bool Trace(class Ray* inputRay, IntersectionState* outputIntersection) const;

    size_t GetTotalObjects() const
    {
        return sceneObjects.size();
    }

    size_t GetTotalLights() const
    {
        return sceneLights.size();
    }

    template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
    const SceneObject& GetSceneObject(T index) const
    {
        assert(index >= 0 && index < sceneObjects.size());
        const std::shared_ptr<SceneObject>& internalObject = sceneObjects[index];
        return *internalObject.get();
    }

    template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
    const Light* GetLightObject(T index) const
    {
        if (index >= 0 && index < sceneLights.size()) {
            return sceneLights[index].get();
        }
        return nullptr;
    }

    void AddSceneObject(std::shared_ptr<SceneObject> object);
    void AddLight(std::shared_ptr<Light> light);

    void Finalize();

    void PerformRaySpecularReflection(Ray& outputRay, const Ray& inputRay, const glm::vec3& intersectionPoint, const float NdR, const IntersectionState& state) const;
    void PerformRayRefraction(Ray& outputRay, const Ray& inputRay, const glm::vec3& intersectionPoint, const float NdR, const IntersectionState& state, float& targetIOR) const;
private:
    std::shared_ptr<class AccelerationStructure> acceleration;

    std::vector<std::shared_ptr<SceneObject>> sceneObjects;
    std::vector<std::shared_ptr<Light>> sceneLights;
};