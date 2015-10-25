#include "common/Application.h"
#include "common/Acceleration/AccelerationCommon.h"
#include "common/Output/ImageWriter.h"

std::string Application::GetOutputFilename() const
{
    return "output.png";
}

int Application::GetSamplesPerPixel() const
{
    return 16;
}

glm::vec2 Application::GetImageOutputResolution() const
{
    return glm::vec2(1280.f, 720.f);
}

void Application::PerformImagePostprocessing(class ImageWriter&)
{
}