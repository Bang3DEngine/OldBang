#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include "GLBindable.h"
#include "GLIdable.h"

class Texture : public GLBindable, public GLIdable
{
public:
    enum FilterMode
    {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR
    };

    enum WrapMode
    {
        Repeat = GL_REPEAT,
        Clamp = GL_CLAMP
    };

protected:
    int width;
    int height;
    int numComponents;

    FilterMode filterMode;
    WrapMode wrapMode;
    int textureSlot;

    unsigned char *rgbs;

    GLint glTextureTarget;
    GLint glTextureTargetGetInteger;

public:
    Texture(GLint glTextureTarget);
    virtual ~Texture();

    virtual void CreateEmpty(int width, int height) = 0;

    void SetFilterMode(FilterMode filterMode);
    void SetWrapMode(WrapMode wrapMode);
    void SetTextureSlot(int textureSlot);

    FilterMode GetFilterMode() const;
    WrapMode GetWrapMode() const;
    int GetTextureSlot() const;

    void Bind() const override;
    void UnBind() const override;
};

#endif // TEXTURE_H
