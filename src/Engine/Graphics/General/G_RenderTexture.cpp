#include "Bang/G_RenderTexture.h"

G_RenderTexture::G_RenderTexture() : Texture2D()
{
    SetFilterMode(G_Texture::FilterMode::Linear);
    SetWrapMode(G_Texture::WrapMode::Clamp);
    SetFormat(G_Texture::Format::RGBA_Float32);
}

G_RenderTexture::~G_RenderTexture()
{

}
