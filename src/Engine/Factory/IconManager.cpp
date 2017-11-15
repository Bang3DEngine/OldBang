#include "Bang/IconManager.h"

#include "Bang/Paths.h"
#include "Bang/Resources.h"
#include "Bang/Texture2D.h"

USING_NAMESPACE_BANG

Texture2D *IconManager::GetRightArrowIcon() { return GetIconTexture("RightArrow"); }
Texture2D *IconManager::GetDownArrowIcon() { return GetIconTexture("DownArrow"); }

Texture2D *IconManager::GetIconTexture(const String &filename)
{
    Texture2D *iconTex =
            Resources::Load<Texture2D>(Paths::EngineAssets().
                                       Append("Icons").
                                       Append(filename).
                                       AppendExtension("png"));
    ASSERT(iconTex);

    iconTex->SetFilterMode(GL::FilterMode::Linear);
    iconTex->SetWrapMode(GL::WrapMode::ClampToEdge);

    return iconTex;
}

