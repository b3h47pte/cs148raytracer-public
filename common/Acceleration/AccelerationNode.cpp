#include "common/Acceleration/AccelerationNode.h"

std::atomic<uint64_t> AccelerationNode::globalIdCount(0);

AccelerationNode::AccelerationNode():
    uniqueId(++globalIdCount)
{
}
