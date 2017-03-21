#ifndef COMPONENT_H
#define COMPONENT_H

#include <QPixmap>

#include "IO.h"
#include "Object.h"
#include "String.h"
#include "IToString.h"
#include "IconManager.h"
#include "SerializableObject.h"
#include "ISceneEventListener.h"

#ifdef BANG_EDITOR
#include "SerializableObject.h"
#include "IWindowEventManagerListener.h"
#endif

class XMLNode;
class Transform;
class GameObject;
class Component :
              public Object,
              public ISceneEventListener,
              public IToString,
              public SerializableObject
{
    OBJECT(Component)
    ICLONEABLE(Component)

public:
    GameObject *m_gameObject = nullptr;
    Transform *m_gameObjectTransform = nullptr;
    GameObject* const& gameObject = m_gameObject;
    Transform* const& transform = m_gameObjectTransform; // shortcut

    virtual void CloneInto(ICloneable *clone) const override;

    virtual String ToString() const override;

    void SetGameObject(GameObject *gameObject);

    void SetEnabled(bool enabled);
    bool IsEnabled() const;

    void SetClosedInInspector(bool closed);
    bool IsClosedInInspector() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

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
