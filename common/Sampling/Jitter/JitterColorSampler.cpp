#include "common/Sampling/Jitter/JitterColorSampler.h"

void JitterColorSampler::SetGridSize(glm::ivec3 inputGridSize)
{
    assert(inputGridSize.x * inputGridSize.y * inputGridSize.z > 0);
    gridSize = inputGridSize;
}

glm::vec3 JitterColorSampler::ComputeSampleCoordinate(SamplerState& state) const
{
    const int cellIndex = state.samplesComputed % (gridSize.x * gridSize.y * gridSize.z);
    const glm::vec3 gridCellIndex = glm::vec3(cellIndex % gridSize.x, (cellIndex / gridSize.x) % gridSize.y, cellIndex / (gridSize.x * gridSize.y));
    const glm::vec3 gridCellSize = glm::vec3(1.f) / glm::vec3(gridSize.x, gridSize.y, gridSize.z);
    const glm::vec3 gridCellOffset = gridCellIndex * gridCellSize;

    const glm::vec3 random = ColorSampler::ComputeSampleCoordinate(state);
    return gridCellOffset + gridCellSize * random;
}

std::unique_ptr<SamplerState> JitterColorSampler::CreateSampler(std::random_device& randomDevice, const int maxSamples, const int dimensions) const
{
    std::unique_ptr<JitterSamplerState> state = make_unique<JitterSamplerState>(randomDevice, maxSamples, dimensions);
    state->samplesPerCell = maxSamples / (gridSize.x * gridSize.y * gridSize.z);
    assert(state->samplesPerCell > 0);
    return std::move(state);
}