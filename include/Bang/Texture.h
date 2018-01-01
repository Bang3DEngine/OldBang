#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include "Bang/Vector2.h"
#include "Bang/GLObject.h"

NAMESPACE_BANG_BEGIN

class Texture : public GLObject
{
public:

    Texture();
    Texture(GL_TextureTarget texTarget);
    Texture(const Texture &t);
    virtual ~Texture();

    virtual void CreateEmpty(int width, int height) = 0;
    virtual void Resize(int width, int height) = 0;

    void SetInternalFormat(GL_ColorFormat internalFormat);
    void SetTarget(GL_TextureTarget target);
    void SetFilterMode(GL_FilterMode filterMode);
    void SetWrapMode(GL_WrapMode wrapMode);
    void SetTextureUnit(int textureSlot);

    int GetWidth() const;
    int GetHeight() const;
    const Vector2i& GetSize() const;
    GL_FilterMode GetFilterMode() const;
    GL_WrapMode GetWrapMode() const;
    int GetTextureUnit() const;

    GL_DataType GetInternalDataType() const;
    GL_ColorFormat GetInternalFormat() const;
    uint GetBytesSize() const;

    GL_TextureTarget GetTextureTarget() const;
    GL_BindTarget GetGLBindTarget() const override;
    void BindToTextureUnit(int textureUnit) const;

protected:
    void SetWidth(int width);
    void SetHeight(int height);

private:
    Vector2i m_size = Vector2i::Zero;

    GL_FilterMode m_filterMode = Undef<GL_FilterMode>();
    GL_WrapMode m_wrapMode     = Undef<GL_WrapMode>();
    int m_textureUnit = 0;

    GL_ColorFormat m_internalFormat = GL_ColorFormat::RGBA_Float32;
    GL_TextureTarget m_target = GL_TextureTarget::Texture2D;
};

NAMESPACE_BANG_END

#endif // TEXTURE_H
