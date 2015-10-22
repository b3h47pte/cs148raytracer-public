#include "common/Intersection/IntersectionState.h"
#include "common/Scene/Geometry/Primitives/PrimitiveBase.h"

glm::vec3 IntersectionState::ComputeNormal() const
{
    assert(hasIntersection && intersectedPrimitive && primitiveParent);
    assert(primitiveIntersectionWeights.size() == static_cast<size_t>(intersectedPrimitive->GetTotalVertices()));

    const glm::mat3 normalTransform = glm::mat3(glm::transpose(glm::inverse(primitiveParent->GetObjectToWorldMatrix())));

    if (intersectedPrimitive->HasVertexNormals()) {
        // If the mesh has normals, linearly interpolate the normals to get the normal to use.
        glm::vec3 retNormal;
        for (int i = 0; i < intersectedPrimitive->GetTotalVertices(); ++i) {
            retNormal += primitiveIntersectionWeights[i] * intersectedPrimitive->GetVertexNormal(i);
        }
        return glm::normalize(normalTransform * retNormal);
    }

    // Otherwise, use the face normal.
    return glm::normalize(normalTransform * intersectedPrimitive->GetPrimitiveNormal());
}