#include "Bang/Texture.h"

#include "Bang/GL.h"

USING_NAMESPACE_BANG

Texture::Texture()
{
    GL::GenTextures(1, &m_idGL);
    SetFilterMode(GL::FilterMode::Bilinear);
    SetWrapMode(GL::WrapMode::ClampToEdge);
    SetInternalFormat(m_internalFormat);
}

Texture::~Texture()
{
    GL::DeleteTextures(1, &m_idGL);
}

Texture::Texture(GL::TextureTarget texTarget) : Texture()
{
    m_target = texTarget;
}

Texture::Texture(const Texture &t) : GLObject(t)
{
    m_size = Vector2i(t.GetWidth(), t.GetHeight());
    m_filterMode = t.m_filterMode;
    m_wrapMode = t.m_wrapMode;
    m_internalFormat = t.m_internalFormat;
    m_target = t.m_target;
}

void Texture::SetInternalFormat(GL::ColorFormat internalFormat)
{
    m_internalFormat = internalFormat;
}

void Texture::SetTarget(GL::TextureTarget target)
{
    m_target = target;
}

void Texture::SetFilterMode(GL::FilterMode filterMode)
{
    if (filterMode != GetFilterMode())
    {
        m_filterMode = filterMode;

        Bind();

        if (GetFilterMode() == GL::FilterMode::Nearest ||
            GetFilterMode() == GL::FilterMode::Bilinear)
        {
            GL::TexParameterFilter(GetTextureTarget(), GL::FilterMagMin::Mag,
                                   GetFilterMode());
        }
        GL::TexParameterFilter(GetTextureTarget(), GL::FilterMagMin::Min,
                               GetFilterMode());

        UnBind();

        PropagateTextureChanged();
    }
}

void Texture::SetWrapMode(GL::WrapMode wrapMode)
{
    if (wrapMode != GetWrapMode())
    {
        m_wrapMode = wrapMode;

        Bind();
        GL::TexParameterWrap(GetTextureTarget(), GL::WrapCoord::WrapS, GetWrapMode());
        GL::TexParameterWrap(GetTextureTarget(), GL::WrapCoord::WrapT, GetWrapMode());
        GL::TexParameterWrap(GetTextureTarget(), GL::WrapCoord::WrapR, GetWrapMode());
        UnBind();

        PropagateTextureChanged();
    }
}

int Texture::GetWidth() const
{
    return m_size.x;
}

int Texture::GetHeight() const
{
    return m_size.y;
}

const Vector2i &Texture::GetSize() const
{
    return m_size;
}

GL::DataType Texture::GetInternalDataType() const
{
    return GL::GetDataTypeFrom( GetInternalFormat() );
}

GL::ColorFormat Texture::GetInternalFormat() const
{
    return m_internalFormat;
}

uint Texture::GetBytesSize() const
{
    return GetWidth() * GetHeight() * GL::GetPixelBytesSize(m_internalFormat);
}

GL::TextureTarget Texture::GetTextureTarget() const
{
    return m_target;
}

GL::FilterMode Texture::GetFilterMode() const
{
    return m_filterMode;
}

GL::WrapMode Texture::GetWrapMode() const
{
    return m_wrapMode;
}

void Texture::BindToTextureUnit(int textureUnit) const
{
    GL::ActiveTexture(GL_TEXTURE0 + textureUnit);
    Bind();
}

void Texture::SetWidth(int width)
{
    if (width != GetWidth())
    {
        m_size.x = width;
        PropagateTextureChanged();
    }
}
void Texture::SetHeight(int height)
{
    if (height != GetHeight())
    {
        m_size.y = height;
        PropagateTextureChanged();
    }
}

GL::BindTarget Texture::GetGLBindTarget() const
{
    return GL::BindTarget::Texture2D;
}

void Texture::PropagateTextureChanged()
{
    EventEmitter<ITextureChangedListener>::PropagateToListeners(
                &ITextureChangedListener::OnTextureChanged, this);
}
