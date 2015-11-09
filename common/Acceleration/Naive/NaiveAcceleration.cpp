#include "common/Acceleration/Naive/NaiveAcceleration.h"
#include "common/Scene/Geometry/Ray/Ray.h"
#include "common/Intersection/IntersectionState.h"

void NaiveAcceleration::AddNode(std::shared_ptr<AccelerationNode> node)
{
    nodes.push_back(std::move(node));
}

bool NaiveAcceleration::Trace(const SceneObject* parentObject, Ray* inputRay, IntersectionState* outputIntersection) const
{
    bool hasHit = false;
    for (size_t i = 0; i < nodes.size(); ++i) {
        bool hit = nodes[i]->Trace(parentObject, inputRay, outputIntersection);
        // early exit when we just want to know whether or not we hit.
        if (hit && !outputIntersection) {
            return true; 
        }
        hasHit |= hit;
    }  
    return hasHit;
}