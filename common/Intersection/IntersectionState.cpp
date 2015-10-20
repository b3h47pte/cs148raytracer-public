#include "common/Intersection/IntersectionState.h"
#include "common/Scene/Geometry/Primitives/PrimitiveBase.h"

glm::vec3 IntersectionState::ComputeNormal() const
{
    assert(hasIntersection && intersectedPrimitive);
    assert(primitiveIntersectionWeights.size() == static_cast<size_t>(intersectedPrimitive->GetTotalVertices()));

    if (intersectedPrimitive->HasVertexNormals()) {
        // If the mesh has normals, linearly interpolate the normals to get the normal to use.
        glm::vec3 retNormal;
        for (int i = 0; i < intersectedPrimitive->GetTotalVertices(); ++i) {
            retNormal += primitiveIntersectionWeights[i] * intersectedPrimitive->GetVertexNormal(i);
        }
        return glm::normalize(retNormal);
    }

    // Otherwise, use the face normal.
    return intersectedPrimitive->GetPrimitiveNormal();
}