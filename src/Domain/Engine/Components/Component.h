#ifndef COMPONENT_H
#define COMPONENT_H

#include "Object.h"
#include "SerializableObject.h"
#include "IToString.h"
#include "ISceneEventListener.h"

#ifdef BANG_EDITOR
#include "SerializableObject.h"
#include "IWindowEventManagerListener.h"
#endif

#include "String.h"

class XMLNode;
class Transform;
class GameObject;
class Component :
              public Object,
              public ISceneEventListener,
              public IToString,
              public SerializableObject
{
protected:
    bool m_enabled = true;
    bool m_closedInInspector = false;

    Component();
    virtual ~Component();

    virtual void OnAddedToGameObject() {}

    virtual void OnDrawGizmos(bool depthed, bool overlay) override;

public:

    GameObject *m_gameObject = nullptr;
    Transform *m_gameObjectTransform = nullptr;
    GameObject* const& gameObject = m_gameObject;
    Transform* const& transform = m_gameObjectTransform; // shortcut

    virtual void CloneInto(ICloneable *clone) const override;
    ICloneable *CloneVirtual() const override;

    virtual String ToString() const override;

    virtual String GetName() const;

    void SetGameObject(GameObject *gameObject);

    void SetEnabled(bool enabled);
    bool IsEnabled() const;

    void SetClosedInInspector(bool closed);
    bool IsClosedInInspector() const;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    friend class GameObject;
    friend class ICloneable;
    friend class ComponentClipboard;
};

#endif // COMPONENT_H
