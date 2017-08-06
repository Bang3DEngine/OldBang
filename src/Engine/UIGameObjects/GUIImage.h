#ifndef GUIIMAGE_H
#define GUIIMAGE_H

#include "Bang/Path.h"
#include "Bang/UIGameObject.h"

FORWARD class UIImageRenderer;
FORWARD class Texture2D;

class GUIImage : public UIGameObject
{
public:
    GUIImage(const Path &imagePath = Path::Empty);
    virtual ~GUIImage();

    UIImageRenderer *GetImage();
    const UIImageRenderer *GetImage() const;

private:
    UIImageRenderer *p_image = nullptr;
    Texture2D *m_texture = nullptr;
};

#endif // GUIIMAGE_H
