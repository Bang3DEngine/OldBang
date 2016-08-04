#include "GameObject.h"

#include "Component.h"
#include "FileReader.h"
#include "SingletonManager.h"
#include "Transform.h"

#include "DirectionalLight.h"
#include "BehaviourHolder.h"
#include "MeshRenderer.h"
#include "PointLight.h"

#ifdef BANG_EDITOR
#include "Hierarchy.h"
#include "EditorScene.h"
#include "WindowEventManager.h"
#include "EditorSelectionGameObject.h"
#endif

GameObject::GameObject() : GameObject("")
{
}

GameObject::GameObject(const std::string &name) :
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
    this->_OnDestroy();

    while (m_children.size() > 0)
    {
        GameObject *child = *(m_children.begin());
        child->SetParent(nullptr);
        delete child;
    }
}

void GameObject::SetParent(GameObject *newParent, bool keepWorldTransform)
{
    if (m_parent != newParent)
    {
        if (m_parent)
        {
            m_parent->m_children.remove(this);

            #ifdef BANG_EDITOR
            WindowEventManager::NotifyChildRemoved(this);
            #endif
        }

        if(keepWorldTransform)
        {
            m_transform->SetLocalPosition(m_transform->LocalToWorldPoint(m_transform->GetLocalPosition()));
            // TODO
            // SetRotation
            // SetScale
        }

        m_parent = newParent;

        if (m_parent)
        {
            m_parent->m_children.push_back(this);

            if(keepWorldTransform)
            {
                m_transform->SetLocalPosition(m_transform->WorldToLocalPoint(m_transform->GetLocalPosition()));
                // TODO
                // SetRotation
                // SetScale
            }

            #ifdef BANG_EDITOR
            WindowEventManager::NotifyChildAdded(this);
            #endif
        }
    }
}


Scene *GameObject::GetScene()
{
    if (IsScene()) { return (Scene*) this; }
    if (m_parent) return m_parent->GetScene();
    return nullptr;
}

GameObject *GameObject::GetParent() const
{
    return m_parent;
}

const std::string GameObject::GetName() const
{
    return m_name;
}


const std::list<Component *> &GameObject::GetComponents() const { return m_components; }

const std::list<GameObject *> GameObject::GetChildren() const
{
    std::list<GameObject *> cc;
    for (auto c = m_children.begin(); c != m_children.end(); ++c)
    {
        if (!(*c)->IsEditorGameObject()) cc.push_back(*c);
    }
    return cc;
}

std::list<GameObject*> GameObject::GetChildrenRecursively() const
{
    std::list<GameObject*> cc;
    for (auto c = m_children.begin(); c != m_children.end(); ++c)
    {
        cc.splice(cc.end(), (*c)->GetChildrenRecursively()); //concat
        if (!(*c)->IsEditorGameObject()) cc.push_back(*c);
    }
    return cc;
}

#ifdef BANG_EDITOR
std::list<GameObject*> GameObject::GetChildrenRecursivelyEditor() const
{
    std::list<GameObject*> cc;
    for (auto c = m_children.begin(); c != m_children.end(); ++c)
    {
        cc.splice(cc.end(), (*c)->GetChildrenRecursivelyEditor());
        cc.push_back(*c);
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
        Box bc = child->GetLocalBoundingBox();
        b = Box::Union(b, bc);
    }

    return b;
}

Box GameObject::GetLocalBoundingBox() const
{
    Box b = GetObjectBoundingBox();
    if (CAN_USE_COMPONENT(m_transform))
    {
        Matrix4 mat;
        m_transform->GetObjectModelMatrix(&mat);
        b = mat * b; //Apply transform to Box
    }
    return b;
}

Box GameObject::GetBoundingBox() const
{
    Box b = GetObjectBoundingBox();
    if (CAN_USE_COMPONENT(m_transform))
    {
        Matrix4 mat;
        m_transform->GetModelMatrix(&mat);
        b = mat * b;
    }
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
            Logger_Error("A gameObject must contain one and only one Transform.");
            delete c;
            return;
        }
        m_transform = t;
    }

    c->SetGameObject(this);
    m_components.push_back(c);
    c->_OnStart();
}

