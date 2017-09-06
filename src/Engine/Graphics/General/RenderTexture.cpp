#include "Bang/RenderTexture.h"

RenderTexture::RenderTexture() : Texture2D()
{
    SetFilterMode(GL::FilterMode::Linear);
    SetWrapMode(GL::WrapMode::Clamp);
    SetInternalFormat(GL::ColorFormat::RGBA_Float32);
}

RenderTexture::~RenderTexture()
{

}
