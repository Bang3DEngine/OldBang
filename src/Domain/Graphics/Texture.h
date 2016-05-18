#ifndef TEXTURE_H
#define TEXTURE_H

#include "Bang.h"

#include <GL/glew.h>

#include "IGLBindable.h"
#include "IGLIdable.h"

class Texture : public IGLBindable, public IGLIdable
{

private:
    //NON COPYABLE:
    Texture(const Texture &t) {}

public:
    enum TextureType
    {
        Texture1D = GL_TEXTURE_1D,
        Texture2D = GL_TEXTURE_2D,
        Texture3D = GL_TEXTURE_3D,
        TextureCubeMap = GL_TEXTURE_CUBE_MAP,
    };

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

private:
    GLint glTextureGetIntegerType = 0;

protected:
    int width = 0;
    int height = 0;
    int numComponents = 0;

    FilterMode filterMode = FilterMode::Nearest;
    WrapMode wrapMode = WrapMode::Repeat;
    int textureSlot = 0;

    unsigned char *data = nullptr;

    TextureType glTextureType = TextureType::Texture2D;

public:
    Texture();
    Texture(TextureType glTextureType);
    virtual ~Texture();

    virtual void CreateEmpty(int width, int height) = 0;
    virtual void Resize(int width, int height) = 0;

    void SetFilterMode(FilterMode filterMode);
    void SetWrapMode(WrapMode wrapMode);
    void SetTextureSlot(int textureSlot);

    int GetWidth() const;
    int GetHeight() const;
    int GetNumComponents() const;
    FilterMode GetFilterMode() const;
    WrapMode GetWrapMode() const;
    int GetTextureSlot() const;

    void Bind() const override;
    void UnBind() const override;
};

#endif // TEXTURE_H
