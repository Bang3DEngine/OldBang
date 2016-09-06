#include "GameObject.h"

#include "FileReader.h"
#include "SceneManager.h"
#include "SingletonManager.h"

#include "Component.h"
#include "PointLight.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "BehaviourHolder.h"
#include "DirectionalLight.h"

#ifdef BANG_EDITOR
#include "Hierarchy.h"
#include "EditorScene.h"
#include "WindowEventManager.h"
#include "EditorSelectionGameObject.h"
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

    if (parent)
    {
        parent->m_children.Remove(this); // Remove me from parent
    }

    #ifdef BANG_EDITOR
    WindowEventManager::NotifyGameObjectDestroyed(this);
    #endif
}

void GameObject::SetParent(GameObject *newParent, bool keepWorldTransform)
{
    if (m_parent != newParent)
    {
        if (m_parent)
        {
            m_parent->m_children.Remove(this);
        }

        if(keepWorldTransform)
        {
            m_transform->SetLocalPosition(m_parent->m_transform->LocalToWorldPoint(m_transform->GetLocalPosition()));
            // TODO
            // SetRotation
            // SetScale
        }

        m_parent = newParent;

        if (m_parent)
        {
            m_parent->m_children.PushBack(this);

            if(keepWorldTransform)
            {
                m_transform->SetLocalPosition(m_parent->m_transform->WorldToLocalPoint(m_transform->GetLocalPosition()));
                // TODO
                // SetRotation
                // SetScale
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
    return nullptr;
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
    for (GameObject *c : m_children)
    {
        if (!c->IsEditorGameObject()) cc.PushBack(c);
    }
    return cc;
}

List<GameObject*> GameObject::GetChildrenRecursively() const
{
    List<GameObject*> cc;
    for (GameObject *c : m_children)
    {
        List<GameObject*> childChildren = c->GetChildrenRecursively();
        cc.Splice(cc.End(), childChildren); //concat
        if (!c->IsEditorGameObject()) cc.PushBack(c);
    }
    return cc;
}

#ifdef BANG_EDITOR
List<GameObject*> GameObject::GetChildrenRecursivelyEditor() const
{
    List<GameObject*> cc;
    for (GameObject *c : m_children)
    {
        List<GameObject*> childChildren = c->GetChildrenRecursivelyEditor();
        cc.Splice(cc.End(), childChildren);
        cc.PushBack(c);
    }
    return cc;
}
#endif

Box GameObject::GetObjectBoundingBox() const
{
    Box b;
    MeshRenderer *mr = GetComponent<MeshRenderer>();
    if (CAN_USE_COMPONENT(mr))
    {
        const Mesh *m = mr->GetMesh();
        if (m)
        {
            b = m->GetBoundingBox();
        }
    }

    for (GameObject *child : m_children)
    {
        #ifdef BANG_EDITOR
        if (child->IsEditorGameObject() ||
            child->IsDraggedGameObject()) continue;
        #endif

        Box bc = child->GetLocalBoundingBox();
        b = Box::Union(b, bc);
    }

    return b;
}

Box GameObject::GetLocalBoundingBox() const
{
    Box b = GetObjectBoundingBox();
    Matrix4 mat = transform->GetLocalToParentMatrix();
    b = mat * b; //Apply transform to Box
    return b;
}

Box GameObject::GetBoundingBox() const
{
    Box b = GetObjectBoundingBox();
    Matrix4 mat; transform->GetLocalToWorldMatrix(&mat);
    b = mat * b;
    return b;
}

Sphere GameObject::GetObjectBoundingSphere() const
{
    return Sphere::FromBox(GetObjectBoundingBox());
}

Sphere GameObject::GetLocalBoundingSphere() const
{
    return Sphere::FromBox(GetLocalBoundingBox());
}

Sphere GameObject::GetBoundingSphere() const
{
    return Sphere::FromBox(GetBoundingBox());
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
    if (t)
    {
        return;
    }
    m_components.Remove(c);
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
                transform->ReadXMLInfo(xmlChild);
                c = transform;
            }
            else if (tagName == "MeshRenderer")
            {
                MeshRenderer *mr = AddComponent<MeshRenderer>();
                mr->ReadXMLInfo(xmlChild);
                c = mr;
            }
            else if (tagName == "Camera")
            {
                Camera *cam = AddComponent<Camera>();
                cam->ReadXMLInfo(xmlChild);
                c = cam;
            }
            else if (tagName == "BehaviourHolder")
            {
                BehaviourHolder *bh = AddComponent<BehaviourHolder>();
                bh->ReadXMLInfo(xmlChild);
                c = bh;
            }
            else if (tagName == "DirectionalLight")
            {
                DirectionalLight *dl = AddComponent<DirectionalLight>();
                dl->ReadXMLInfo(xmlChild);
                c = dl;
            }
            else if (tagName == "PointLight")
            {
                PointLight *pl = AddComponent<PointLight>();
                pl->ReadXMLInfo(xmlChild);
                c = pl;
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
bool GameObject::IsSelectedInHierarchy() const
{
    return m_isSelectedInHierarchy;
}

void GameObject::OnTreeHierarchyGameObjectsSelected(
        List<GameObject*> &selectedEntities )
{
    if (IsEditorGameObject() || IsScene()) return;

    bool selected = selectedEntities.Contains(this);
    bool wasSelected = IsSelectedInHierarchy();
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

bool GameObject::IsChildOf(const GameObject *goParent) const
{
    if (!m_parent)
    {
        return false;
    }
    return (m_parent == goParent || m_parent->IsChildOf(goParent));
}

void GameObject::_OnStart()
{
    ISceneEventListener::_OnStart();

    PROPAGATE_EVENT(_OnStart, m_children);
    OnStart();
}


void GameObject::_OnUpdate()
{
    ISceneEventListener::_OnUpdate();

    #ifdef BANG_EDITOR
    bool canUpdate = Toolbar::GetInstance()->IsPlaying() || IsEditorGameObject();
    #else
    bool canUpdate = true;
    #endif

    if (canUpdate)
    {
        PROPAGATE_EVENT(_OnUpdate, m_components);
    }

    PROPAGATE_EVENT(_OnUpdate, m_children);

    if (canUpdate)
    {
        OnUpdate();
    }
}

void GameObject::_OnPreRender ()
{
    ISceneEventListener::_OnPreRender();

    PROPAGATE_EVENT(_OnPreRender, m_children);
    PROPAGATE_EVENT(_OnPreRender, m_components);
    OnPreRender();
}

void GameObject::_OnRender ()
{
    ISceneEventListener::_OnRender();

    #ifdef BANG_EDITOR
    EditorScene *scene = static_cast<EditorScene*>(SceneManager::GetActiveScene());
    if (!scene->GetSelectionFramebuffer()->IsPassing())
    #endif
    {
        PROPAGATE_EVENT(_OnRender, m_children);
    }
    PROPAGATE_EVENT(_OnRender, m_components);
    OnRender();
}

void GameObject::_OnDestroy()
{
    ISceneEventListener::_OnDestroy();

    PROPAGATE_EVENT(_OnDestroy, m_components);
    //No need to propagate _OnDestroy to children,
    //since the "delete child" itself propagates it (look at the destructor)
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
    ISceneEventListener::_OnDrawGizmos();

    EditorScene *scene = static_cast<EditorScene*>(SceneManager::GetActiveScene());
    if (!scene->GetSelectionFramebuffer()->IsPassing())
    {
        PROPAGATE_EVENT(_OnDrawGizmos, m_children);
    }
    PROPAGATE_EVENT(_OnDrawGizmos, m_components);

    OnDrawGizmos();
    #endif
}

void GameObject::_OnDrawGizmosNoDepth()
{
    #ifdef BANG_EDITOR
    ISceneEventListener::_OnDrawGizmosNoDepth();

    EditorScene *scene = static_cast<EditorScene*>(SceneManager::GetActiveScene());
    if (!scene->GetSelectionFramebuffer()->IsPassing())
    {
        PROPAGATE_EVENT(_OnDrawGizmosNoDepth, m_children);
    }
    PROPAGATE_EVENT(_OnDrawGizmosNoDepth, m_components);
    OnDrawGizmosNoDepth();
    #endif
}

void GameObject::OnDrawGizmos()
{
}

void GameObject::OnDrawGizmosNoDepth()
{
}

