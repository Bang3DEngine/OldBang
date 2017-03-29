#ifndef PREFABASSETFILEINSPECTABLE_H
#define PREFABASSETFILEINSPECTABLE_H

#include <QTimer>
#include "Bang/WinUndef.h"

#include "Bang/File.h"
#include "Bang/SerializableObject.h"
#include "Bang/IWindowEventManagerListener.h"

class GameObject;
class InspectorWidget;
class GameObjectPrefabInspectable;
class PrefabAssetFileInspectable : public SerializableObject,
                                   public IWindowEventManagerListener
{

private:
    File m_file;
    GameObject *m_prefabTempGameObject = nullptr;

public:
    PrefabAssetFileInspectable(const File &f);
    virtual ~PrefabAssetFileInspectable();

    void ShowInInspector();

    void OnInspectorSlotChanged(InspectorWidget *inspectorItem) override;

    GameObject *GetPrefabTempGameObject() const;
};

#endif // PREFABASSETFILEINSPECTABLE_H
