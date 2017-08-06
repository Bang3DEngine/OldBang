#include "Bang/GUIImage.h"

#include "Bang/UIImageRenderer.h"

GUIImage::GUIImage(const Path &imagePath) : UIGameObject("GUIImage")
{
    m_texture = new Texture2D(imagePath);

    p_image = AddComponent<UIImageRenderer>();
    p_image->SetImage(m_texture);
}

GUIImage::~GUIImage()
{
    delete m_texture;
}

UIImageRenderer *GUIImage::GetImage()
{
    return p_image;
}

const UIImageRenderer *GUIImage::GetImage() const
{
    return p_image;
}
