#ifndef PREFABINSPECTORWIDGETGROUP_H
#define PREFABINSPECTORWIDGETGROUP_H

#include "Bang/Path.h"
#include "Bang/GameObjectInspectorWidgetGroup.h"

class Prefab;
class PrefabInspectorWidgetGroup : public GameObjectInspectorWidgetGroup
{
public:
    PrefabInspectorWidgetGroup(Prefab *prefab);
    virtual ~PrefabInspectorWidgetGroup();

    virtual void OnInspectorWidgetChanged(IInspectorWidget *inspWidget) override;

private:
    Path m_prefabFilepath;
};

#endif // PREFABINSPECTORWIDGETGROUP_H
