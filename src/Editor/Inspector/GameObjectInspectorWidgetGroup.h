#ifndef GAMEOBJECTINSPECTORWIDGETGROUP_H
#define GAMEOBJECTINSPECTORWIDGETGROUP_H

#include "Bang/Array.h"
#include "Bang/InspectorWidgetGroup.h"

class Component;
class GameObject;
class GameObjectInspectorWidgetGroup : public InspectorWidgetGroup
{
public:
    GameObjectInspectorWidgetGroup(GameObject *go);
    virtual ~GameObjectInspectorWidgetGroup();

    virtual String GetTitle() const override;

    virtual void Init() override;
    virtual void OnUpdate() override;
    virtual bool NeedsEnableCheckBox() const override;
    virtual void OnEnableCheckBoxChanged(bool enabled) override;
    virtual bool IsEnabled() const override;

    virtual List<InspectorWidget*>::Iterator RemoveInspectorWidget(
                                                InspectorWidget *inspWidget);

protected:
    GameObject *p_gameObject = nullptr;
    List<Component*> p_components;
};

#endif // GAMEOBJECTINSPECTORWIDGETGROUP_H
