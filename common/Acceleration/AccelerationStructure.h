#pragma once

#include "common/common.h"
#include "common/Acceleration/AccelerationNode.h"
#include <type_traits>

class AccelerationStructure
{
public:
    AccelerationStructure();
    virtual ~AccelerationStructure();
    
    template<typename T, typename std::enable_if<std::is_base_of<AccelerationNode, T>::value>::type* = nullptr>
    void Initialize(const std::vector<std::shared_ptr<T>>& inputData)
    {
        nodes.resize(inputData.size());
        for (size_t i = 0; i < inputData.size(); ++i) {
            nodes[i] = inputData.at(i);
        }

        InternalInitialization();
    }

    virtual bool Trace(const class SceneObject* sceneObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const = 0;
protected:
    std::vector<std::shared_ptr<AccelerationNode>> nodes;

private:
    virtual void InternalInitialization() {}
};