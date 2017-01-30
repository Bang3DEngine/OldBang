#include "GameObject.h"

#include <iostream>

#include "Debug.h"
#include "Scene.h"
#include "Canvas.h"
#include "UIText.h"
#include "Camera.h"
#include "XMLNode.h"
#include "UIImage.h"
#include "Material.h"
#include "Component.h"
#include "Transform.h"
#include "PointLight.h"
#include "EditorState.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "GraphicPipeline.h"
#include "BehaviourHolder.h"
#include "SingletonManager.h"
#include "DirectionalLight.h"

#ifdef BANG_EDITOR
#include "Hierarchy.h"
#include "EditorScene.h"
#include "WindowEventManager.h"
#include "SelectionFramebuffer.h"
#include "EditorSelectionGameObject.h"
#include "EditorPlayStopFlowController.h"
#endif

GameObject::GameObject() : GameObject("")
{
}

GameObject::GameObject(const String &name) :
    m_name(name)
{
    AddComponent<Transform>();
}


void GameObject::CloneInto(ICloneable *clone) const
{
    GameObject *go = static_cast<GameObject*>(clone);

    go->SetName(m_name);
    go->SetParent(nullptr);

    for (GameObject *child : m_children)
    {
        if (child->IsEditorGameObject()) continue;
        GameObject *childClone = static_cast<GameObject*>(child->Clone());
        childClone->SetParent(go);
    }

    for (Component *comp : m_components)
    {
        Transform* t = dynamic_cast<Transform*>(comp);
        if (!t)
        {
            go->AddComponent( static_cast<Component*>(comp->Clone()) );
        }
        else
        {
            m_transform->CloneInto(go->transform);
        }
    }
}

ICloneable* GameObject::Clone() const
{
    GameObject *clone = new GameObject();
    CloneInto(clone);
    return clone;
}

GameObject::~GameObject()
{
    _OnDestroy();

    while (!m_children.Empty())
    {
        GameObject *child = m_children.Front();
        delete child;
    }

    while (!m_components.Empty())
    {
        Component *comp = m_components.Front();
        m_components.PopFront();
        delete comp;
    }

    SetParent(nullptr);

    #ifdef BANG_EDITOR
    WindowEventManager::NotifyGameObjectDestroyed(this);
    #endif
}

void GameObject::SetParent(GameObject *newParent, bool keepWorldTransform, GameObject *aboveThisChild)
{
    if (m_parent)
    {
        m_parent->m_children.Remove(this);
    }

    if (keepWorldTransform)
    {
        // TODO: Not working yet (sometimes scaling breaks)

        Matrix4 oldParentToWorld;
        parent->transform->GetLocalToWorldMatrix(&oldParentToWorld);

        Matrix4 worldToNewParent;
        if (newParent)
        {
            newParent->transform->GetLocalToWorldMatrix(&worldToNewParent);
            worldToNewParent = worldToNewParent.Inversed();
        }

        Matrix4 keepWorldTransformMatrix =
                worldToNewParent * oldParentToWorld * transform->GetLocalToParentMatrix();

        Transform t = Transform::FromTransformMatrix(keepWorldTransformMatrix);
        transform->SetLocalPosition(t.GetLocalPosition());
        transform->SetLocalRotation(t.GetLocalRotation());
        transform->SetLocalScale   (t.GetLocalScale());
    }

    m_parent = newParent;

    if (m_parent)
    {
        if (!aboveThisChild)
        {
            m_parent->m_children.PushBack(this); // Add it to the end
        }
        else
        {
            bool itemToBeAboveOfFound = false;
            for (auto it = m_parent->m_children.Begin(); it != m_parent->m_children.End(); ++it)
            {
                if (aboveThisChild == *it)
                {
                    m_parent->m_children.InsertBefore(it, this);
                    itemToBeAboveOfFound = true;
                    break;
                }
            }

            if (!itemToBeAboveOfFound)
            {
                m_parent->m_children.PushBack(this); // Just in case, add to the end
            }
        }
    }
}


