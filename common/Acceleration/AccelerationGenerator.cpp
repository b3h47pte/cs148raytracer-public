#include "common/Acceleration/AccelerationGenerator.h"
#include "common/Acceleration/AccelerationCommon.h"

namespace AccelerationGenerator
{
    std::unique_ptr<AccelerationStructure> CreateStructureFromType(AccelerationTypes inputType)
    {
        std::unique_ptr<AccelerationStructure> acceleration;
        switch (inputType)  {
            case AccelerationTypes::NONE:
                acceleration = make_unique<NaiveAcceleration>();
                break;
            case AccelerationTypes::BVH:
                acceleration = make_unique<BVHAcceleration>();
                break;
            case AccelerationTypes::UNIFORM_GRID:
                acceleration = make_unique<UniformGridAcceleration>();
                break;
            default:
                throw std::runtime_error("ERROR: Unsupported acceleration structure.");
                break;
        }
        return std::move(acceleration);
    }
}