#ifndef IMAGEIODDS_H
#define IMAGEIODDS_H

#include "Bang/Path.h"
#include "Bang/Texture2D.h"

NAMESPACE_BANG_BEGIN

class ImageIODDS
{
public:
    static void ImportDDS(const Path &filepath, Texture2D *tex, bool *_ok);

    ImageIODDS() = delete;
};

NAMESPACE_BANG_END

#endif // IMAGEIODDS_H

