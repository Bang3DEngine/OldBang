#include "Bang/GameObject.h"

#include <iostream>

#include "Bang/GL.h"
#include "Bang/AABox.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Sphere.h"
#include "Bang/Camera.h"
#include "Bang/Renderer.h"
#include "Bang/XMLNode.h"
#include "Bang/Material.h"
#include "Bang/Component.h"
#include "Bang/Transform.h"
#include "Bang/UIGameObject.h"
#include "Bang/SceneManager.h"
#include "Bang/GameObjectFactory.h"

#define PROPAGATE_EVENT_TO_COMPONENTS(Event, Components) \
    m_iteratingComponents = true; \
    PROPAGATE_EVENT(Event, Components); \
    m_iteratingComponents = false; \
    RemoveQueuedComponents();

GameObject::GameObject(const String &name)
    : m_name(name)
{
}

GameObject::~GameObject()
{
    while (!GetChildren().IsEmpty())
    {
        GameObject *child = GetChildren().Front();
        delete child;
    }

    while (!m_components.IsEmpty())
    {
        Component *comp = m_components.Front();
        m_components.PopFront();
        delete comp;
    }

    SetParent(nullptr);
}

const String& GameObject::GetName() const
{
    return m_name;
}

const List<Component *> &GameObject::GetComponents() const
{
    return m_components;
}

void GameObject::Destroy(GameObject *gameObject)
{
    SceneManager::GetActiveScene()->Destroy(gameObject);
}

Rect GameObject::GetBoundingScreenRect(Camera *cam, bool includeChildren) const
{
    AABox bbox = GetAABBox(includeChildren);
    return cam->GetScreenBoundingRect(bbox);
}

AABox GameObject::GetObjectAABBox(bool includeChildren) const
{
    List<Renderer*> rends = GetComponents<Renderer>();
    AABox aabBox = AABox::Empty;
    for (Renderer *rend : rends)
    {
        if (rend && rend->IsEnabled())
        {
            aabBox = AABox::Union(aabBox, rend->GetAABBox());
        }
    }

    if (includeChildren)
    {
        for (GameObject *child : GetChildren())
        {
            AABox aabBoxChild = child->GetObjectAABBox(true);
            Matrix4 mat;
            if (child->transform)
            {
                mat = child->transform->GetLocalToParentMatrix();
            }
            aabBoxChild = mat * aabBoxChild;
            aabBox = AABox::Union(aabBox, aabBoxChild);
        }
    }

    return aabBox;
}

AABox GameObject::GetAABBox(bool includeChildren) const
{
    AABox b = GetObjectAABBox(includeChildren);
    Matrix4 mat;
    if (transform) { transform->GetLocalToWorldMatrix(&mat); }
    b = mat * b;
    return b;
}

Sphere GameObject::GetObjectBoundingSphere(bool includeChildren) const
{
    return Sphere::FromBox(GetObjectAABBox(includeChildren));
}

Sphere GameObject::GetBoundingSphere(bool includeChildren) const
{
    return Sphere::FromBox(GetAABBox(includeChildren));
}

Component *GameObject::AddComponent(const String &componentClassName,
                                    int _index)
{
    Component *c = ComponentFactory::CreateComponent(componentClassName);
    return AddComponent(c, _index);
}

Component* GameObject::
AddComponent(Component *c, int _index)
{
    if (c && !m_components.Contains(c))
    {
        const int index = (_index != -1 ? _index : m_components.Size());
        m_components.Insert(index, c);

        c->SetGameObject(this);
        if (IsStarted()) { c->Start(); }

        Transform *trans = DCAST<Transform*>(c);
        if (trans) { p_transform = trans; }
    }
    return c;
}

GameObject *GameObject::GetChild(const GUID &guid) const
{
    for (GameObject *go : GetChildren())
    {
        if (go->GetGUID() == guid) { return go; }
    }
    return nullptr;
}

Component *GameObject::GetComponent(const GUID &guid) const
{
    for (Component *comp : GetComponents())
    {
        if (comp->GetGUID() == guid) { return comp; }
    }
    return nullptr;
}

void GameObject::RemoveComponent(Component *c)
{
    if (!m_iteratingComponents)
    {
        RemoveComponentInstantly(c);
    }
    else
    {
        m_componentsToBeRemoved.push(c);
    }
}

void GameObject::RemoveComponentInstantly(Component *c)
{
    if (p_transform == c) { p_transform = nullptr; }
    m_components.Remove(c);
    delete c;
}

void GameObject::RemoveQueuedComponents()
{
    while (!m_componentsToBeRemoved.empty())
    {
        Component *c = m_componentsToBeRemoved.front();
        RemoveComponentInstantly(c);
        m_componentsToBeRemoved.pop();
    }
}

GameObject *GameObject::GetChild(const String &name) const
{
    for (auto it = GetChildren().Begin(); it != GetChildren().End(); ++it)
    {
        GameObject *child = (*it);
        if (child->m_name == name)
        {
            return child;
        }
    }
    return nullptr;
}

