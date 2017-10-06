#include "Bang/UILabel.h"

#include "Bang/UIMask.h"
#include "Bang/GameObject.h"
#include "Bang/RectTransform.h"
#include "Bang/UIFrameLayout.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"

USING_NAMESPACE_BANG

UILabel::UILabel()
{
}

UILabel::~UILabel()
{
}

UIMask *UILabel::GetMask() const { return p_mask; }
UITextRenderer *UILabel::GetText() const { return p_text; }

UILabel *UILabel::CreateInto(GameObject *go)
{
    REQUIRE_COMPONENT(go, RectTransform);

    UILabel *label = go->AddComponent<UILabel>();

    UIFrameLayout *fl = go->AddComponent<UIFrameLayout>();

    UIMask *mask = go->AddComponent<UIMask>();
    go->AddComponent<UIImageRenderer>(); // Quad mask
    label->p_mask = mask;

    GameObject *textContainer = GameObjectFactory::CreateUIGameObject();
    UITextRenderer *text = textContainer->AddComponent<UITextRenderer>();
    text->SetTextSize(12);
    text->SetWrapping(false);
    label->p_text = text;

    go->AddChild(textContainer);

    return label;
}

