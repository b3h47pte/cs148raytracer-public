#include "common/Output/ImageWriter.h"
#include <locale>

using namespace std;

// Ctor/Dtor
ImageWriter::ImageWriter(std::string inFile, int inWidth, int inHeight) : mWidth(inWidth), mHeight(inHeight)
{
    // Initialize Free Image and get it ready to do stuff
    FreeImage_Initialise();

    // Create Bitmap and check if it's valid
    // Hard-code bits per pixel to 24 for now since we're just doing RBG (no alpha)
    m_pOutBitmap = FreeImage_Allocate(mWidth, mHeight, 24);
    mHDRData = new glm::vec3[mWidth * mHeight];

    if (!m_pOutBitmap) {
        throw std::runtime_error("ERROR: Bitmap failed to initialize.");
        return;
    }

    m_sFileName = inFile;
}

ImageWriter::~ImageWriter()
{
    delete[] mHDRData;
    FreeImage_DeInitialise();
}

glm::vec3 ImageWriter::GetHDRPixelColor(int inX, int inY) const
{
    int linearIdx = inY * mWidth + inX;
    return mHDRData[linearIdx];
}

void ImageWriter::SetPixelColor(glm::vec3 inColor, int inX, int inY)
{
    int linearIdx = inY * mWidth + inX;
    mHDRData[linearIdx] = inColor;
}

void ImageWriter::CopyHDRToBitmap()
{
    for (int x = 0; x < mWidth; ++x) {
        for (int y = 0; y < mHeight; ++y) {
            int linearIdx = y * mWidth + x;
            SetFinalPixelColor(mHDRData[linearIdx], x, y);
        }
    }
}

// Simple Call to Set Pixel Color
void ImageWriter::SetFinalPixelColor(glm::vec3 inColor, int inX, int inY)
{
    RGBQUAD color;
    color.rgbRed = (BYTE)std::max(std::min(inColor[0] * 255.0, 255.0), 0.0);
    color.rgbGreen = (BYTE)std::max(std::min(inColor[1] * 255.0, 255.0), 0.0);
    color.rgbBlue = (BYTE)std::max(std::min(inColor[2] * 255.0, 255.0), 0.0);

    FreeImage_SetPixelColor(m_pOutBitmap, inX, mHeight - inY - 1, &color);
}

// Manual call to save file 
void ImageWriter::SaveImage()
{
    FREE_IMAGE_FORMAT fm = FIF_JPEG;
    if (m_pOutBitmap == NULL)
        return;

    // Determine extension provided and save properly
    // Determine last period (right before the extension -- if none, default to JPEG)
    size_t indx = m_sFileName.find_last_of(".");
    if (indx != string::npos) {
        locale loc;
        string sub = m_sFileName.substr(indx + 1);
        for (size_t i = 0; i < sub.length(); i++) {
            sub[i] = toupper(sub[i], loc);
        }

        if (sub == "JPG" || sub == "JPEG")
            fm = FIF_JPEG;
        else if (sub == "BMP")
            fm = FIF_BMP;
        else if (sub == "PNG")
            fm = FIF_PNG;
        else {
            m_sFileName = m_sFileName.replace(indx + 1, sub.length(), "jpg");
        }
    }

    if (FreeImage_Save(fm, m_pOutBitmap, m_sFileName.c_str(), 0)) {
        // At this point we have saved successfully
        // Make sure m_pOutBitmap is NULL so we don't try to save it again
        m_pOutBitmap = NULL;
    }
}
