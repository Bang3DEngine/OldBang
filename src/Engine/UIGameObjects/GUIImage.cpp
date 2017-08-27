#include "Bang/GUIImage.h"

#include "Bang/Resources.h"

GUIImage::GUIImage(const Path &imagePath) : UIGameObject("GUIImage")
{
    p_imageRend = AddComponent<UIImageRenderer>();

    if (imagePath.IsFile())
    {
        Debug_Log("Resources::Load<Texture2D>( " << imagePath << " )");
        m_texture = Resources::Load<Texture2D>(imagePath);
        p_imageRend->SetTexture(m_texture);
    }
}

GUIImage::~GUIImage()
{
    if (m_texture) { delete m_texture; }
}

UIImageRenderer *GUIImage::GetImage()
{
    return p_imageRend;
}

const UIImageRenderer *GUIImage::GetImage() const
{
    return p_imageRend;
}
