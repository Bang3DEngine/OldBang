#ifndef GAMEOBJECTINSPECTORWIDGETGROUP_H
#define GAMEOBJECTINSPECTORWIDGETGROUP_H

#include "Bang/InspectorWidgetGroup.h"

class GameObject;
class GameObjectInspectorWidgetGroup : public InspectorWidgetGroup
{
public:
    GameObjectInspectorWidgetGroup(GameObject *go);
    virtual ~GameObjectInspectorWidgetGroup();

    virtual String GetTitle() const override;

    virtual bool NeedsEnableCheckBox() const override;
    virtual void OnEnableCheckBoxChanged(bool enabled) override;
    virtual bool IsEnabled() const override;

protected:
    GameObject *p_gameObject = nullptr;
};

#endif // GAMEOBJECTINSPECTORWIDGETGROUP_H
