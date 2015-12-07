#include "common/Rendering/Textures/Texture2D.h"

Texture2D::Texture2D(unsigned char* rawData, int width, int height):
    Texture(), textureData(rawData), texWidth(width), texHeight(height)
{
}

Texture2D::~Texture2D()
{
    delete[] textureData;
}

glm::vec4 Texture2D::Sample(const glm::vec2& coord) const
{
    const glm::vec2 imageSpaceCoordinates = coord * glm::vec2(texWidth, texHeight);

    glm::vec2 floorVec(std::floor(imageSpaceCoordinates.x), std::floor(imageSpaceCoordinates.y));
    glm::vec2 ceilVec(floorVec.x + 1.f, floorVec.y + 1.f);

    // Bilinear Interpolation
    const glm::ivec2 q11(floorVec);
    const glm::ivec2 q12(floorVec.x, ceilVec.y);
    const glm::ivec2 q21(ceilVec.x, floorVec.y);
    const glm::ivec2 q22(ceilVec);

    const glm::vec4 fx1 = (ceilVec.x - imageSpaceCoordinates.x) * InternalSample(q11) + (imageSpaceCoordinates.x - floorVec.x) * InternalSample(q21);
    const glm::vec4 fx2 = (ceilVec.x - imageSpaceCoordinates.x) * InternalSample(q12) + (imageSpaceCoordinates.x - floorVec.x) * InternalSample(q22);
    return (ceilVec.y - imageSpaceCoordinates.y) * fx1 + (imageSpaceCoordinates.y - floorVec.y) * fx2;
}

glm::ivec2 Texture2D::HandleBorderCondition(const glm::ivec2& coord) const
{
    // By default, do repeat across borders
    glm::ivec2 result = coord;
    if (result.x < 0) {
        result.x = texWidth + result.x % texWidth;
    }

    if (result.y < 0) {
        result.y = texHeight + result.y % texHeight;
    }

    result.x = result.x % texWidth;
    result.y = result.y % texHeight;
    return result;
}

glm::vec4 Texture2D::InternalSample(const glm::ivec2& coord) const
{
    int index = ComputeLinearIndex(HandleBorderCondition(coord));
    glm::vec4 result = glm::vec4(textureData[index], textureData[index + 1], textureData[index + 2], textureData[index + 3]) / 255.f;
    return result;
}

glm::vec4 Texture2D::Sample(const glm::vec3& coord) const
{
    return Sample(glm::vec2(coord));
}

int Texture2D::ComputeLinearIndex(const glm::ivec2& pixel) const
{
    return (pixel.x + pixel.y * texWidth) * 4;
}
