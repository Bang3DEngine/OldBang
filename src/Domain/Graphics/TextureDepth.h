#ifndef TEXTUREDEPTH_H
#define TEXTUREDEPTH_H

#include "Bang.h"

#include "Texture.h"

class TextureDepth : public Texture
{
public:
    TextureDepth();
    virtual ~TextureDepth();

    void CreateEmpty(int width, int height) override;
    void Resize(int width, int height) override;
};

#endif // TEXTUREDEPTH_H
