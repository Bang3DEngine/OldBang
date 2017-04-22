#ifndef COMPONENT_H
#define COMPONENT_H

#include <QPixmap>
#include "Bang/WinUndef.h"

#include "Bang/IO.h"
#include "Bang/Object.h"
#include "Bang/String.h"
#include "Bang/IToString.h"
#include "Bang/IconManager.h"
#include "Bang/SerializableObject.h"
#include "Bang/ISceneEventListener.h"

#ifdef BANG_EDITOR
#include "Bang/IWindowEventManagerListener.h"
#endif

class XMLNode;
class Transform;
class GameObject;
class Component : public ISceneEventListener,
                  public IToString,
                  public SerializableObject
{
    OBJECT(Component)

public:
    GameObject *m_gameObject = nullptr;
    Transform *m_gameObjectTransform = nullptr;
    GameObject* const& gameObject = m_gameObject;
    Transform* const& transform = m_gameObjectTransform; // shortcut

    virtual void CloneInto(ICloneable *clone) const override;

    virtual String ToString() const override;

    void SetGameObject(GameObject *gameObject);

    void SetEnabled(bool enabled);
    bool IsEnabled(bool recursive = true) const;

    void SetClosedInInspector(bool closed);
    bool IsClosedInInspector() const;

    virtual String GetInstanceId() const override;
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    #ifdef BANG_EDITOR
    virtual void OnEnabledChanged(bool enabled) override;
    virtual InspectorWidget *GetNewInspectorWidget() override;
    #endif

protected:
    bool m_enabled = true;
    bool m_closedInInspector = false;

    Component();
    virtual ~Component();

    virtual void OnAddedToGameObject() {}

    virtual void OnDrawGizmos(bool depthed, bool overlay) override;

    friend class ComponentClipboard;
};

#define COMPONENT_ICON(CLASS, ICON_PATH) \
public: \
    static const QPixmap& GetIconStatic() \
    {\
        String path = IO::ToAbsolute(ICON_PATH, true); \
        return IconManager::LoadPixmap(path); \
    } \
    const virtual QPixmap& GetIcon() const override \
    { \
        return CLASS::GetIconStatic(); \
    }

#endif // COMPONENT_H
