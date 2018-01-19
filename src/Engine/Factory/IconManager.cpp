#include "Bang/IconManager.h"

#include "Bang/Paths.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"

USING_NAMESPACE_BANG

RH<Texture2D> IconManager::GetRightArrowIcon()
{ return GetIconTexture("RightArrow"); }
RH<Texture2D> IconManager::GetDownArrowIcon()
{ return GetIconTexture("DownArrow"); }
RH<Texture2D> IconManager::GetCheckIcon()
{ return GetIconTexture("Check"); }

RH<Texture2D> IconManager::GetIconTexture(const String &filename)
{
    static Map<Path, RH<Texture2D>> cache;

    Path path = Paths::GetEngineAssetsDir().Append("Icons").
                       Append(filename).AppendExtension("png");
    if (!cache.ContainsKey(path))
    {
        RH<Texture2D> iconTex = Resources::Load<Texture2D>(path);
        iconTex.Get()->SetFilterMode(GL::FilterMode::Bilinear);
        iconTex.Get()->SetWrapMode(GL::WrapMode::ClampToEdge);
        cache.Add(path, iconTex);
    }
    return cache.Get(path);
}

