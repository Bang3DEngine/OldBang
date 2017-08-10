#ifndef DISTANCEFIELDER_H
#define DISTANCEFIELDER_H

#include "Bang/G_Image.h"

class DistanceFielder
{
public:

    // Black:  background
    // White: foreground
    static void CreateDistanceField(const G_Image &inputImageBW,
                                    G_Image *distanceFieldOutputImage,
                                    int distanceRadius);
    DistanceFielder() = delete;
};

#endif // DISTANCEFIELDER_H
