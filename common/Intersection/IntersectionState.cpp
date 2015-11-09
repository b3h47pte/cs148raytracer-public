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
        glm::vec3 retTangent;
        glm::vec3 retBitangent;
        for (int i = 0; i < intersectedPrimitive->GetTotalVertices(); ++i) {
            retNormal += primitiveIntersectionWeights[i] * normalTransform * intersectedPrimitive->GetVertexNormal(i);
            retTangent += primitiveIntersectionWeights[i] * normalTransform* intersectedPrimitive->GetVertexTangent(i);
            retBitangent += primitiveIntersectionWeights[i] * normalTransform * intersectedPrimitive->GetVertexBitangent(i);
        }

        if (intersectedPrimitive->HasNormalMap()) {
            return glm::normalize(intersectedPrimitive->GetVertexNormalMap(ComputeUV(), retTangent, retBitangent, retNormal));
        }

        return glm::normalize(retNormal);
    }

    // Otherwise, use the face normal.
    return glm::normalize(normalTransform * intersectedPrimitive->GetPrimitiveNormal());
}

glm::vec2 IntersectionState::ComputeUV() const
{
    assert(hasIntersection && intersectedPrimitive && primitiveParent);
    assert(primitiveIntersectionWeights.size() == static_cast<size_t>(intersectedPrimitive->GetTotalVertices()));

    glm::vec2 retUV;
    for (int i = 0; i < intersectedPrimitive->GetTotalVertices(); ++i) {
        retUV += primitiveIntersectionWeights[i] * intersectedPrimitive->GetVertexUV(i);
    }
    return retUV;
}