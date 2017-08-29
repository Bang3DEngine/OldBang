#include "Bang/G_RenderTexture.h"

G_RenderTexture::G_RenderTexture() : Texture2D()
{
    SetFilterMode(GL::FilterMode::Linear);
    SetWrapMode(GL::WrapMode::Clamp);
    SetInternalFormat(GL::ColorInternalFormat::RGBA_Float32);
}

G_RenderTexture::~G_RenderTexture()
{

}
