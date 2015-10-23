#include "common/Sampling/Jitter/JitterSampler.h"

JitterSampler::JitterSampler():
    maxSamples(0), samplesPerCell(0), processedSamples(0)
{
}

void JitterSampler::Initialize(const glm::ivec3 inputGridSize, int inputMaxSamples)
{
    gridSize = inputGridSize;
    maxSamples = inputMaxSamples;
    samplesPerCell = inputMaxSamples / (gridSize.x * gridSize.y * gridSize.z);
    assert(samplesPerCell > 0);
    ResetSamplerState();
}

void JitterSampler::ResetSamplerState()
{
    internalSampler.ResetSamplerState();
    processedSamples = 0;
}

int JitterSampler::GetCurrentCell() const
{
    // Returns a linear index
    return processedSamples / samplesPerCell;
}

float JitterSampler::Sample1D(float rangeMin, float rangeMax)
{
    assert(false);
    return 0.f;
}

glm::vec2 JitterSampler::Sample2D(glm::vec2 rangeMin, glm::vec2 rangeMax)
{
    const int linearIndex = GetCurrentCell();
    // X (columns) first then Y (rows).
    const glm::vec2 gridCellIndex = glm::vec2(linearIndex % gridSize.x, linearIndex / gridSize.x);
    const glm::vec2 gridCellSize = (rangeMax - rangeMin) / glm::vec2(gridSize.x, gridSize.y);
    const glm::vec2 gridCellOffset = gridCellIndex * gridCellSize;
    return internalSampler.Sample2D(rangeMin + gridCellOffset, rangeMin + gridCellOffset + gridCellSize);
}

glm::vec3 JitterSampler::Sample3D(glm::vec3 rangeMin, glm::vec3 rangeMax)
{
    assert(false);
    return glm::vec3();
}

void JitterSampler::FinishSample()
{
    ++processedSamples;
}