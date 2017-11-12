#ifndef UITREE_H
#define UITREE_H

#include "Bang/Component.h"
#include "Bang/IComponentDriver.h"

NAMESPACE_BANG_BEGIN

class UITree : public IComponentDriver<UITree>,
               public Component
{
    COMPONENT(UITree);

public:
    void OnUpdate() override;

protected:
    UITree();
    virtual ~UITree();

    static UITree* CreateInto(GameObject *go);

    friend class IComponentDriver<UITree>;
};

NAMESPACE_BANG_END

#endif // UITREE_H

