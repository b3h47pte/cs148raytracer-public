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

    //std::cout << "Trace Box Ray: " << glm::to_string(rayPos) << " " << glm::to_string(rayDir) << std::endl;
    //std::cout << "  Box: " << glm::to_string(minVertex) << " " << glm::to_string(maxVertex) << std::endl;

    float globalMinT = std::numeric_limits<float>::lowest();
    float globalMaxT = std::numeric_limits<float>::max();

    // Do intersection against slabs in the X, Y, and then Z direction. Make sure we are within the slabs for all three axes.
    int usedDimensions = 0;
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

        if (usedDimensions > 0 && (dimMinT - globalMaxT > SMALL_EPSILON || globalMinT - dimMaxT > SMALL_EPSILON)) {
            return false;
        }

        globalMinT = std::max(globalMinT, dimMinT);
        globalMaxT = std::min(globalMaxT, dimMaxT);
        didIntersect = true;
        ++usedDimensions;
    }

    if (!didIntersect) {
        return false;
    }

    if (globalMinT - inputRay->GetMaxT() > SMALL_EPSILON || globalMaxT < SMALL_EPSILON) {
        return false;
    }

    // WARNING: Ray-Box intersection isn't as well supported as ray-triangle intersection. This bit is kinda hacky atm.
    if (outputIntersection) {
        if (globalMinT - outputIntersection->intersectionT > SMALL_EPSILON) {
            return false;
        }
        outputIntersection->intersectionT = (globalMinT > SMALL_EPSILON) ? globalMinT : globalMaxT;
    }

    return true;
}

Box Box::Expand(float delta) const
{
    Box newBoundingBox;
    glm::vec3 diagonal = glm::normalize(maxVertex - minVertex);
    newBoundingBox.maxVertex = maxVertex + delta * diagonal;
    newBoundingBox.minVertex = minVertex - delta * diagonal;
    return newBoundingBox;
}

Box Box::Transform(glm::mat4 transformation) const
{
    Box newBoundingBox;
    std::vector<glm::vec3> corners;
    Corners(corners);

    for (size_t i = 0; i < corners.size(); ++i) {
        glm::vec3 transformedCorner = glm::vec3(transformation * glm::vec4(corners[i], 1.f));
        newBoundingBox.minVertex = glm::min(transformedCorner, newBoundingBox.minVertex);
        newBoundingBox.maxVertex = glm::max(transformedCorner, newBoundingBox.maxVertex);
    }

    return newBoundingBox;
}

void Box::Corners(std::vector<glm::vec3>& corners) const
{
    corners.clear();
    for (int i = 0; i < 2; ++i) {
        float x = (i == 0) ? minVertex.x : maxVertex.x;
        for (int j = 0; j < 2; ++j) {
            float y = (j == 0) ? minVertex.y : maxVertex.y;
            for (int k = 0; k < 2; ++k) {
                float z = (k == 0) ? minVertex.z : maxVertex.z;
                corners.emplace_back(x, y, z);
            }
        }
    }
}

float Box::Volume() const
{
    glm::vec3 diagonal = maxVertex - minVertex;
    return diagonal[0] * diagonal[1] * diagonal[2];
}