#include "Bang/GameObject.h"

#include <iostream>

#include "Bang/GL.h"
#include "Bang/AABox.h"
#include "Bang/Debug.h"
#include "Bang/Scene.h"
#include "Bang/Sphere.h"
#include "Bang/Camera.h"
#include "Bang/XMLNode.h"
#include "Bang/Renderer.h"
#include "Bang/Material.h"
#include "Bang/Component.h"
#include "Bang/Transform.h"
#include "Bang/SceneManager.h"
#include "Bang/ObjectManager.h"
#include "Bang/RectTransform.h"
#include "Bang/IEnabledListener.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

GameObject::GameObject(const String &name) : m_name(name)
{
}

GameObject::~GameObject()
{
    ASSERT(GetChildren().IsEmpty());
    ASSERT(GetComponents().IsEmpty());
    SetParent(nullptr);
}

void GameObject::PreStart()
{
    PropagateToChildren(&GameObject::PreStart);
    PropagateToComponents(&Component::PreStart);
    Object::PreStart();
}

void GameObject::Start()
{
    PropagateToChildren(&GameObject::Start);
    PropagateToComponents(&Component::Start);
    Object::Start();
}

void GameObject::PreUpdate()
{
    if (!IsActive()) { return; }

    PropagateToComponents(&Component::PreUpdate);
    PropagateToChildren(&GameObject::PreUpdate);
}

void GameObject::BeforeChildrenUpdate()
{
    if (!IsActive()) { return; }

    PropagateToComponents(&Component::BeforeChildrenUpdate);
}

void GameObject::Update()
{
    if (!IsActive()) { return; }

    PropagateToComponents(&Component::Update);
    BeforeChildrenUpdate();
    PropagateToChildren(&GameObject::Update);
    AfterChildrenUpdate();
}

void GameObject::AfterChildrenUpdate()
{
    if (!IsActive()) { return; }

    PropagateToComponents(&Component::OnAfterChildrenUpdate);
}

void GameObject::PostUpdate()
{
    if (!IsActive()) { return; }

    PropagateToComponents(&Component::OnPostUpdate);
    PropagateToChildren(&GameObject::PostUpdate);
}

void GameObject::BeforeRender()
{
    if (!IsActive()) { return; }

    PropagateToComponents(&Component::OnBeforeRender);
    PropagateToChildren(&GameObject::BeforeRender);
}

void GameObject::Render(RenderPass renderPass, bool renderChildren)
{
    if (!IsActive()) { return; }

    if (IsVisible())
    {
        PropagateToComponents(&Component::OnRender, renderPass);
        if (renderChildren)
        {
            BeforeChildrenRender(renderPass);
            PropagateToChildren(&GameObject::Render, renderPass, true);
            AfterChildrenRender(renderPass);
        }
    }
}

void GameObject::BeforeChildrenRender(RenderPass renderPass)
{
    if (!IsActive()) { return; }

    PropagateToComponents(&Component::OnBeforeChildrenRender, renderPass);
}

void GameObject::AfterChildrenRender(RenderPass renderPass)
{
    if (!IsActive()) { return; }

    PropagateToComponents(&Component::OnAfterChildrenRender, renderPass);
}

void GameObject::ChildAdded(GameObject *addedChild, GameObject *parent)
{
    EventEmitter<IChildrenListener>::
          PropagateToListeners(&IChildrenListener::OnChildAdded,
                               addedChild, parent);
    Propagate(&IChildrenListener::OnChildAdded,
              GetComponentsInParentAndThis<IChildrenListener>(),
              addedChild, parent);
    Propagate(&IChildrenListener::OnChildAdded, GetParent(),
              addedChild, parent);
}

