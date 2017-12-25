#include "Bang/IconManager.h"

#include "Bang/Paths.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"

USING_NAMESPACE_BANG

RH<Texture2D> IconManager::GetRightArrowIcon()
{ return GetIconTexture("RightArrow"); }
RH<Texture2D> IconManager::GetDownArrowIcon()
{ return GetIconTexture("DownArrow"); }

RH<Texture2D> IconManager::GetIconTexture(const String &filename)
{
    RH<Texture2D> iconTexture =
         Resources::Load<Texture2D>(Paths::EngineAssets().Append("Icons").
                                    Append(filename).AppendExtension("png"));

    iconTexture.Get()->SetFilterMode(GL::FilterMode::Bilinear);
    iconTexture.Get()->SetWrapMode(GL::WrapMode::ClampToEdge);
    return iconTexture;
}

