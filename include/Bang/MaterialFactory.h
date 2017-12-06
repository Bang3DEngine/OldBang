#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include "Bang/Material.h"
#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

class MaterialFactory
{
public:
    static void GetDefault(RH<Material> *material);
    static void GetDefaultUnLighted(RH<Material> *material);

    static void GetMissing(RH<Material> *material);

    static void GetPointLight(RH<Material> *material);
    static void GetDirectionalLight(RH<Material> *material);

    static void GetUIText(RH<Material> *material);
    static void GetUIImage(RH<Material> *material);

    static void GetRenderGBufferToScreen(RH<Material> *material);

    MaterialFactory() = delete;

private:
    static void Load(RH<Material> *material, const String &enginePath);
};

NAMESPACE_BANG_END

#endif // MATERIALFACTORY_H
