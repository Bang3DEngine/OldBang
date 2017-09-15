#ifndef SUBSCENE_H
#define SUBSCENE_H

#include "Bang/Rect.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class Scene;
FORWARD class Camera;

class SubScene : public Component
{
    COMPONENT(SubScene)

public:
    SubScene();
    virtual ~SubScene();

    // Component
    virtual void OnBeforeChildrenRender(RenderPass renderPass) override;
    virtual void OnAfterChildrenRender(RenderPass renderPass) override;

    Scene *GetParentScene() const;

private:
    Camera *p_previousCamera   = nullptr;
};

NAMESPACE_BANG_END

#endif // SUBSCENE_H
