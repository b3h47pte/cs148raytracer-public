#pragma once

#include "common/common.h"

enum class AccelerationTypes;
class AccelerationStructure;

namespace AccelerationGenerator
{
    std::unique_ptr<AccelerationStructure> CreateStructureFromType(AccelerationTypes inputType);
}