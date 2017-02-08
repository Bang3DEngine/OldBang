#ifndef COMPONENT_H
#define COMPONENT_H

#include "IFileable.h"
#include "IToString.h"
#include "ICloneable.h"
#include "ISceneEventListener.h"

#ifdef BANG_EDITOR
#include "IInspectable.h"
#include "IWindowEventManagerListener.h"
#endif

#include "String.h"

#define CAN_USE_COMPONENT(comp)  ( comp  && comp->IsEnabled() )

class XMLNode;
class Transform;
class GameObject;
class Component :
              public ISceneEventListener
             ,public IToString
             ,public IFileable
             ,public ICloneable
            #ifdef BANG_EDITOR
             ,public IWindowEventManagerListener
             ,public IInspectable
            #endif
{
protected:
    bool m_enabled = true;
    bool m_closedInInspector = false;

    Component();
    virtual ~Component();

    virtual void OnAddedToGameObject() {}

    virtual void OnDrawGizmos() override;
    virtual void OnDrawGizmosOverlay() override;

public:

    GameObject *m_gameObject = nullptr;
    Transform *m_gameObjectTransform = nullptr;
    GameObject* const& gameObject = m_gameObject;
    Transform* const& transform = m_gameObjectTransform; // shortcut

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override = 0;

    virtual String ToString() const override;

    virtual String GetName() const;

    void SetGameObject(GameObject *gameObject);

    void SetEnabled(bool enabled);
    bool IsEnabled() const;

    void SetClosedInInspector(bool closed);
    bool IsClosedInInspector() const;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    friend class GameObject;
    friend class ComponentClipboard;
};

#endif // COMPONENT_H
