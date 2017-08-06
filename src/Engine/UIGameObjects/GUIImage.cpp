#include "Bang/GUIImage.h"

GUIImage::GUIImage(const Path &imagePath) : UIGameObject("GUIImage")
{
    p_image = AddComponent<UIImageRenderer>();

    if (imagePath.IsFile())
    {
        m_texture = new Texture2D(imagePath);
        p_image->SetImage(m_texture);
    }
}

GUIImage::~GUIImage()
{
    if (m_texture) { delete m_texture; }
}

UIImageRenderer *GUIImage::GetImage()
{
    return p_image;
}

const UIImageRenderer *GUIImage::GetImage() const
{
    return p_image;
}
