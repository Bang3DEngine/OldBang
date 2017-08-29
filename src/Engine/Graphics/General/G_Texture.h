#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include "Bang/GLObject.h"

class G_Texture : public GLObject
{
public:

    G_Texture();
    G_Texture(GL::TextureTarget texTarget);
    G_Texture(const G_Texture &t);
    virtual ~G_Texture();

    virtual void CreateEmpty(int width, int height) = 0;
    virtual void Resize(int width, int height) = 0;

    void SetInternalFormat(GL::ColorInternalFormat internalFormat);
    void SetTarget(GL::TextureTarget target);
    void SetFilterMode(GL::FilterMode filterMode);
    void SetWrapMode(GL::WrapMode wrapMode);
    void SetTextureUnit(int textureSlot);

    int GetWidth() const;
    int GetHeight() const;
    GL::FilterMode GetFilterMode() const;
    GL::WrapMode GetWrapMode() const;
    int GetTextureUnit() const;

    GL::DataType GetInternalDataType() const;
    GL::ColorInternalFormat GetInternalFormat() const;
    uint GetBytesSize() const;

    GL::BindTarget GetGLBindTarget() const override;
    void BindToTextureUnit(int textureUnit) const;

protected:

    int m_width = 0;
    int m_height = 0;

    GL::FilterMode m_filterMode = GL::FilterMode::Nearest;
    GL::WrapMode m_wrapMode     = GL::WrapMode::Repeat;
    int m_textureUnit = 0;

    Byte *m_data = nullptr;

    GL::ColorInternalFormat m_internalFormat = GL::ColorInternalFormat::RGBA_Float32;
    GL::TextureTarget m_target = GL::TextureTarget::Texture2D;
};

#endif // TEXTURE_H
