#ifndef IMAGEEFFECTS_H
#define IMAGEEFFECTS_H

#include "Bang/G_Image.h"

class ImageEffects
{
public:

    // Black:  background
    // White: foreground
    static void CreateDistanceField(const G_Image &inputImageBW,
                                    G_Image *distanceFieldOutputImage,
                                    int distanceRadius);

    static bool Diff(const G_Image &inputImage1,
                     const G_Image &inputImage2,
                     G_Image *diffOutputImage,
                     bool diffAlpha = false);

    ImageEffects() = delete;
};

#endif // DISTANCEFIELDER_H
