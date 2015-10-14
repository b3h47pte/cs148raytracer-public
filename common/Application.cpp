#include "common/Application.h"
#include "common/Acceleration/AccelerationCommon.h"
#include "common/Output/ImageWriter.h"

AccelerationTypes Application::GetSceneAccelerationType() const
{
    return AccelerationTypes::NONE;
}

AccelerationTypes Application::GetPerObjectAccelerationType() const
{
    return AccelerationTypes::NONE;
}

std::string Application::GetOutputFilename() const
{
    return "output.png";
}

int Application::GetSamplesPerPixel() const
{
    return 1;
}

void Application::PerformImagePostprocessing(class ImageWriter&)
{
}