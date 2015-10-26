#include "common/Scene/Geometry/Simple/Box/Box.h"
#include "common/Scene/SceneObject.h"
#include "common/Scene/Geometry/Ray/Ray.h"
#include "common/Intersection/IntersectionState.h"

Box::Box() :
    minVertex(std::numeric_limits<float>::max()), maxVertex(std::numeric_limits<float>::lowest())
{
}

Box::Box(glm::vec3 minV, glm::vec3 maxV):
    minVertex(minV), maxVertex(maxV)
{
}

void Box::Reset()
{
    minVertex = glm::vec3(std::numeric_limits<float>::max());
    maxVertex = glm::vec3(std::numeric_limits<float>::lowest());
}

void Box::IncludeBox(const Box& box)
{
    minVertex = glm::min(minVertex, box.minVertex);
    maxVertex = glm::max(maxVertex, box.maxVertex);
}

glm::vec3 Box::Center() const
{
    return 0.5f * (minVertex + maxVertex);
}

bool Box::Trace(const class SceneObject* parentObject, class Ray* inputRay, struct IntersectionState* outputIntersection) const
{
    DIAGNOSTICS_STAT(DiagnosticsType::BOX_INTERSECTIONS);
    glm::mat4 spaceTransform(1.f);
    if (parentObject) {
        spaceTransform = parentObject->GetWorldToObjectMatrix();
    }

    // Convert ray into object space.
    const glm::vec3 rayPos = glm::vec3(spaceTransform * inputRay->GetPosition());
    const glm::vec3 rayDir = glm::vec3(spaceTransform * inputRay->GetForwardDirection());

    float globalMinT = std::numeric_limits<float>::lowest();
    float globalMaxT = std::numeric_limits<float>::max();

    // Do intersection against slabs in the X, Y, and then Z direction. Make sure we are within the slabs for all three axes.
    bool didIntersect = false;
    for (int i = 0; i < 3; ++i) {
        if (std::abs(rayDir[i]) < SMALL_EPSILON) {
            // If we're not moving in this direction then we should already be within the specified by range.
            if (rayPos[i] - minVertex[i] < SMALL_EPSILON || rayPos[i] - maxVertex[i] > SMALL_EPSILON) {
                return false;
            }

            continue;
        }

        float dimMinT = (minVertex[i] - rayPos[i]) / rayDir[i];
        float dimMaxT = (maxVertex[i] - rayPos[i]) / rayDir[i];

        if (dimMaxT - dimMinT < SMALL_EPSILON) {
            std::swap(dimMinT, dimMaxT);
        }

        if (i > 0 && (dimMinT - globalMaxT > SMALL_EPSILON || globalMinT - dimMaxT > SMALL_EPSILON)) {
            return false;
        }

        globalMinT = std::max(globalMinT, dimMinT);
        globalMaxT = std::min(globalMaxT, dimMaxT);
        didIntersect = true;
    }

    if (!didIntersect) {
        return false;
    }

    if (globalMinT - inputRay->GetMaxT() > SMALL_EPSILON || globalMaxT < SMALL_EPSILON) {
        return false;
    }
    return true;
}