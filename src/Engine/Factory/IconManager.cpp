#include "Bang/IconManager.h"

#include "Bang/Paths.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"

USING_NAMESPACE_BANG

void IconManager::GetRightArrowIcon(RH<Texture2D> *iconTexture)
{ return GetIconTexture(iconTexture, "RightArrow"); }
void IconManager::GetDownArrowIcon(RH<Texture2D> *iconTexture)
{ return GetIconTexture(iconTexture, "DownArrow"); }

void IconManager::GetIconTexture(RH<Texture2D> *iconTexture, const String &filename)
{
    Resources::Load<Texture2D>(iconTexture,
                               Paths::EngineAssets().Append("Icons").
                               Append(filename).AppendExtension("png"));

    iconTexture->Get()->SetFilterMode(GL::FilterMode::Linear);
    iconTexture->Get()->SetWrapMode(GL::WrapMode::ClampToEdge);
}

