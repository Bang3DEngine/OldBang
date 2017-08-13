#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include "Bang/Material.h"

class MaterialFactory
{
public:
    static Material* GetDefault();
    static Material* GetDefaultUnLighted();

    static Material* GetMissing();

    static Material* GetPointLight();
    static Material* GetDirectionalLight();

    static Material* GetUIText();
    static Material* GetUIImage();

    static Material* GetRenderGBufferToScreen();

    MaterialFactory() = delete;

private:
    static Material* Load(const String &enginePath);
};

#endif // MATERIALFACTORY_H
