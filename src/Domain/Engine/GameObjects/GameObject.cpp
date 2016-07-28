#include "GameObject.h"
#include "Scene.h"
#include "Component.h"
#include "FileReader.h"
#include "SingletonManager.h"
#include "Transform.h"

#include "DirectionalLight.h"
#include "BehaviourHolder.h"
#include "PointLight.h"

#ifdef BANG_EDITOR
#include "Hierarchy.h"
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
    go->SetRenderLayer(m_renderLayer);
    go->SetParent(nullptr);

    for (GameObject *child : m_children)
    {
        if (child->IsEditorGameObject()) continue;
        GameObject *childClone = static_cast<GameObject*>(child->Clone());
        childClone->SetParent(go);
    }

    for (Component *comp : m_comps)
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

unsigned char GameObject::GetRenderLayer() const
{
    return m_renderLayer;
}

const std::list<Component *> &GameObject::GetComponents() const { return m_comps; }

const std::list<GameObject *> GameObject::GetChildren() const
{
    std::list<GameObject *> cc;
    for (auto c = m_children.begin(); c != m_children.end(); ++c)
    {
        if (!(*c)->IsEditorGameObject()) cc.push_back(*c);
    }
    return cc;
}

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
            Logger_Error("A gameObject can only contain one unique Transform.");
            delete c;
            return;
        }

        m_transform = t;
    }

    c->m_gameObject = this;
    m_comps.push_back(c);
    c->_OnStart();
}

void GameObject::MoveComponent(Component *c, int distance)
{
    for (auto comp = m_comps.begin(); comp != m_comps.end(); ++comp)
    {
        if (c == *comp)
        {
            auto comp1 = comp;
            std::advance(comp1, 1);
            m_comps.erase(comp, comp1);
            std::advance(comp1, distance);
            m_comps.insert(comp1, 1, c);
            break;
        }
    }
}

Transform *GameObject::GetTransform() const
{
    return m_transform;
}

void GameObject::RemoveComponent(Component *c)
{
    for (auto comp = m_comps.begin(); comp != m_comps.end(); ++comp)
    {
        if (c == *comp)
        {
            m_comps.erase(comp);
            break;
        }
    }

    Transform *t = dynamic_cast<Transform*>(c);
    if (t)
    {
        m_transform = nullptr;
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

void GameObject::SetRenderLayer(unsigned char layer)
{
    this->m_renderLayer = layer;
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

void GameObject::ReadXMLNode(const XMLNode *xmlNode)
{
    // FileReader::RegisterNextPointerId(f, this);
    m_enabled = xmlNode->GetBool("enabled");
    SetName( xmlNode->GetString("name") );  //Read GameObject name

    for ( XMLNode *xmlChild : xmlNode->GetChildren() )
    {
        std::string tagName = xmlChild->GetTagName();
        if (tagName == "GameObject") // It's a child
        {
            GameObject *child = new GameObject();
            child->ReadXMLNode(xmlChild);
            child->SetParent(this);
        }
        else // It's a Component
        {
            Component *c = nullptr;
            bool isTransform = false;
            if (tagName == "Transform")
            {
                transform->ReadXMLNode(xmlChild);
                c = transform;
                isTransform = true;
            }
            else if (tagName == "MeshRenderer")
            {
                MeshRenderer *mr = new MeshRenderer();
                mr->ReadXMLNode(xmlChild);
                c = mr;
            }
            else if (tagName == "Camera")
            {
                Camera *cam = new Camera();
                cam->ReadXMLNode(xmlChild);
                c = cam;
            }
            else if (tagName == "BehaviourHolder")
            {
                BehaviourHolder *bh = new BehaviourHolder();
                bh->ReadXMLNode(xmlChild);
                c = bh;
            }
            else if (tagName == "DirectionalLight")
            {
                DirectionalLight *dl = new DirectionalLight();
                dl->ReadXMLNode(xmlChild);
                c = dl;
            }
            else if (tagName == "PointLight")
            {
                PointLight *pl = new PointLight();
                pl->ReadXMLNode(xmlChild);
                c = pl;
            }

            if (c && !isTransform)
            {
                AddComponent(c);
            }
        }
    }
}

void GameObject::GetXMLNode(XMLNode *xmlNode) const
{
    xmlNode->SetTagName("GameObject");
    xmlNode->SetAttribute("id", this);
    xmlNode->SetAttribute("enabled", m_enabled);
    xmlNode->SetAttribute("name", m_name);

    for (Component *c : m_comps)
    {
        XMLNode *xmlComp = new XMLNode();
        c->GetXMLNode(xmlComp);
        xmlNode->AddChild(xmlComp);
    }

    for (GameObject *go : m_children)
    {
        XMLNode *child = new XMLNode();
        go->GetXMLNode(child);
        xmlNode->AddChild(child);
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
        PROPAGATE_EVENT(_OnUpdate, m_comps);
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

    if (this->m_renderLayer == GetScene()->m_currentRenderLayer)
    {
        PROPAGATE_EVENT(_OnPreRender, m_comps);
        OnPreRender();
    }
}

void GameObject::_OnRender ()
{
    PROPAGATE_EVENT(_OnRender, m_children);

    if (this->m_renderLayer == GetScene()->m_currentRenderLayer)
    {
        PROPAGATE_EVENT(_OnRender, m_comps);
        OnRender();
    }
}

void GameObject::_OnDestroy()
{
    PROPAGATE_EVENT(_OnDestroy, m_comps);
    //No need to propagate _OnDestroy to children,
    //since the "delete child" itself propagates it (look at the destructor)
    OnDestroy();
}

