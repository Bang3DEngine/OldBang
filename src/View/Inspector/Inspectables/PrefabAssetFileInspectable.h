#ifndef PREFABASSETFILEINSPECTABLE_H
#define PREFABASSETFILEINSPECTABLE_H

#include <QTimer>
#include "Bang/WinUndef.h"

#include "Bang/File.h"
#include "Bang/GameObjectInspectable.h"

class GameObject;
class InspectorWidget;
class PrefabAssetFileInspectable : public Inspectable<GameObject>,
                                   public IWindowEventManagerListener
{
    ICLONEABLE(PrefabAssetFileInspectable)

public:
    PrefabAssetFileInspectable();
    PrefabAssetFileInspectable(const File &f);
    virtual ~PrefabAssetFileInspectable();

    void CloneInto(ICloneable *clone) const override;

    void OnInspectorSlotChanged(InspectorWidget *InspectorWidget) override;

private:
    File m_file;
};

#endif // PREFABASSETFILEINSPECTABLE_H
