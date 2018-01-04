#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include "Bang/Material.h"
#include "Bang/ResourceHandle.h"

NAMESPACE_BANG_BEGIN

class MaterialFactory
{
public:
    static RH<Material> GetDefault();
    static RH<Material> GetDefaultUnLighted();
    static RH<Material> GetOnlyColor();

    static RH<Material> GetMissing();

    static RH<Material> GetPointLight();
    static RH<Material> GetDirectionalLight();

    static RH<Material> GetUIText();
    static RH<Material> GetUIImage();

    static RH<Material> GetRenderGBufferToScreen();

private:
    Map<String, RH<Material>> m_cache;

    MaterialFactory() = default;
    static RH<Material> Load(const String &matEnginePath);

    static MaterialFactory* GetActive();

    friend class Resources;
};

NAMESPACE_BANG_END

#endif // MATERIALFACTORY_H
