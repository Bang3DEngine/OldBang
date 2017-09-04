#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include "Bang/Vector2.h"
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

    void SetInternalFormat(GL::ColorFormat internalFormat);
    void SetTarget(GL::TextureTarget target);
    void SetFilterMode(GL::FilterMode filterMode);
    void SetWrapMode(GL::WrapMode wrapMode);
    void SetTextureUnit(int textureSlot);

    int GetWidth() const;
    int GetHeight() const;
    const Vector2i& GetSize() const;
    GL::FilterMode GetFilterMode() const;
    GL::WrapMode GetWrapMode() const;
    int GetTextureUnit() const;

    GL::DataType GetInternalDataType() const;
    GL::ColorFormat GetInternalFormat() const;
    uint GetBytesSize() const;

    GL::TextureTarget GetTextureTarget() const;
    GL::BindTarget GetGLBindTarget() const override;
    void BindToTextureUnit(int textureUnit) const;

protected:
    void SetWidth(int width);
    void SetHeight(int height);

private:
    Vector2i m_size = Vector2i::Zero;

    GL::FilterMode m_filterMode = GL::FilterMode::Nearest;
    GL::WrapMode m_wrapMode     = GL::WrapMode::Repeat;
    int m_textureUnit = 0;

    GL::ColorFormat m_internalFormat = GL::ColorFormat::RGBA_Float32;
    GL::TextureTarget m_target = GL::TextureTarget::Texture2D;
};

#endif // TEXTURE_H