Scene *GameObject::GetScene()
{
    if (IsScene()) { return (Scene*) this; }
    if (m_parent) return m_parent->GetScene();
    return nullptr;
}

bool GameObject::IsInsideScene() const
{
    if (IsScene()) { return true; }
    if (parent) return parent->IsInsideScene();
    return false;
}

GameObject *GameObject::GetParent() const
{
    return m_parent;
}

const String GameObject::GetName() const
{
    return m_name;
}


const List<Component *> &GameObject::GetComponents() const
{
    return m_components;
}

const List<GameObject *> GameObject::GetChildren() const
{
    List<GameObject *> cc;
    //Debug_Log(m_children);
    //Debug_Log(m_children.Size());
    for (GameObject *c : m_children)
    {
        //Debug_Log("    " << c);
        if (!c->IsEditorGameObject()) cc.PushBack(c);
    }
    return cc;
}

const List<GameObject *> GameObject::GetChildrenEditor() const
{
    List<GameObject *> cc;
    for (GameObject *c : m_children)
    {
        cc.PushBack(c);
    }
    return cc;
}


List<GameObject*> GameObject::GetChildrenRecursively() const
{
    List<GameObject*> cc;
    for (GameObject *c : m_children)
    {
        if (!c->IsEditorGameObject()) cc.PushBack(c);
        List<GameObject*> childChildren = c->GetChildrenRecursively();
        cc.Splice(cc.Begin(), childChildren); //concat
    }
    return cc;
}

