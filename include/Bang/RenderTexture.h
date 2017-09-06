#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include "Bang/Texture2D.h"

NAMESPACE_BANG_BEGIN

class RenderTexture : public Texture2D
{
public:
    RenderTexture();
    virtual ~RenderTexture();
};

NAMESPACE_BANG_END

#endif // RENDERTEXTURE_H
