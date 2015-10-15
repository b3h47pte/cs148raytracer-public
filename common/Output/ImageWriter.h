#pragma once

#include "common/common.h"
#include "FreeImage.h"
// Image Writer Class
// Use the FreeImage library to write an image to a file
// Assume (0, 0) is the top left of the image.
class ImageWriter
{
public:
    ImageWriter(std::string, int, int);
    ~ImageWriter();

    glm::vec3 GetHDRPixelColor(int inX, int inY) const;
    // this function will stored in a float array to support HDR.
    void SetPixelColor(glm::vec3, int, int);

    void CopyHDRToBitmap();
    // Assume color will be passed in as a 0-1 float
    void SetFinalPixelColor(glm::vec3, int, int);

    // Explicit Call to Finish and Save File -- Otherwise done at destructor
    void SaveImage();

private:
    // File name that we want to output to
    std::string m_sFileName;
    int mWidth;
    int mHeight;

    // Float data
    glm::vec3* mHDRData;

    // Bitmap file
    FIBITMAP*	m_pOutBitmap;
};
