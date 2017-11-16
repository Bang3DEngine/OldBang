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
#include "Bang/SceneManager.h"
#include "Bang/ObjectManager.h"
#include "Bang/IEnabledListener.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

#define _PROPAGATE_EVENT(FUNCTION, ITERABLE, _CAST)\
do {\
    for (auto it = (ITERABLE).Begin(); it != (ITERABLE).End(); ++it ) \
    { \
        Object *obj = _CAST<Object*>(*it); \
        if (obj->IsEnabled() && \
            obj->IsStarted() && \
           !obj->IsWaitingToBeDestroyed()) \
        { \
            (*it)->FUNCTION; \
        } \
    } \
} while (0)
#define PROPAGATE_EVENT_D(FUNC, ITER) _PROPAGATE_EVENT(FUNC, ITER, DCAST)
#define   PROPAGATE_EVENT(FUNC, ITER) _PROPAGATE_EVENT(FUNC, ITER, SCAST)

GameObject::GameObject(const String &name) : m_name(name)
{
    ASSERT(ObjectManager::AssertCreatedFromObjectManager);
}

GameObject::~GameObject()
{
    ASSERT(ObjectManager::AssertDestroyedFromObjectManager);
    SetParent(nullptr);
}

void GameObject::PreUpdate()
{
    PROPAGATE_EVENT(OnPreUpdate(), GetComponents());
    PROPAGATE_EVENT(PreUpdate(), GetChildren());
}

void GameObject::Update()
{
    PROPAGATE_EVENT(OnUpdate(), GetComponents());
    PROPAGATE_EVENT(Update(), GetChildren());
}

void GameObject::PostUpdate()
{
    PROPAGATE_EVENT(OnPostUpdate(), GetComponents());
    PROPAGATE_EVENT(PostUpdate(), GetChildren());
}

void GameObject::Render(RenderPass renderPass, bool renderChildren)
{
    PROPAGATE_EVENT(OnRender(renderPass), GetComponents());
    if (renderChildren)
    {
        BeforeChildrenRender(renderPass);
        PROPAGATE_EVENT(Render(renderPass, true), GetChildren());
        AfterChildrenRender(renderPass);
    }
}

void GameObject::BeforeChildrenRender(RenderPass renderPass)
{
    PROPAGATE_EVENT(OnBeforeChildrenRender(renderPass), GetComponents());
}

void GameObject::AfterChildrenRender(RenderPass renderPass)
{
    PROPAGATE_EVENT(OnAfterChildrenRender(renderPass), GetComponents());
}

void GameObject::ChildAdded(GameObject *addedChild)
{
    PROPAGATE(IChildrenListener, OnChildAdded, SCAST<GameObject*>(addedChild));
    PROPAGATE_2(IChildrenListener, OnChildAdded,
                GetComponents<IChildrenListener>(), {GetParent()},
                SCAST<GameObject*>(addedChild));
}

void GameObject::ChildRemoved(GameObject *removedChild)
{
    PROPAGATE_2(IChildrenListener, OnChildRemoved,
                GetComponents<IChildrenListener>(), {GetParent()},
                SCAST<GameObject*>(removedChild));
}

void GameObject::ParentChanged(GameObject *oldParent, GameObject *newParent)
{
    PROPAGATE_2(IChildrenListener, OnParentChanged,
                GetComponents<IChildrenListener>(), GetChildren(),
                SCAST<GameObject*>(oldParent),
                SCAST<GameObject*>(newParent));
}

void GameObject::RenderGizmos()
{
    PROPAGATE_EVENT(OnRenderGizmos(), GetComponents());
    PROPAGATE_EVENT(RenderGizmos(), GetChildren());
}

void GameObject::PropagateEnabledEvent(bool enabled) const
{
    auto enabledListeners = GetComponents<IEnabledListener>();
    for (IEnabledListener *eList : enabledListeners)
    {
        if (enabled) { eList->OnEnabled(); } else { eList->OnDisabled(); }
    }

    for (GameObject *child : GetChildren())
    {
        if (enabled) { child->OnEnabled(); } else { child->OnDisabled(); }
    }
}
void GameObject::OnEnabled()
{
    PROPAGATE_2(IEnabledListener, OnEnabled,
                GetComponents<IEnabledListener>(), GetChildren());
}
void GameObject::OnDisabled()
{
    PROPAGATE_2(IEnabledListener, OnEnabled,
                GetComponents<IEnabledListener>(), GetChildren());
}

