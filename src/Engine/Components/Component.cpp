#include "Bang/Component.h"

#include "Bang/String.h"
#include "Bang/XMLNode.h"
#include "Bang/Transform.h"
#include "Bang/GameObject.h"

Component::Component()
{
}

Component::~Component()
{
    for (Component *c : m_delegates) { delete c; }
}

void Component::AddDelegate(Component *delegate)
{
    if (!m_delegates.Contains(delegate))
    {
        m_delegates.PushBack(delegate);
    }
}

void Component::RemoveDelegate(Component *delegate)
{
    m_delegates.Remove(delegate);
}

void Component::SetGameObject(GameObject *gameObject)
{
    m_gameObject = gameObject;
    m_gameObjectTransform = m_gameObject->transform;
}

GameObject *Component::GetGameObject() const { return m_gameObject; }

void Component::CloneInto(ICloneable *clone) const
{
    SerializableObject::CloneInto(clone);
    Component *c = SCAST<Component*>(clone);
    c->SetEnabled( IsEnabled() );
}

String Component::ToString() const
{
    std::ostringstream msg;
    msg << "";
    return GetClassName() + "(" + String::ToString((void*)this) + ")";
}

void Component::Start()
{
    PROPAGATE_EVENT(Start(), m_delegates);
    SceneAgent::Start();
}
void Component::Update()
{
    PROPAGATE_EVENT(Update(), m_delegates);
    SceneAgent::Update();
}
void Component::ParentSizeChanged()
{
    PROPAGATE_EVENT(ParentSizeChanged(), m_delegates);
    SceneAgent::ParentSizeChanged();
}
void Component::Render(RenderPass renderPass)
{
    PROPAGATE_EVENT(Render(renderPass), m_delegates);
    SceneAgent::Render(renderPass);
}
void Component::RenderGizmos()
{
    PROPAGATE_EVENT(RenderGizmos(), m_delegates);
    SceneAgent::RenderGizmos();
}
void Component::Destroy()
{
    SceneAgent::Destroy();
    PROPAGATE_EVENT(Destroy(), m_delegates);
}

String Component::GetInstanceId() const
{
    String instanceId = "Component";
    if (gameObject)
    {
        instanceId.Prepend(gameObject->GetInstanceId() + "_");
        Component *ncThis = const_cast<Component*>(this);
        int indInGameObject = gameObject->GetComponents().IndexOf(ncThis);
        instanceId.Append( String::ToString(indInGameObject) );
    }
    return instanceId;
}

void Component::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);
    SetEnabled(xmlInfo.Get<bool>("enabled"));
}

void Component::Write(XMLNode *xmlInfo) const
{
    SerializableObject::Write(xmlInfo);

    xmlInfo->SetTagName( GetClassName() );
    xmlInfo->Set("id", GetInstanceId());
    xmlInfo->Set("enabled", IsEnabled());
}