#ifdef BANG_EDITOR
List<GameObject*> GameObject::GetChildrenRecursivelyEditor() const
{
    List<GameObject*> cc;
    for (GameObject *c : m_children)
    {
        cc.PushBack(c);
        List<GameObject*> childChildren = c->GetChildrenRecursivelyEditor();
        cc.Splice(cc.Begin(), childChildren);
    }
    return cc;
}
#endif

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
        if (CAN_USE_COMPONENT(rend))
        {
            aabBox = AABox::Union(aabBox, rend->GetAABBox());
        }
    }

    if (includeChildren)
    {
        for (GameObject *child : m_children)
        {
            #ifdef BANG_EDITOR
            if (child->IsEditorGameObject() ||
                child->IsDraggedGameObject()) continue;
            #endif

            AABox aabBoxChild = child->GetObjectAABBox(true);
            Matrix4 mat = child->transform->GetLocalToParentMatrix();
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
    transform->GetLocalToWorldMatrix(&mat);
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

void GameObject::AddComponent(Component *c)
{
    Transform *t = dynamic_cast<Transform*>(c);
    if (t)
    {
        if (HasComponent<Transform>())
        {
            Debug_Error("A gameObject must contain one and only one Transform.");
            delete c;
            return;
        }
        m_transform = t;
    }

    c->SetGameObject(this);
    m_components.PushBack(c);
}

#ifdef BANG_EDITOR
void GameObject::MoveComponent(Component *c, int distance)
{
    for (auto comp = m_components.Begin(); comp != m_components.End(); ++comp)
    {
        if (c == *comp)
        {
            auto comp1 = comp;
            std::advance(comp1, 1);
            m_components.Remove(comp, comp1);
            std::advance(comp1, distance);
            m_components.Insert(comp1, 1, c);
            break;
        }
    }
}
#endif

Transform *GameObject::GetTransform() const
{
    return m_transform;
}

void GameObject::RemoveComponent(Component *c)
{
    Transform *t = dynamic_cast<Transform*>(c);
    if (!t)
    {
        m_components.Remove(c);
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
    this->m_name = name;
    #ifdef BANG_EDITOR
    Hierarchy::GetInstance()->OnGameObjectNameChanged(this);
    #endif
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

bool GameObject::IsEditorGameObject() const
{
    return false;
}

bool GameObject::IsScene() const
{
    return false;
}

GameObject *GameObject::Find(const String &name)
{
    Scene *scene = SceneManager::GetActiveScene();
    return scene->FindInChildren(name);
}

GameObject *GameObject::FindInChildren(const String &name)
{
    for (GameObject *child : GetChildren())
    {
        if (child->name == name)
        {
            return child;
        }
        else
        {
            GameObject *found = child->FindInChildren(name);
            if (found)
            {
                return found;
            }
        }
    }
    return nullptr;
}

void GameObject::ReadXMLInfo(const XMLNode *xmlInfo)
{
    IFileable::ReadXMLInfo(xmlInfo);

    SetEnabled( xmlInfo->GetBool("enabled") );
    SetName( xmlInfo->GetString("name") );  //Read GameObject name

    for ( XMLNode *xmlChild : xmlInfo->GetChildren() )
    {
        String tagName = xmlChild->GetTagName();
        if (tagName == "GameObject") // It's a child
        {
            GameObject *child = new GameObject();
            // Important: this line must be before the ReadXMLInfo, because
            // it will avoid Behaviour to be compiled if it's not in the scene,
            // and it only can really know if its in scene when executing the Read if
            // we set it before the Read, not after. Wtf I don't know how to write this xds
            child->SetParent(this);
            child->ReadXMLInfo(xmlChild);
        }
        else // It's a Component
        {
            Component *c = nullptr;
            if (tagName == "Transform")
            {
                c = transform;
            }
            else if (tagName == "MeshRenderer")
            {
                c = AddComponent<MeshRenderer>();
            }
            else if (tagName == "Camera")
            {
                c = AddComponent<Camera>();
            }
            else if (tagName == "BehaviourHolder")
            {
                c = AddComponent<BehaviourHolder>();
            }
            else if (tagName == "DirectionalLight")
            {
                c = AddComponent<DirectionalLight>();
            }
            else if (tagName == "PointLight")
            {
                c = AddComponent<PointLight>();
            }
            else if (tagName == "Canvas")
            {
                c = AddComponent<Canvas>();
            }
            else if (tagName == "UIImage")
            {
                c = AddComponent<UIImage>();
            }
            else if (tagName == "UIText")
            {
                c = AddComponent<UIText>();
            }

            if (c)
            {
                c->ReadXMLInfo(xmlChild);
            }
        }
    }
}

void GameObject::FillXMLInfo(XMLNode *xmlInfo) const
{
    IFileable::FillXMLInfo(xmlInfo);

    xmlInfo->SetTagName("GameObject");
    xmlInfo->SetPointer("id", this,
                        {XMLProperty::Hidden,
                         XMLProperty::Readonly});
    xmlInfo->SetBool("enabled", m_enabled,
                     {XMLProperty::Hidden,
                      XMLProperty::Readonly});
    xmlInfo->SetString("name", m_name,
                       {XMLProperty::Hidden,
                        XMLProperty::Readonly});

    for (Component *c : m_components)
    {
        XMLNode *xmlComp = new XMLNode();
        c->FillXMLInfo(xmlComp);
        xmlInfo->AddChild(xmlComp);
    }

    for (GameObject *child : m_children)
    {
        if (!child->IsEditorGameObject())
        {
            XMLNode *xmlChild = new XMLNode();
            child->FillXMLInfo(xmlChild);
            xmlInfo->AddChild(xmlChild);
        }
    }
}


#ifdef BANG_EDITOR
bool GameObject::IsSelected() const
{
    return m_isSelectedInHierarchy;
}

void GameObject::OnHierarchyGameObjectsSelected(List<GameObject*> &selectedEntities)
{
    if (IsEditorGameObject() || IsScene()) return;

    bool selected = selectedEntities.Contains(this);
    bool wasSelected = IsSelected();
    m_isSelectedInHierarchy = selected;
    if (!wasSelected && selected)
    {
        m_selectionGameObject = new EditorSelectionGameObject(this);
        m_selectionGameObject->SetParent(SceneManager::GetActiveScene());
    }
    else if (wasSelected && !selected && m_selectionGameObject)
    {
        delete m_selectionGameObject;
        m_selectionGameObject = nullptr;
    }
}
#endif

void GameObject::SetEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool GameObject::IsEnabled() const
{
    return m_enabled && (!m_parent ? true : m_parent->IsEnabled());
}

#ifdef BANG_EDITOR
bool GameObject::IsDraggedGameObject() const
{
    return m_isDragged || (parent && parent->IsDraggedGameObject());
}
#endif

const String GameObject::ToString() const
{
    std::ostringstream oss;
    oss << "GameObject: " << m_name << "(" << ((void*)this) << ")";
    return oss.str();
}

void GameObject::OnMouseEnter(bool fromChildren)
{
    if (m_parent)
    {
        m_parent->OnMouseEnter(true);
    }
}

void GameObject::OnMouseOver(bool fromChildren)
{
    if (m_parent)
    {
        m_parent->OnMouseOver(true);
    }
}

void GameObject::OnMouseExit(bool fromChildren)
{
    if (m_parent)
    {
        m_parent->OnMouseExit(true);
    }
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

void GameObject::_OnStart()
{
    OnStart();
    PROPAGATE_EVENT(_OnStart, m_components);
    PROPAGATE_EVENT(_OnStart, m_children);
}


void GameObject::_OnUpdate()
{
    #ifdef BANG_EDITOR
    bool canUpdate = EditorPlayStopFlowController::IsPlaying() || IsEditorGameObject();
    #else
    bool canUpdate = true;
    #endif

    if (canUpdate)
    {
        OnUpdate();
    }

    if (canUpdate)
    {
        PROPAGATE_EVENT(_OnUpdate, m_components);
    }

    PROPAGATE_EVENT(_OnUpdate, m_children);
}

void GameObject::_OnDestroy()
{
    //No need to propagate _OnDestroy to children, just on components,
    //since the "delete child" itself propagates it (look at the destructor)
    PROPAGATE_EVENT(_OnDestroy, m_components);

    OnDestroy();
}

#ifdef BANG_EDITOR

void GameObject::OnDragEnterMaterial(Material *m)
{
    m_materialsBeforeDrag.Clear();

    List<Renderer*> rends = GetComponents<Renderer>();
    for (Renderer *r : rends)
    {
        m_materialsBeforeDrag.PushBack(r->GetMaterial());
        r->SetMaterial(m);
    }
}

void GameObject::OnDragLeaveMaterial(Material *m)
{
    int i = 0;
    List<Renderer*> rends = GetComponents<Renderer>();
    for (Renderer *r : rends)
    {
        r->SetMaterial(m_materialsBeforeDrag[i]);
        ++i;
    }
}

void GameObject::OnDropMaterial(Material *m)
{
    List<Renderer*> rends = GetComponents<Renderer>();
    for (Renderer *r : rends)
    {
        r->SetMaterial(m);
    }
}

#endif

void GameObject::_OnDrawGizmos()
{
    #ifdef BANG_EDITOR
    ASSERT(EditorState::ShowGizmosEnabled());

    PROPAGATE_EVENT(_OnDrawGizmos, m_children);  // The order matters

    GraphicPipeline *gp = GraphicPipeline::GetActive();
    SelectionFramebuffer *sfb = gp->GetSelectionFramebuffer();
    if (sfb->IsPassing())
    {
        sfb->PrepareNextGameObject(this);
    }
    PROPAGATE_EVENT(_OnDrawGizmos, m_components);  // The order matters
    OnDrawGizmos();

    #endif
}

void GameObject::_OnDrawGizmosOverlay()
{
    #ifdef BANG_EDITOR
    ASSERT(EditorState::ShowGizmosEnabled());

    GraphicPipeline *gp = GraphicPipeline::GetActive();
    SelectionFramebuffer *sfb = gp->GetSelectionFramebuffer();
    if (sfb->IsPassing())
    {
        sfb->PrepareNextGameObject(this);
    }
    PROPAGATE_EVENT(_OnDrawGizmosOverlay, m_components);  // The order matters
    OnDrawGizmosOverlay();

    PROPAGATE_EVENT(_OnDrawGizmosOverlay, m_children); // The order matters

    #endif
}

void GameObject::OnDrawGizmos()
{
}

void GameObject::OnDrawGizmosOverlay()
{
}