void GameObject::Destroy(GameObject *gameObject)
{
    for (GameObject *child : gameObject->GetChildren())
    {
        GameObject::Destroy(child);
    }

    for (Component *comp : gameObject->GetComponents())
    {
        ObjectManager::Destroy(comp);
    }

    ObjectManager::Destroy(gameObject);
}

bool GameObject::IsEnabled(bool recursive) const
{
    if (!recursive) { return Object::IsEnabled(); }
    else { return IsEnabled(false) &&
                  (!GetParent() || GetParent()->IsEnabled(true)); }
}

Component *GameObject::AddComponent(const String &componentClassName,
                                    int _index)
{
    Component *c = ComponentFactory::Create(componentClassName);
    return AddComponent(c, _index);
}

Component* GameObject::AddComponent(Component *c, int _index)
{
    if (c && !GetComponents().Contains(c))
    {
        Transform *trans = DCAST<Transform*>(c);
        if (trans) { ASSERT(!HasComponent<Transform>()); }

        const int index = (_index != -1 ? _index : GetComponents().Size());
        m_components.Insert(index, c);

        c->SetGameObject(this);
        if (IsStarted()) { c->Start(); }

        if (trans) { p_transform = trans; }
    }
    return c;
}

const List<Component *> &GameObject::GetComponents() const
{
    return m_components;
}

Component *GameObject::GetComponentByGUID(const GUID &guid) const
{
    for (Component *comp : GetComponents())
    {
        if (comp->GetGUID() == guid) { return comp; }
    }
    return nullptr;
}

void GameObject::RemoveComponent(Component *c)
{
    ObjectManager::Destroy(c);
}

Scene* GameObject::GetScene() const
{
    Scene *scene = DCAST<Scene*>( const_cast<GameObject*>(this) );
    if (scene) { return scene; }

    GameObject *parent = GetParent();
    return parent ? parent->GetScene() : nullptr;
}

Transform *GameObject::GetTransform() const { return p_transform; }

void GameObject::SetName(const String &name) { m_name = name; }
const String& GameObject::GetName() const { return m_name; }

GameObject *GameObject::Find(const String &name)
{
    Scene *scene = SceneManager::GetActiveScene();
    return scene->FindInChildren(name);
}

