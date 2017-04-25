#ifndef GAMEOBJECTINSPECTABLE_H
#define GAMEOBJECTINSPECTABLE_H

#include "Bang/Inspectable.h"

class GameObject;
class GameObjectInspectable : public IInspectable
{
public:
    GameObjectInspectable();

    virtual bool IsEnabled() const override;
    virtual bool NeedsEnabledCheckBox() const override;
    virtual void OnEnabledChanged(bool enabled) override;
    virtual String GetTitleInInspector() const override;
    virtual InspectorWidget *GetNewInspectorWidget() override;
    virtual List<IInspectable*> GetNewInspectablesToShow() override;

private:
    GameObject *GetGameObject() const;

};

#endif // GAMEOBJECTINSPECTABLE_H
