#ifndef IMAGEEFFECTS_H
#define IMAGEEFFECTS_H

#include "Bang/G_Image.h"

class ImageEffects
{
public:

    // Zero  Color (0,0,0,0): background
    // Other Color:           foreground
    static void SignedDistanceField(const G_Image &inputImageBW,
                                    G_Image *distanceFieldOutputImage,
                                    int distanceRadius);

    // Zero  Color (0,0,0,0): background
    // Other Color:           foreground
    static bool Outline(const G_Image &inputImageBW,
                        G_Image *outlineOutputImageBW);

    static bool Diff(const G_Image &inputImage1,
                     const G_Image &inputImage2,
                     G_Image *diffOutputImage,
                     bool diffAlpha = false);

    ImageEffects() = delete;
};

#endif // DISTANCEFIELDER_H
