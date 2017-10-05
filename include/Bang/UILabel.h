#ifndef UILABEL_H
#define UILABEL_H

#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIMask;
FORWARD class UITextRenderer;

class UILabel : public IComponentDriver<UILabel>,
                public Component
{
    COMPONENT(UILabel)

public:
	virtual ~UILabel();

    UIMask *GetMask() const;
    UITextRenderer *GetText() const;

private:
    UIMask *p_mask = nullptr;
    UITextRenderer *p_text = nullptr;

    UILabel();

    // IComponentDriver
    static UILabel *CreateInto(GameObject *go);

    friend class GameObjectFactory;
    friend class IComponentDriver<UILabel>;
};

NAMESPACE_BANG_END

#endif // UILABEL_H_H