void GameObject::ChildRemoved(GameObject *removedChild, GameObject *parent)
{
    EventEmitter<IChildrenListener>::
          PropagateToListeners(&IChildrenListener::OnChildRemoved,
                               removedChild, parent);
    Propagate(&IChildrenListener::OnChildRemoved,
              GetComponentsInParentAndThis<IChildrenListener>(true),
              removedChild, parent);
    Propagate(&IChildrenListener::OnChildRemoved, GetParent(),
              removedChild, parent);
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

void GameObject::AddChild(GameObject *child, int index)
{
    ASSERT(!GetChildren().Contains(child));

    auto nextIt = m_children.Insert(index, child);
    auto it = m_children.Begin(); std::advance(it, index);
    if (!m_currentChildrenIterators.empty() &&
        m_currentChildrenIterators.top() == it)
    {
        m_currentChildrenIterators.top() = nextIt;
        m_increaseChildrenIterator = false;
    }
}

void GameObject::RemoveChild(GameObject *child)
{
    auto it = m_children.Find(child);
    if (it != GetChildren().End())
    {
        auto nextIt = m_children.Remove(it);
        if (!m_currentChildrenIterators.empty() &&
            m_currentChildrenIterators.top() == it)
        {
            m_currentChildrenIterators.top() = nextIt;
            m_increaseChildrenIterator = false;
        }
    }
}

void GameObject::RemoveComponent(Component *component)
{
    auto it = m_components.Find(component);
    if (it != m_components.End())
    {
        auto nextIt = m_components.Remove(it);
        if (!m_currentComponentsIterators.empty() &&
            m_currentComponentsIterators.top() == it)
        {
            m_currentComponentsIterators.top() = nextIt;
            m_increaseComponentsIterator = false;
        }

        EventEmitter<IComponentListener>::PropagateToListeners(
                    &IComponentListener::OnComponentRemoved, component);
    }

    if (component == p_transform) { p_transform = nullptr; }
}

void GameObject::OnEnabled()
{
    Object::OnEnabled();
    Propagate(&IEnabledListener::OnEnabled, GetComponents<IEnabledListener>());
    PropagateToChildren(&IEnabledListener::OnEnabled);
}
void GameObject::OnDisabled()
{
    Object::OnDisabled();
    Propagate(&IEnabledListener::OnDisabled, GetComponents<IEnabledListener>());
    PropagateToChildren(&IEnabledListener::OnDisabled);
}

void GameObject::Destroy(GameObject *gameObject)
{
    ASSERT(gameObject);

    for (GameObject *child : gameObject->GetChildren())
    {
        GameObject::Destroy(child);
    }

    for (Component *comp : gameObject->GetComponents())
    {
        Component::Destroy(comp);
    }

    Object::DestroyObject(gameObject);
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
    Component *component = ComponentFactory::Create(componentClassName);
    return AddComponent(component, _index);
}

Component* GameObject::AddComponent(Component *component, int _index)
{
    if (component && !GetComponents().Contains(component))
    {
        Transform *transformComp = DCAST<Transform*>(component);
        if (transformComp)
        {
            ASSERT_SOFT_MSG(!HasComponent<Transform>(),
                            "A GameObject can not have more than one transform");
        }

        component->SetGameObject(this);

        const int index = (_index != -1 ? _index : GetComponents().Size());
        auto nextIt = m_components.Insert(index, component);

        auto it = m_components.Begin(); std::advance(it, index);
        if (!m_currentComponentsIterators.empty() &&
             m_currentComponentsIterators.top() == it)
        {
            m_currentComponentsIterators.top() = nextIt;
            m_increaseComponentsIterator = false;
        }

        if (transformComp) { p_transform = transformComp; }

        EventEmitter<IComponentListener>::PropagateToListeners(
                    &IComponentListener::OnComponentAdded, component, index);
    }
    return component;
}

const List<Component *> &GameObject::GetComponents() const
{
    return m_components;
}

bool GameObject::HasComponent(const String &className) const
{
    for (Component *comp : GetComponents())
    {
        if (comp->GetClassName() == className) { return true; }
    }
    return false;
}

Component *GameObject::GetComponentByGUID(const GUID &guid) const
{
    for (Component *comp : GetComponents())
    {
        if (comp->GetGUID() == guid) { return comp; }
    }
    return nullptr;
}

Scene* GameObject::GetScene() const
{
    Scene *scene = DCAST<Scene*>( const_cast<GameObject*>(this) );
    if (scene) { return scene; }

    GameObject *parent = GetParent();
    return parent ? parent->GetScene() : nullptr;
}

Transform *GameObject::GetTransform() const { return p_transform; }
RectTransform *GameObject::GetRectTransform() const
{
    return GetTransform() ? DCAST<RectTransform*>(GetTransform()) : nullptr;
}

void GameObject::SetName(const String &name)
{
    if (name != GetName())
    {
        String oldName = name;
        m_name = name;
        EventEmitter<INameListener>::PropagateToListeners(
                    &INameListener::OnNameChanged, this, oldName, GetName());
    }
}
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

void GameObject::SetVisible(bool visible)
{
    if (visible != IsVisible())
    {
        m_visible = visible;
        EventEmitter<IGameObjectVisibilityChangedListener>::PropagateToListeners(
            &IGameObjectVisibilityChangedListener::OnVisibilityChanged, this);
    }
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

GameObject *GameObject::GetChild(uint index) const
{
    if (index >= GetChildren().Size()) { return nullptr; }

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

bool GameObject::IsChildOf(const GameObject *_parent, bool recursive) const
{
    if (!GetParent()) { return false; }

    if (recursive)
    {
        return GetParent() == _parent || GetParent()->IsChildOf(_parent);
    }
    return GetParent() == _parent;
}

bool GameObject::IsVisible() const
{
    return m_visible;
}

void GameObject::SetParent(GameObject *newParent, int _index)
{
    ASSERT( newParent != this );
    ASSERT( !newParent || !newParent->IsChildOf(this) )

    if (!IsWaitingToBeDestroyed() && GetParent() != newParent)
    {
        GameObject *oldParent = GetParent();
        if (GetParent())
        {
            GetParent()->RemoveChild(this);
            GetParent()->ChildRemoved(this, oldParent);
        }

        p_parent = newParent;
        if (GetParent())
        {
            int index = (_index != -1 ? _index : GetParent()->GetChildren().Size());
            GetParent()->AddChild(this, index);
            GetParent()->ChildAdded(this, newParent);
        }

        EventEmitter<IChildrenListener>::
               PropagateToListeners(&IChildrenListener::OnParentChanged,
                                    oldParent, newParent);
        Propagate(&IChildrenListener::OnParentChanged,
                  GetComponents<IChildrenListener>(), oldParent, newParent);
    }
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

Rect GameObject::GetBoundingViewportRect(Camera *cam, bool includeChildren) const
{
    AABox bbox = GetAABBox(includeChildren);
    return cam->GetViewportBoundingRect(bbox);
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

template<class TFunction, class... Args>
void GameObject::PropagateToChildren(const TFunction &func, const Args&... args)
{
    m_currentChildrenIterators.push(m_children.Begin());
    while (m_currentChildrenIterators.top() != m_children.End())
    {
        m_increaseChildrenIterator = true;
        GameObject *child = *(m_currentChildrenIterators.top());
        GameObject::Propagate(func, child, args...);
        if (m_increaseChildrenIterator) { ++m_currentChildrenIterators.top(); }
    }
    m_currentChildrenIterators.pop();
}

template<class TFunction, class... Args>
void GameObject::PropagateToComponents(const TFunction &func, const Args&... args)
{
    m_currentComponentsIterators.push(m_components.Begin());
    while (m_currentComponentsIterators.top() != m_components.End())
    {
        m_increaseComponentsIterator = true;
        Component *comp = *(m_currentComponentsIterators.top());
        GameObject::Propagate(func, comp, args...);
        if (m_increaseComponentsIterator) { ++m_currentComponentsIterators.top(); }
    }
    m_currentComponentsIterators.pop();
}

void GameObject::CloneInto(ICloneable *clone) const
{
    Object::CloneInto(clone);

    GameObject *go = DCAST<GameObject*>(clone);
    go->SetName(m_name);
    go->SetParent(nullptr);

    for (GameObject *child : GetChildren())
    {
        if (!child->GetHideFlags().IsOn(HideFlag::DontClone))
        {
            GameObject *childClone = child->Clone();
            childClone->SetParent(go);
        }
    }

    for (Component *comp : GetComponents())
    {
        if (!comp->GetHideFlags().IsOn(HideFlag::DontClone))
        {
            go->AddComponent(comp->Clone());
        }
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
    oss << indent << " - " << GetName() << " " << GetComponents();
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

void GameObject::ImportXML(const XMLNode &xmlInfo)
{
    Serializable::ImportXML(xmlInfo);

    if (xmlInfo.Contains("Enabled"))
    { SetEnabled( xmlInfo.Get<bool>("Enabled") ); }

    if (xmlInfo.Contains("Visible"))
    { SetVisible( xmlInfo.Get<bool>("Visible") ); }

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
            if (comp) { comp->ImportXML(xmlChild); }
        }
    }
}

void GameObject::ExportXML(XMLNode *xmlInfo) const
{
    Serializable::ExportXML(xmlInfo);

    xmlInfo->Set("Enabled", IsEnabled());
    xmlInfo->Set("Visible", IsVisible());
    xmlInfo->Set("Name", GetName());
    xmlInfo->Set("DontDestroyOnLoad", IsDontDestroyOnLoad());

    for (Component *c : GetComponents())
    {
        if (c->GetHideFlags().IsOff(HideFlag::DontSerialize))
        {
            XMLNode xmlComp;
            c->ExportXML(&xmlComp);
            xmlInfo->AddChild(xmlComp);
        }
    }

    for (GameObject *child : GetChildren())
    {
        if (child->GetHideFlags().IsOff(HideFlag::DontSerialize))
        {
            XMLNode xmlChild;
            child->ExportXML(&xmlChild);
            xmlInfo->AddChild(xmlChild);
        }
    }
}
