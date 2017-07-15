#ifndef COMPONENT_H
#define COMPONENT_H

#include <QPixmap>
#include "Bang/WinUndef.h"

#include "Bang/Paths.h"
#include "Bang/Object.h"
#include "Bang/String.h"
#include "Bang/IToString.h"
#include "Bang/SerializableObject.h"
#include "Bang/ISceneEventListener.h"

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

    virtual String GetInstanceId() const override;
    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

protected:
    bool m_enabled = true;

    Component();
    virtual ~Component();

    virtual void OnDrawGizmos(bool depthed, bool overlay) override;

    friend class ComponentClipboard;
};

#endif // COMPONENT_H
