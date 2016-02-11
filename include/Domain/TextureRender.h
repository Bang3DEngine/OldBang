#ifndef TEXTURERENDER_H
#define TEXTURERENDER_H

#include "Texture.h"

class TextureRender : public Texture
{

public:
    TextureRender();
    virtual ~TextureRender();

    void CreateEmpty(int width, int height) override;
};

#endif // TEXTURERENDER_H
