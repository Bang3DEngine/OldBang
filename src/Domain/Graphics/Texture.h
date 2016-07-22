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
    GLint m_glTextureGetIntegerType = 0;

protected:
    GLint m_internalType = GL_UNSIGNED_BYTE;
    GLint m_internalFormat = GL_RGB;
    GLint m_format = GL_RGB;

    int m_width = 0;
    int m_height = 0;
    int m_numComponents = 0;

    FilterMode m_filterMode = FilterMode::Nearest;
    WrapMode m_wrapMode = WrapMode::Repeat;
    int m_textureSlot = 0;

    unsigned char *p_data = nullptr;

    TextureType m_glTextureType = TextureType::Texture2D;

public:
    Texture();
    Texture(TextureType m_glTextureType);
    virtual ~Texture();

    virtual void CreateEmpty(int width, int height) = 0;
    virtual void Resize(int width, int height) = 0;

    void SetGLInternalType(GLint glInternalType);
    void SetGLInternalFormat(GLint glInternalFormat);
    void SetGLFormat(GLint glFormat);
    void SetFilterMode(FilterMode filterMode);
    void SetWrapMode(WrapMode wrapMode);
    void SetTextureSlot(int textureSlot);

    int GetWidth() const;
    int GetHeight() const;
    int GetNumComponents() const;
    GLint GetGLInternalType() const;
    GLint GetGLInternalFormat() const;
    GLint GetGLFormat() const;
    FilterMode GetFilterMode() const;
    WrapMode GetWrapMode() const;
    int GetTextureSlot() const;

    void Bind() const override;
    void UnBind() const override;
};

#endif // TEXTURE_H
