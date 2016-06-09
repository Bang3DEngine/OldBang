#include "GameObject.h"
#include "Scene.h"
#include "Component.h"
#include "FileReader.h"
#include "WindowEventManager.h"

#include "WindowMain.h"
#include "EditorSelectionGameObject.h"

GameObject::GameObject() : GameObject("")
{
}

GameObject::GameObject(const std::string &name) : name(name)
{
}

GameObject::~GameObject()
{
    this->_OnDestroy();
    for(auto it = children.begin(); it != children.end();)
    {
        GameObject *child = *it;
        it = this->RemoveChildWithoutNotifyingHierarchy(it);
        delete child;
    }
}


Scene *GameObject::GetScene()
{
    if(isScene) { return (Scene*) this; }
    if(parent != nullptr) return parent->GetScene();
    return nullptr;
}

GameObject *GameObject::GetParent() const
{
    return parent;
}

const std::string GameObject::GetName() const { return name; }

unsigned char GameObject::GetRenderLayer() const
{
    return renderLayer;
}

const std::list<Component *> &GameObject::GetComponents() const { return comps; }

const std::list<GameObject *> GameObject::GetChildren() const
{
    std::list<GameObject *> cc;
    for(auto c = children.begin(); c != children.end(); ++c)
    {
        if(!(*c)->IsEditorGameObject()) cc.push_back(*c);
    }
    return cc;
}

Box GameObject::GetObjectBoundingBox() const
{
    Box b;
    MeshRenderer *mr = GetComponent<MeshRenderer>();
    if(CAN_USE_COMPONENT(mr))
    {
        const Mesh *m = mr->GetMesh();
        if(m != nullptr)
        {
            b = m->GetBoundingBox();
        }
    }
    return b;
}

Box GameObject::GetLocalBoundingBox() const
{
    Box b = GetObjectBoundingBox();
    Transform *t = GetComponent<Transform>();
    if(CAN_USE_COMPONENT(t))
    {
        Matrix4 mat;
        t->GetLocalMatrix(mat);
        b = mat * b; //Apply transform to Box
    }
    return b;
}

Box GameObject::GetBoundingBox() const
{
    Box b = GetObjectBoundingBox();
    Transform *t = GetComponent<Transform>();
    if(CAN_USE_COMPONENT(t))
    {
        Matrix4 mat;
        t->GetMatrix(mat);
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
    c->owner = this;
    comps.push_back(c);
}

void GameObject::MoveComponent(Component *c, int distance)
{
    for(auto comp = comps.begin(); comp != comps.end(); ++comp)
    {
        if(c == *comp)
        {
            auto comp1 = comp;
            std::advance(comp1, 1);
            comps.erase(comp, comp1);
            std::advance(comp1, distance);
            comps.insert(comp1, 1, c);
            break;
        }
    }
}

void GameObject::RemoveComponent(Component *c)
{
    for(auto comp = comps.begin(); comp != comps.end(); ++comp)
    {
        if(c == *comp)
        {
            comps.erase(comp);
            break;
        }
    }
}


void GameObject::AddChildWithoutNotifyingHierarchy(GameObject *child)
{
    child->parent = this;
    children.push_back(child);
}

void GameObject::AddChild(GameObject *child)
{
    AddChildWithoutNotifyingHierarchy(child);

    #ifdef BANG_EDITOR
    WindowEventManager::NotifyChildAdded(child);
    #endif
}

GameObject *GameObject::GetChild(const std::string &name) const
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        GameObject *child = (*it);
        if(child->name == name)
        {
            return child;
        }
    }
    return nullptr;
}

void GameObject::MoveChild(GameObject *child, GameObject *newParent)
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        if((*it) == child)
        {
            RemoveChildWithoutNotifyingHierarchy(it);
            newParent->AddChildWithoutNotifyingHierarchy(child);

            #ifdef BANG_EDITOR
            WindowEventManager::NotifyChildChangedParent(child, this);
            #endif

            break;
        }
    }
}

std::list<GameObject*>::iterator GameObject::RemoveChildWithoutNotifyingHierarchy(
        std::list<GameObject*>::iterator &it)
{
    GameObject *child = (*it);
    child->parent = nullptr;
    return children.erase(it);
}


std::list<GameObject*>::iterator GameObject::RemoveChild(
        std::list<GameObject*>::iterator &it)
{
    auto itret = RemoveChildWithoutNotifyingHierarchy(it);
    #ifdef BANG_EDITOR
    WindowEventManager::NotifyChildRemoved((*it));
    #endif
    return itret;
}

