#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include "Bang/Vector2.h"
#include "Bang/GLObject.h"
#include "Bang/IEventEmitter.h"
#include "Bang/ITextureChangedListener.h"

NAMESPACE_BANG_BEGIN

class Texture : public GLObject,
                public EventEmitter<ITextureChangedListener>
{
public:

    Texture();
    Texture(GL::TextureTarget texTarget);
    Texture(const Texture &t);
    virtual ~Texture();

    virtual void CreateEmpty(int width, int height) = 0;
    virtual void Resize(int width, int height) = 0;

    void SetFormat(GL::ColorFormat internalFormat);
    void SetTarget(GL::TextureTarget target);
    void SetFilterMode(GL::FilterMode filterMode);
    void SetWrapMode(GL::WrapMode wrapMode);

    int GetWidth() const;
    int GetHeight() const;
    const Vector2i& GetSize() const;
    GL::FilterMode GetFilterMode() const;
    GL::WrapMode GetWrapMode() const;

    GL::DataType GetDataType() const;
    GL::ColorComp GetColorComp() const;
    GL::ColorFormat GetFormat() const;
    uint GetBytesSize() const;

    GL::TextureTarget GetTextureTarget() const;
    GL::BindTarget GetGLBindTarget() const override;

protected:
    void SetWidth(int width);
    void SetHeight(int height);

    void PropagateTextureChanged();

private:
    Vector2i m_size = Vector2i::Zero;

    GL::FilterMode m_filterMode = Undef<GL::FilterMode>();
    GL::WrapMode m_wrapMode     = Undef<GL::WrapMode>();

    GL::ColorFormat m_glFormat = GL::ColorFormat::RGBA_Float32;
    GL::TextureTarget m_target = GL::TextureTarget::Texture2D;
};

NAMESPACE_BANG_END

#endif // TEXTURE_H
