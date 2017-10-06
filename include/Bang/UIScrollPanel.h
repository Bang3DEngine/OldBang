#ifndef UISCROLLPANEL_H
#define UISCROLLPANEL_H

#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

class UIScrollPanel : public IComponentDriver<UIScrollPanel>,
                      public Component
{
    COMPONENT(UIScrollPanel)

public:
	virtual ~UIScrollPanel();

    void OnUpdate() override;

    GameObject   *GetContainer() const;
    UIScrollArea *GetScrollArea() const;
    UIScrollBar  *GetScrollBar() const;

private:
    UIScrollPanel();

    UIScrollArea *p_scrollArea = nullptr;
    UIScrollBar  *p_scrollBar  = nullptr;

    static UIScrollPanel* CreateInto(GameObject *go);

    friend class GameObjectFactory;
    friend class IComponentDriver<UIScrollPanel>;
};

NAMESPACE_BANG_END

#endif // UISCROLLPANEL_H_H

