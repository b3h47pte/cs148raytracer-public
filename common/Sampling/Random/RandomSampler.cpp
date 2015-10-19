#include "common/Sampling/Random/RandomSampler.h"

void RandomSampler::ResetSamplerState()
{
}

float RandomSampler::Sample1D(float rangeMin, float rangeMax)
{
    assert(std::abs(rangeMax - rangeMin) >= EPSILON);
    static std::mt19937 gen(randomDevice());
    static std::uniform_real_distribution<> dist(0, 1);

    float result = rangeMin + (rangeMax - rangeMin) * static_cast<float>(dist(gen));
    return result;
}

glm::vec2 RandomSampler::Sample2D(glm::vec2 rangeMin, glm::vec2 rangeMax)
{
    glm::vec2 result;
    for (int i = 0; i < 2; ++i) {
        result[i] = Sample1D(rangeMin[i], rangeMax[i]);
    }
    return result;
}

glm::vec3 RandomSampler::Sample3D(glm::vec3 rangeMin, glm::vec3 rangeMax)
{
    glm::vec3 result;
    for (int i = 0; i < 3; ++i) {
        result[i] = Sample1D(rangeMin[i], rangeMax[i]);
    }
    return result;
}