void GameObject::SetName(const String &name)
{
    m_name = name;
}

GameObject *GameObject::Find(const String &name)
{
    Scene *scene = SceneManager::GetActiveScene();
    return scene->FindInChildren(name);
}

GameObject *GameObject::FindInChildren(const String &name, bool recursive)
{
    for (GameObject *child : GetChildren())
    {
        if (child->name == name)
        {
            return child;
        }
        else if (recursive)
        {
            GameObject *found = child->FindInChildren(name, true);
            if (found)
            {
                return found;
            }
        }
    }
    return nullptr;
}

void GameObject::Start()
{
    PROPAGATE_EVENT_TO_COMPONENTS(Start(), m_components);
    SceneNode<GameObject>::Start();
}

void GameObject::Update()
{
    PROPAGATE_EVENT_TO_COMPONENTS(Update(), m_components);
    SceneNode<GameObject>::Update();
}

void GameObject::ParentSizeChanged()
{
    PROPAGATE_EVENT_TO_COMPONENTS(ParentSizeChanged(), m_components);
    SceneNode<GameObject>::ParentSizeChanged();
}

void GameObject::Render(RenderPass renderPass, bool renderChildren)
{
    PROPAGATE_EVENT_TO_COMPONENTS(Render(renderPass, renderChildren),
                                  m_components);
    SceneNode<GameObject>::Render(renderPass, renderChildren);
}

void GameObject::BeforeChildrenRender(RenderPass renderPass)
{
    PROPAGATE_EVENT_TO_COMPONENTS(BeforeChildrenRender(renderPass),
                                  m_components);
    SceneNode<GameObject>::BeforeChildrenRender(renderPass);
}

void GameObject::ChildrenRendered(RenderPass renderPass)
{
    PROPAGATE_EVENT_TO_COMPONENTS(ChildrenRendered(renderPass), m_components);
    SceneNode<GameObject>::ChildrenRendered(renderPass);
}

void GameObject::RenderGizmos()
{
    PROPAGATE_EVENT_TO_COMPONENTS(RenderGizmos(), m_components);
    SceneNode<GameObject>::RenderGizmos();
}

void GameObject::Destroy()
{
    SceneNode<GameObject>::Destroy();
    PROPAGATE_EVENT_TO_COMPONENTS(Destroy(), m_components);
}

void GameObject::CloneInto(ICloneable *clone) const
{
    SerializableObject::CloneInto(clone);
    GameObject *go = SCAST<GameObject*>(clone);
    go->SetName(m_name);
    go->SetParent(nullptr);

    for (GameObject *child : GetChildren())
    {
        GameObject *childClone = child->Clone();
        childClone->SetParent(go);
    }

    for (Component *comp : m_components)
    {
        go->AddComponent(comp->Clone());
    }
}

void GameObject::Print(const String &indent) const
{
    String indent2 = indent; indent2 += "   ";
    Debug_Log(indent << this);
    for (GameObject *child : GetChildren())
    {
        child->Print(indent2);
    }
}

String GameObject::ToString() const
{
    std::ostringstream oss;
    oss << "GameObject: " << m_name << "(" << ((void*)this) << ")";
    return oss.str();
}

String GameObject::GetInstanceId() const
{
    String instanceId = name;
    if (parent)
    {
        instanceId.Prepend( parent->GetInstanceId() + "_");
        GameObject *ncThis = const_cast<GameObject*>(this);
        int indexInParent = parent->GetChildren().IndexOf(ncThis);
        instanceId.Append( String::ToString(indexInParent) );
    }
    return instanceId;
}

void GameObject::Read(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);

    if (xmlInfo.Contains("Enabled"))
    { SetEnabled( xmlInfo.Get<bool>("Enabled") ); }

    if (xmlInfo.Contains("Name"))
    { SetName( xmlInfo.Get<String>("Name") ); }

    for (const XMLNode& xmlChild : xmlInfo.GetChildren() )
    {
        const String tagName = xmlChild.GetTagName();
        if (GameObjectFactory::ExistsGameObjectClass(tagName))
        {
            GameObject *child = GameObjectFactory::CreateGameObject(tagName);
            child->SetParent(this);
            child->Read(xmlChild);
        }
        else
        {
            Component *comp = AddComponent(tagName);
            comp->Read(xmlChild);
        }
    }
}

void GameObject::Write(XMLNode *xmlInfo) const
{
    SerializableObject::Write(xmlInfo);

    xmlInfo->Set("Enabled", IsEnabled());
    xmlInfo->Set("Name", GetName());

    for (Component *c : GetComponents())
    {
        if (c->GetHideFlags().IsOff(HideFlag::DontSave))
        {
            XMLNode xmlComp;
            c->Write(&xmlComp);
            xmlInfo->AddChild(xmlComp);
        }
    }

    for (GameObject *child : GetChildren())
    {
        if (child->GetHideFlags().IsOff(HideFlag::DontSave))
        {
            XMLNode xmlChild;
            child->Write(&xmlChild);
            xmlInfo->AddChild(xmlChild);
        }
    }
}