#ifdef BANG_EDITOR
void GameObject::MoveComponent(Component *c, int distance)
{
    for (auto comp = m_components.begin(); comp != m_components.end(); ++comp)
    {
        if (c == *comp)
        {
            auto comp1 = comp;
            std::advance(comp1, 1);
            m_components.erase(comp, comp1);
            std::advance(comp1, distance);
            m_components.insert(comp1, 1, c);
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

    for (auto comp = m_components.begin(); comp != m_components.end(); ++comp)
    {
        if (c == *comp)
        {
            m_components.erase(comp);
            break;
        }
    }
}

GameObject *GameObject::GetChild(const std::string &name) const
{
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        GameObject *child = (*it);
        if (child->m_name == name)
        {
            return child;
        }
    }
    return nullptr;
}

void GameObject::SetName(const std::string &name)
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

GameObject *GameObject::Find(const std::string &name)
{
    Scene *scene = Canvas::GetCurrentScene();
    return scene->FindInChildren(name);
}

GameObject *GameObject::FindInChildren(const std::string &name)
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
        std::string tagName = xmlChild->GetTagName();
        if (tagName == "GameObject") // It's a child
        {
            GameObject *child = new GameObject();
            child->ReadXMLInfo(xmlChild);
            child->SetParent(this);
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
        std::list<GameObject*> &selectedEntities )
{
    if (IsEditorGameObject() || IsScene()) return;

    bool selected = false;
    for (auto it : selectedEntities)
    {
        if ( it == this )
        {
            selected = true;
            break;
        }
    }

    if (selected)
    {
        if (!m_isSelectedInHierarchy)
        {
            m_selectionGameObject = new EditorSelectionGameObject(this);
            m_selectionGameObject->SetParent(Canvas::GetCurrentScene());
        }
    }
    else
    {
        if (m_isSelectedInHierarchy)
        {
            m_selectionGameObject->SetParent(nullptr);
        }
    }

    m_isSelectedInHierarchy = selected;
}
#endif

void GameObject::SetEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool GameObject::IsEnabled()
{
    return m_enabled && (!m_parent ? true : m_parent->IsEnabled());
}

const std::string GameObject::ToString() const
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

void GameObject::_OnStart()
{
    PROPAGATE_EVENT(_OnStart, m_children);
    OnStart();
}


void GameObject::_OnUpdate()
{

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
    PROPAGATE_EVENT(_OnPreRender, m_children);
    PROPAGATE_EVENT(_OnPreRender, m_components);
    OnPreRender();
}

void GameObject::_OnRender ()
{
    #ifdef BANG_EDITOR
    EditorScene *scene = static_cast<EditorScene*>(Scene::GetCurrentScene());
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
    PROPAGATE_EVENT(_OnDestroy, m_components);
    //No need to propagate _OnDestroy to children,
    //since the "delete child" itself propagates it (look at the destructor)
    OnDestroy();
}

#ifdef BANG_EDITOR
void GameObject::_OnDrawGizmos()
{
    Gizmos::Reset();
    EditorScene *scene = static_cast<EditorScene*>(Scene::GetCurrentScene());
    if (!scene->GetSelectionFramebuffer()->IsPassing())
    {
        PROPAGATE_EVENT(_OnDrawGizmos, m_children);
    }
    PROPAGATE_EVENT(_OnDrawGizmos, m_components);
    OnDrawGizmos();
}

void GameObject::_OnDrawGizmosNoDepth()
{
    Gizmos::Reset();
    EditorScene *scene = static_cast<EditorScene*>(Scene::GetCurrentScene());
    if (!scene->GetSelectionFramebuffer()->IsPassing())
    {
        PROPAGATE_EVENT(_OnDrawGizmosNoDepth, m_children);
    }
    PROPAGATE_EVENT(_OnDrawGizmosNoDepth, m_components);
    OnDrawGizmosNoDepth();
}
#endif