void GameObject::RemoveChild(const std::string &name)
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        GameObject *child = (*it);
        if(child->name == name)
        {
            RemoveChild(it);
            break;
        }
    }
}

void GameObject::RemoveChild(GameObject *child)
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        if((*it) == child)
        {
            RemoveChild(it);
            break;
        }
    }
}

void GameObject::SetParent(GameObject *newParent)
{
    GameObject *previousParent = parent;
    if(parent != nullptr)
    {
        if(newParent != nullptr)
        {
            parent->MoveChild(this, newParent);
        }
        else
        {
            Scene *st = GetScene();
            if(st != nullptr)
            {
                parent->MoveChild(this, newParent);
            }
        }
    }

    #ifdef BANG_EDITOR
    WindowEventManager::NotifyChildChangedParent(this, previousParent);
#endif
}

void GameObject::SetRenderLayer(unsigned char layer)
{
    this->renderLayer = layer;
}

void GameObject::SetName(const std::string &name)
{
    this->name = name;
}

bool GameObject::IsEditorGameObject() const
{
    return false;
}

bool GameObject::IsScene() const
{
    return isScene;
}

#ifdef BANG_EDITOR
void GameObject::OnTreeHierarchyGameObjectsSelected(
        std::list<GameObject*> &selectedEntities )
{
    if(IsEditorGameObject() || IsScene()) return;

    bool selected = false;
    for(auto it : selectedEntities)
    {
        if( it == this )
        {
            selected = true;
            break;
        }
    }

    if(selected)
    {
        if(!ed_wasSelectedInHierarchy)
        {
            ed_selectionGameObject = new EditorSelectionGameObject();
            AddChildWithoutNotifyingHierarchy(ed_selectionGameObject);
        }
    }
    else
    {
        if(ed_wasSelectedInHierarchy)
        {
            RemoveChild(ed_selectionGameObject);
        }
    }

    ed_wasSelectedInHierarchy = selected;
}
#endif

void GameObject::Write(std::ostream &f) const
{
    f << "<GameObject>" << std::endl;
    f << ((void*)this) << std::endl;   //internal file id
    f << this->GetName() << std::endl; //Print name

    f << "<children>" << std::endl;
    for(GameObject *e : children)
    {
        e->Write(f);
    }
    f << "</children>" << std::endl;

    f << "<components>" << std::endl;
    for(Component *p : comps)
    {
        p->Write(f);
    }
    f << "</components>" << std::endl;

    f << "</GameObject>" << std::endl;
}

void GameObject::Read(std::istream &f)
{
    FileReader::RegisterNextPointerId(f, this); //Read GameObject id
    SetName( FileReader::ReadString(f) );  //Read GameObject name
    std::string line;
    while( (line = FileReader::ReadNextLine(f)) != "</GameObject>")
    {
        if(line == "<children>")
        {
            FileReader::ReadChildren(f, this);
        }
        else if(line == "<components>")
        {
            FileReader::ReadComponents(f, this);
        }
        else
        {

        }
    }
}

void GameObject::SetEnabled(bool enabled) { this->enabled = enabled; }

bool GameObject::IsEnabled() { return enabled; }


const std::string GameObject::ToString() const
{
    return " [GameObject:'" + name + "']";
}

void GameObject::_OnStart()
{
    PROPAGATE_EVENT(_OnStart, comps);
    PROPAGATE_EVENT(_OnStart, children);
    OnStart();
}


void GameObject::_OnUpdate()
{
    PROPAGATE_EVENT(_OnUpdate, comps);
    PROPAGATE_EVENT(_OnUpdate, children);
    OnUpdate();
}

void GameObject::_OnPreRender ()
{
    PROPAGATE_EVENT(_OnPreRender, children);

    if(this->renderLayer == GetScene()->currentRenderLayer)
    {
        PROPAGATE_EVENT(_OnPreRender, comps);
        OnPreRender();
    }
}

void GameObject::_OnRender ()
{
    PROPAGATE_EVENT(_OnRender, children);

    if(this->renderLayer == GetScene()->currentRenderLayer)
    {
        PROPAGATE_EVENT(_OnRender, comps);
        OnRender();
    }
}

void GameObject::_OnDestroy()
{
    PROPAGATE_EVENT(_OnDestroy, comps);
    //No need to propagate _OnDestroy to children,
    //since the "delete child" itself propagates it (look at the destructor)
    OnDestroy();
}
