#include "common/Acceleration/Naive/NaiveAcceleration.h"
#include "common/Scene/Geometry/Ray/Ray.h"
#include "common/Intersection/IntersectionState.h"

bool NaiveAcceleration::Trace(Ray* inputRay, IntersectionState* outputIntersection) const
{
    std::vector<IntersectionState> allIntersections;
    for (size_t i = 0; i < nodes.size(); ++i) {
        IntersectionState newIntersection;
        bool hit = nodes[i]->Trace(inputRay, &newIntersection);

        // early exit when we just want to know whether or not we hit.
        if (hit && !outputIntersection) {
            return true; 
        }

        if (!hit) {
            continue;
        }

        allIntersections.push_back(newIntersection);
    }

    if (allIntersections.empty()) {
        return false;
    }
    
    return true;
}