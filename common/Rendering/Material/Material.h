#pragma once

#include "common/common.h"

class Material: public std::enable_shared_from_this<Material>
{
public:
    virtual ~Material();
};