#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include "NamedEnum.h"
#include "GLObject.h"

class Texture : public GLObject
{
public:
    enum Target
    {
        Texture1D = GL_TEXTURE_1D,
        Texture2D = GL_TEXTURE_2D,
        Texture3D = GL_TEXTURE_3D,
        TextureCubeMap = GL_TEXTURE_CUBE_MAP
    };

    enum Format
    {
        RGBA_Byte8 = 0,
        RGBA_Float32
    };

    enum FilterMode
    {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        Trilinear = GL_LINEAR_MIPMAP_LINEAR
    };

    enum WrapMode
    {
        Repeat = GL_REPEAT,
        Clamp = GL_CLAMP,
        ClampToEdge = GL_CLAMP_TO_EDGE
    };

    Texture();
    Texture(Target texTarget);
    virtual ~Texture();

    virtual void CreateEmpty(int width, int height) = 0;
    virtual void Resize(int width, int height) = 0;

    void SetTarget(Target target);
    void SetFormat(Format format);
    void SetFilterMode(FilterMode filterMode);
    void SetWrapMode(WrapMode wrapMode);
    void SetTextureUnit(int textureSlot);

    int GetWidth() const;
    int GetHeight() const;
    Format GetFormat() const;
    FilterMode GetFilterMode() const;
    WrapMode GetWrapMode() const;
    int GetTextureUnit() const;

    GLint GetGLDataType() const;
    GLint GetGLFormat() const;
    GLint GetGLInternalFormat() const;
    unsigned int GetBytesSize() const;

    GL::BindTarget GetGLBindTarget() const override;
    void BindToTextureUnit(int textureUnit) const;

protected:

    int m_width = 0;
    int m_height = 0;

    FilterMode m_filterMode = FilterMode::Nearest;
    WrapMode m_wrapMode = WrapMode::Repeat;
    int m_textureUnit = 0;

    unsigned char *m_data = nullptr;

    Format m_format = Format::RGBA_Float32;
    Target m_target = Target::Texture2D;

    static unsigned int GetPixelBytesSize(Format texFormat);
    static GLint GetGLDataTypeFrom(Format texFormat);
    static GLint GetGLFormatFrom(Format texFormat);
    static GLint GetGLInternalFormatFrom(Format texFormat);

private:
    //NON COPYABLE:
    Texture(const Texture &t) {}
};

#endif // TEXTURE_H
