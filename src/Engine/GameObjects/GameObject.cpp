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

GameObject::GameObject(const String &name)
    : m_name(name)
{
}


void GameObject::CloneInto(ICloneable *clone) const
{
    Object::CloneInto(clone);
    GameObject *go = SCAST<GameObject*>(clone);
    go->SetName(m_name);
    go->SetParent(nullptr);

    for (GameObject *child : m_children)
    {
        GameObject *childClone = child->Clone();
        childClone->SetParent(go);
    }

    for (Component *comp : m_components)
    {
        go->AddComponent(comp->Clone());
    }
}

GameObject::~GameObject()
{
    while (!m_children.IsEmpty())
    {
        GameObject *child = m_children.Front();
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

void GameObject::SetParent(GameObject *newParent, int _index)
{
    if (p_parent) { p_parent->m_children.Remove(this); }

    p_parent = newParent;
    if (p_parent)
    {
        int index = (_index != -1 ? _index : p_parent->m_children.Size());
        p_parent->m_children.Insert(index, this);
        ParentSizeChanged();
    }
}

GameObject *GameObject::GetChild(int index) const
{
    auto it = GetChildren().Begin(); std::advance(it, index);
    return *it;
}

GameObject *GameObject::GetParent() const
{
    return p_parent;
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

const List<GameObject*>& GameObject::GetChildren() const
{
    return m_children;
}

List<GameObject*> GameObject::GetChildrenRecursively() const
{
    List<GameObject*> cc;
    for (GameObject *c : m_children)
    {
        cc.PushBack(c);
        List<GameObject*> childChildren = c->GetChildrenRecursively();
        cc.Splice(cc.Begin(), childChildren);
    }
    return cc;
}

Rect GameObject::GetBoundingScreenRect(Camera *cam,
                                       bool includeChildren) const
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
        for (GameObject *child : m_children)
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

Component* GameObject::AddComponent(Component *c, int _index)
{
    if (!c) { return nullptr; }
    if (m_components.Contains(c)) { return c; }

    const int index = (_index != -1 ? _index : m_components.Size());
    m_components.Insert(index, c);

    c->SetGameObject(this);
    if (IsStarted()) { c->Start(); }

    Transform *trans = DCAST<Transform*>(c);
    if (trans) { m_transform = trans; }

    return c;
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
    if (m_transform == c) { m_transform = nullptr; }
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
    for (auto it = m_children.Begin(); it != m_children.End(); ++it)
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

void GameObject::Print(const String &indent) const
{
    String indent2 = indent; indent2 += "   ";
    Debug_Log(indent << this);
    for (GameObject *child : m_children)
    {
        child->Print(indent2);
    }
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

void GameObject::UpdateXMLInfo(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);

    SetEnabled( xmlInfo.Get<bool>("enabled") );
    SetName( xmlInfo.Get<String>("name") );

    // IMPORTANT: The order of the xmlNodes must match the order
    // of the children and components list, in order to update every child/comp
    // with its info, and not with another one !!!!!!!!!!!!!!!!!

    const List<XMLNode>& xmlChildren = xmlInfo.GetChildren();
    Array<GameObject*> children = GetChildren().To<Array>();
    Array<Component*> components = GetComponents().To<Array>();
    int iChildren = 0, iComponents = 0;
    for (const XMLNode& xmlChildInfo : xmlChildren)
    {
        String tagName = xmlChildInfo.GetTagName();
        if (tagName.Contains("GameObject"))
        {
            ENSURE(iChildren < children.Size());
            GameObject *child = children[iChildren];
            child->Read(xmlChildInfo);
            ++iChildren;
        }
        else
        {
            ENSURE(iComponents < components.Size());
            Component *component = components[iComponents];
            component->Read(xmlChildInfo);
            ++iComponents;
        }
    }
}

void GameObject::ReadFirstTime(const XMLNode &xmlInfo)
{
    SerializableObject::Read(xmlInfo);

    SetEnabled( xmlInfo.Get<bool>("enabled") );
    SetName( xmlInfo.Get<String>("name") );

    for (const XMLNode& xmlChild : xmlInfo.GetChildren() )
    {
        String tagName = xmlChild.GetTagName();
        if (tagName.Contains("GameObject"))
        {
            GameObject *child = tagName.BeginsWith("UI") ? new UIGameObject() :
                                                           new GameObject();
            child->SetParent(this);
            child->Read(xmlChild);
        }
        else
        {
            const String &componentClassName = tagName;
            Component *newComponent = AddComponent(componentClassName);
            if (newComponent)
            {
                newComponent->Read(xmlChild);
            }
        }
    }
}

void GameObject::Read(const XMLNode &xmlInfo)
{
    if (!m_hasBeenReadOnce)
    {
        ReadFirstTime(xmlInfo);
        m_hasBeenReadOnce = true;
    }
    else
    {
        UpdateXMLInfo(xmlInfo);
    }
}

void GameObject::Write(XMLNode *xmlInfo) const
{
    SerializableObject::Write(xmlInfo);

    xmlInfo->SetTagName("GameObject");
    xmlInfo->Set("id", GetInstanceId());
    xmlInfo->Set("enabled", m_enabled);
    xmlInfo->Set("name", m_name);

    for (Component *c : m_components)
    {
        XMLNode xmlComp;
        c->Write(&xmlComp);
        xmlInfo->AddChild(xmlComp);
    }

    for (GameObject *child : m_children)
    {
        XMLNode xmlChild;
        child->Write(&xmlChild);
        xmlInfo->AddChild(xmlChild);
    }
}

void GameObject::SetEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool GameObject::IsEnabled() const
{
    return m_enabled && (!p_parent ? true : p_parent->IsEnabled());
}

String GameObject::ToString() const
{
    std::ostringstream oss;
    oss << "GameObject: " << m_name << "(" << ((void*)this) << ")";
    return oss.str();
}

bool GameObject::IsChildOf(const GameObject *goParent, bool recursive) const
{
    if (!parent) { return false; }

    if (recursive)
    {
        return parent == goParent || parent->IsChildOf(goParent);
    }
    return parent == goParent;
}

void GameObject::Start()
{
    m_iteratingComponents = true;
    PROPAGATE_EVENT(Start(), m_components);
    m_iteratingComponents = false;
    RemoveQueuedComponents();

    PROPAGATE_EVENT(Start(), m_children);

    ISceneEventListener::Start();
}

void GameObject::Update()
{
    ISceneEventListener::Update();

    m_iteratingComponents = true;
    PROPAGATE_EVENT(Update(), m_components);
    m_iteratingComponents = false;
    RemoveQueuedComponents();

    PROPAGATE_EVENT(Update(), m_children);
}

void GameObject::ParentSizeChanged()
{
    ISceneEventListener::ParentSizeChanged();
    PROPAGATE_EVENT(ParentSizeChanged(), m_components);
    PROPAGATE_EVENT(ParentSizeChanged(), m_children);
}

void GameObject::DrawGizmos()
{
    PROPAGATE_EVENT(DrawGizmos(), m_components);
    OnDrawGizmos();

    PROPAGATE_EVENT(DrawGizmos(), m_children);
}

void GameObject::Destroy()
{
    PROPAGATE_EVENT(Destroy(), m_children);

    m_iteratingComponents = true;
    PROPAGATE_EVENT(Destroy(), m_components);
    m_iteratingComponents = false;
    RemoveQueuedComponents();

    OnDestroy();
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
