#ifndef COMPONENT_H
#define COMPONENT_H

#include "Bang.h"

#include "Debug.h"
#include "IFileable.h"
#include "IToString.h"
#include "ICloneable.h"
#include "FileWriter.h"
#include "ISceneEventListener.h"

#ifdef BANG_EDITOR
#include "IInspectable.h"
#include "InspectorWidget.h"
#include "IWindowEventManagerListener.h"
#endif

#define CAN_USE_COMPONENT(comp)  ( comp  && comp->IsEnabled() )

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
friend class GameObject;
friend class ComponentClipboard;
protected:
    bool m_enabled = true;

    Component();
    virtual ~Component();

    virtual void OnAddedToGameObject() {}

    virtual void OnDrawGizmos() override;
    virtual void OnDrawGizmosNoDepth() override;

public:

    GameObject *m_gameObject = nullptr;
    Transform *m_gameObjectTransform = nullptr;
    GameObject* const& gameObject = m_gameObject;
    Transform* const& transform = m_gameObjectTransform; // shortcut

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override = 0;

    virtual const String ToString() const override;

    virtual String GetName() const;

    void SetGameObject(GameObject *gameObject);

    void SetEnabled(bool enabled);
    bool IsEnabled() const;

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;
};

#endif // COMPONENT_H
