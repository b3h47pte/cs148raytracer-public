#pragma once

#include "common/common.h"
#include "common/Scene/Geometry/Ray/Ray.h"

struct Photon
{
    typedef float value_type;
    glm::vec3 position;
    glm::vec3 intensity;
    Ray toLightRay;
    
    inline value_type operator[](size_t const N) const { return position[static_cast<int>(N)]; }
};

struct PhotonAccessor
{
    typedef float result_type;
    float operator()(const Photon& t, size_t k) const { return t[k]; }
};