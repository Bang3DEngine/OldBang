#ifndef PREFABFILEINSPECTABLE_H
#define PREFABFILEINSPECTABLE_H

#include <QTimer>
#include "Bang/WinUndef.h"

#include "Bang/File.h"
#include "Bang/GameObjectInspectable.h"

class GameObject;
class InspectorWidget;
class PrefabFileInspectable : public Inspectable<GameObject>,
                                   public IWindowEventManagerListener
{
    ICLONEABLE(PrefabFileInspectable)

public:
    PrefabFileInspectable();
    PrefabFileInspectable(const File &f);
    virtual ~PrefabFileInspectable();

    void CloneInto(ICloneable *clone) const override;

    void OnInspectorSlotChanged(InspectorWidget *InspectorWidget) override;

private:
    File m_file;
};

#endif // PREFABFILEINSPECTABLE_H
