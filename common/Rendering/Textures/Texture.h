#pragma once

#include "common/common.h"

class Texture: public std::enable_shared_from_this<Texture>
{
public:
    Texture();
    virtual ~Texture();

    virtual glm::vec4 Sample(const glm::vec2& coord) const = 0;
    virtual glm::vec4 Sample(const glm::vec3& coord) const = 0;
};
