#ifndef TEXTURERENDER_H
#define TEXTURERENDER_H

#include "Bang/Texture2D.h"

class RenderTexture : public Texture2D
{

public:
    RenderTexture();
    virtual ~RenderTexture();

    /*
    void CreateEmpty(int width, int height) override;
    void Resize(int width, int height) override;
    void Fill(unsigned char *newData, int width, int height);
    */
};

#endif // TEXTURERENDER_H
