#pragma once

#include "common/common.h"

struct Photon
{
    typedef float value_type;
    glm::vec3 position;
    
    inline value_type operator[](size_t const N) const { return position[static_cast<int>(N)]; }
};

struct PhotonAccessor
{
    typedef float result_type;
    float operator()(Photon& t, size_t k) const { return t[k]; }
};