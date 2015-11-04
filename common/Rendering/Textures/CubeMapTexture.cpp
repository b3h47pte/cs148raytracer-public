#include "common/Rendering/Textures/CubeMapTexture.h"
#include "common/Rendering/Textures/Texture2D.h"

CubeMapTexture::CubeMapTexture(unsigned char* data[6], int width, int height):
    Texture(), texWidth(width), texHeight(height)
{
    for (int i = 0; i < 6; ++i) {
        cubeTextures[i] = std::make_shared<Texture2D>(data[i], width, height);
    }
}


glm::vec4 CubeMapTexture::Sample(const glm::vec2& coord) const
{
    return Sample(glm::vec3(coord, 0.f));
}

glm::vec4 CubeMapTexture::Sample(const glm::vec3& coord) const
{
    return glm::vec4();
}