GameObject *GameObject::FindInChildren(const String &name, bool recursive)
{
    for (GameObject *child : GetChildren())
    {
        if (child->GetName() == name)
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

GameObject *GameObject::GetChild(const GUID &guid) const
{
    for (GameObject *go : GetChildren())
    {
        if (go->GetGUID() == guid) { return go; }
    }
    return nullptr;
}

GameObject *GameObject::GetChild(const String &name) const
{
    for (GameObject *child : GetChildren())
    {
        if (child->m_name == name) { return child; }
    }
    return nullptr;
}

const List<GameObject *> &GameObject::GetChildren() const
{
    return m_children;
}

GameObject *GameObject::GetChild(int index) const
{
    if (index < 0 || index >= GetChildren().Size()) { return nullptr; }

    auto it = GetChildren().Begin(); std::advance(it, index);
    return *it;
}

List<GameObject *> GameObject::GetChildrenRecursively() const
{
    List<GameObject*> cc;
    for (GameObject *c : GetChildren())
    {
        cc.PushBack(c);
        List<GameObject*> childChildren = c->GetChildrenRecursively();
        cc.Splice(cc.Begin(), childChildren);
    }
    return cc;
}

void GameObject::SetAsChild(GameObject *child)
{
    child->SetParent(this);
}

bool GameObject::IsChildOf(const GameObject *_parent, bool recursive) const
{
    if (!GetParent()) { return false; }

    if (recursive)
    {
        return GetParent() == _parent || GetParent()->IsChildOf(_parent);
    }
    return GetParent() == _parent;
}

void GameObject::SetParent(GameObject *newParent, int _index)
{
    ASSERT( newParent != this );
    ASSERT( !newParent || !newParent->IsChildOf(this) );

    GameObject *oldParent = p_parent;
    if (GetParent())
    {
        GetParent()->m_children.Remove(this);
        GetParent()->ChildRemoved(this);
    }

    p_parent = newParent;
    if (GetParent())
    {
        int index = (_index != -1 ? _index : GetParent()->GetChildren().Size());
        p_parent->m_children.Insert(index, this);
        p_parent->ChildAdded(this);
    }

    ParentChanged(oldParent, newParent);
}

GameObject *GameObject::GetParent() const { return p_parent; }

void GameObject::SetDontDestroyOnLoad(bool dontDestroyOnLoad)
{
    m_dontDestroyOnLoad = dontDestroyOnLoad;
}

bool GameObject::IsDontDestroyOnLoad() const
{
    return m_dontDestroyOnLoad;
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
            if (child->GetTransform())
            {
                mat = child->GetTransform()->GetLocalToParentMatrix();
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
    Matrix4 mat = Matrix4::Identity;
    if (GetTransform()) { mat = GetTransform()->GetLocalToWorldMatrix(); }
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

void GameObject::CloneInto(ICloneable *clone) const
{
    Serializable::CloneInto(clone);
    GameObject *go = SCAST<GameObject*>(clone);
    go->SetName(m_name);
    go->SetParent(nullptr);

    for (GameObject *child : GetChildren())
    {
        GameObject *childClone = child->Clone();
        childClone->SetParent(go);
    }

    for (Component *comp : GetComponents())
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

String GameObject::ToStringStructure(bool recursive, const String &indent)
{
    std::ostringstream oss;
    oss << indent << " - " << GetName() << GetComponents();
    if (recursive)
    {
        oss << "\n";
        for (GameObject *child : GetChildren())
        { oss << child->ToStringStructure(true, indent + "  "); }
    }
    return String(oss.str());
}

String GameObject::ToString() const
{
    std::ostringstream oss;
    oss << "GameObject: " << GetName() << "(" << ((void*)this) << ")";
    return oss.str();
}

String GameObject::GetInstanceId() const
{
    String instanceId = GetName();
    if ( GetParent() )
    {
        instanceId.Prepend( GetParent()->GetInstanceId() + "_");
        GameObject *ncThis = const_cast<GameObject*>(this);
        int indexInParent = GetParent()->GetChildren().IndexOf(ncThis);
        instanceId.Append( String::ToString(indexInParent) );
    }
    return instanceId;
}

void GameObject::ImportXML(const XMLNode &xmlInfo)
{
    Serializable::ImportXML(xmlInfo);

    if (xmlInfo.Contains("Enabled"))
    { SetEnabled( xmlInfo.Get<bool>("Enabled") ); }

    if (xmlInfo.Contains("Name"))
    { SetName( xmlInfo.Get<String>("Name") ); }

    if (xmlInfo.Contains("DontDestroyOnLoad"))
    { SetDontDestroyOnLoad( xmlInfo.Get<bool>("DontDestroyOnLoad") ); }

    for (const XMLNode& xmlChild : xmlInfo.GetChildren() )
    {
        const String& tagName = xmlChild.GetTagName();
        if (tagName == GameObject::GetClassNameStatic())
        {
            GameObject *child = GameObjectFactory::CreateGameObject(false);
            child->SetParent(this);
            child->ImportXML(xmlChild);
        }
        else
        {
            Component *comp = AddComponent(tagName);
            comp->ImportXML(xmlChild);
        }
    }
}

void GameObject::ExportXML(XMLNode *xmlInfo) const
{
    Serializable::ExportXML(xmlInfo);

    xmlInfo->Set("Enabled", IsEnabled());
    xmlInfo->Set("Name", GetName());
    xmlInfo->Set("DontDestroyOnLoad", IsDontDestroyOnLoad());

    for (Component *c : GetComponents())
    {
        if (c->GetHideFlags().IsOff(HideFlag::DontSave))
        {
            XMLNode xmlComp;
            c->ExportXML(&xmlComp);
            xmlInfo->AddChild(xmlComp);
        }
    }

    for (GameObject *child : GetChildren())
    {
        if (child->GetHideFlags().IsOff(HideFlag::DontSave))
        {
            XMLNode xmlChild;
            child->ExportXML(&xmlChild);
            xmlInfo->AddChild(xmlChild);
        }
    }
}
