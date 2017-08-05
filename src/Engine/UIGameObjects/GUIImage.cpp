#include "Bang/GUIImage.h"

#include "Bang/UIImage.h"

GUIImage::GUIImage(const Path &imagePath) : UIGameObject("GUIImage")
{
    m_texture = new Texture2D(imagePath);

    p_image = AddComponent<UIImage>();
    p_image->SetImage(m_texture);
}

GUIImage::~GUIImage()
{
    delete m_texture;
}

UIImage *GUIImage::GetImage()
{
    return p_image;
}

const UIImage *GUIImage::GetImage() const
{
    return p_image;
}
