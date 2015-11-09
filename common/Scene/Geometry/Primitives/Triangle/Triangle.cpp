#include "common/Scene/Geometry/Primitives/Triangle/Triangle.h"
#include "common/Scene/Geometry/Ray/Ray.h"
#include "common/Intersection/IntersectionState.h"

Triangle::Triangle(class MeshObject* inputParent):
    Primitive<3>(inputParent)
{
}

glm::vec3 Triangle::GetPrimitiveNormal() const
{
    const glm::vec3 edge1 = glm::normalize(positions[1] - positions[0]);
    const glm::vec3 edge2 = glm::normalize(positions[2] - positions[0]);
    return glm::normalize(glm::cross(edge1, edge2));
}

bool Triangle::Trace(const SceneObject* parentObject, Ray* inputRay, IntersectionState* outputIntersection) const
{
    DIAGNOSTICS_STAT(DiagnosticsType::TRIANGLE_INTERSECTIONS);
    assert(parentObject);
    // Convert ray into object space.
    const glm::vec3 rayPos = glm::vec3(parentObject->GetWorldToObjectMatrix() * inputRay->GetPosition());
    const glm::vec3 rayDir = glm::vec3(parentObject->GetWorldToObjectMatrix() * inputRay->GetForwardDirection());

    // Use Moller-Trumbore Intersection (Fast, Minimum Storage Ray/Triangle Intersection)
    // Paper: http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
    const glm::vec3 edge1 = positions[1] - positions[0];
    const glm::vec3 edge2 = positions[2] - positions[0];
    const glm::vec3 pvec = glm::cross(rayDir, edge2);

    float det = glm::dot(edge1, pvec);

    if (det > -SMALL_EPSILON && det < SMALL_EPSILON) {
        return false;
    }

    const float invDet = 1.f / det;

    const glm::vec3 tvec = glm::vec3(rayPos) - positions[0];
    const float u = glm::dot(tvec, pvec) * invDet;
    if (u < 0.f || u > 1.f) {
        return false;
    }

    const glm::vec3 qvec = glm::cross(tvec, edge1);
    const float v = glm::dot(rayDir, qvec) * invDet;
    if (v < 0.f || u + v > 1.f) {
        return false;
    }

    const float t = glm::dot(edge2, qvec) * invDet;
    if (t - inputRay->GetMaxT() > SMALL_EPSILON || t < -SMALL_EPSILON) {
        return false;
    }

    if (outputIntersection) {
        if (t - outputIntersection->intersectionT > SMALL_EPSILON) {
            return false;
        }
        outputIntersection->intersectionRay = *inputRay;
        outputIntersection->primitiveParent = parentObject;
        outputIntersection->intersectionT = t;
        outputIntersection->intersectedPrimitive = this;
        outputIntersection->hasIntersection = true;

        outputIntersection->primitiveIntersectionWeights.clear();
        outputIntersection->primitiveIntersectionWeights.emplace_back(1.f - u - v);
        outputIntersection->primitiveIntersectionWeights.emplace_back(u);
        outputIntersection->primitiveIntersectionWeights.emplace_back(v);
    }

    return true;
}