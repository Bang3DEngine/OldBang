#ifndef UIBUTTONDRIVER_H
#define UIBUTTONDRIVER_H

#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

FORWARD class Texture2D;
FORWARD class UIDirLayout;
FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;
FORWARD class UITintedButton;

class UIButtonDriver : public Component,
                       public IComponentDriver<UIButtonDriver>
{
    COMPONENT(UIButtonDriver)

public:
    void SetIconSize(const Vector2i &size);
    void SetIconTexture(Texture2D *texture);
    void SetIconSpacingWithText(int spacingWithText);
    void SetIcon(Texture2D *texture, const Vector2i &size,
                 int spacingWithText = 5);

    UIImageRenderer* GetIcon() const;
    UITextRenderer* GetText() const;
    UIImageRenderer* GetBackground() const;
    UITintedButton* GetButton() const;
    UIDirLayout *GetDirLayout() const;

private:
    UIButtonDriver();
    virtual ~UIButtonDriver();

    UIImageRenderer *p_icon        = nullptr;
    UITextRenderer  *p_text        = nullptr;
    UIImageRenderer *p_background  = nullptr;
    UITintedButton  *p_button      = nullptr;

    // IComponentDriver
    static UIButtonDriver *CreateInto(GameObject *go);

    friend class GameObjectFactory;
    friend class IComponentDriver<UIButtonDriver>;
};

NAMESPACE_BANG_END

#endif // UIBUTTONDRIVER_H
