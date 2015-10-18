#include "common/Scene/Geometry/Simple/Box/Box.h"

Box::Box() :
    minVertex(0.f), maxVertex(0.f)
{
}

Box::Box(glm::vec3 minV, glm::vec3 maxV):
    minVertex(minV), maxVertex(maxV)
{
}

void Box::Reset()
{
    minVertex = glm::vec3(0.f);
    maxVertex = glm::vec3(0.f);
}

void Box::IncludeBox(const Box& box)
{
    minVertex = glm::min(minVertex, box.minVertex);
    maxVertex = glm::max(maxVertex, box.maxVertex);
}