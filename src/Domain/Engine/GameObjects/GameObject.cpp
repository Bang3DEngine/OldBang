#include "GameObject.h"
#include "Scene.h"
#include "Component.h"
#include "FileReader.h"
#include "WindowEventManager.h"

#include "EditorAxis.h"

GameObject::GameObject() : GameObject("")
{
}

GameObject::GameObject(const std::string &name) : name(name), parent(nullptr), isScene(false)
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


void GameObject::AddChildWithoutNoifyingHierarchy(GameObject *child)
{
    child->parent = this;
    children.push_back(child);
}

void GameObject::AddChild(GameObject *child)
{
    AddChildWithoutNoifyingHierarchy(child);

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
            newParent->AddChildWithoutNoifyingHierarchy(child);

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

void GameObject::SetName(const std::string &name)
{
    this->name = name;
}

bool GameObject::IsScene() const
{
    return isScene;
}

#ifdef BANG_EDITOR
void GameObject::OnTreeHierarchyEntitiesSelected(const std::list<GameObject*> &selectedEntities)
{
    bool wasSelected = (selectedMaterial != nullptr);
    bool isSelected = false;
    for(auto it = selectedEntities.begin(); it != selectedEntities.end(); ++it)
    {
        if((*it) == this)
        {
            isSelected = true;
            break;
        }
    }

    //TODO: change this, not really a good way of doing it....
    if(HasComponent<MeshRenderer>())
    {
        if(isSelected)
        {
            if(nonSelectedMaterial == nullptr)
            {
                nonSelectedMaterial = GetComponent<MeshRenderer>()->GetMaterial();
            }

            if(!wasSelected)
            {
                if(nonSelectedMaterial != nullptr)
                {
                    //Create a copy of its material, and modify its properties
                    selectedMaterial = new Material(*nonSelectedMaterial);
                    selectedMaterial->SetDiffuseColor(glm::vec4(0.0f, 0.0f, 1.0f, 0.5f));

                    nonSelectedMaterial = GetComponent<MeshRenderer>()->GetMaterial();
                    //GetComponent<MeshRenderer>()->SetMaterial(selectedMaterial);
                }
            }
        }
        else
        {
            if(wasSelected)
            {
                if(nonSelectedMaterial != nullptr)
                {
                    delete selectedMaterial;
                    selectedMaterial = nullptr;
                    //GetComponent<MeshRenderer>()->SetMaterial(nonSelectedMaterial);
                }
            }
        }
    }
    //
}

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

#endif


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

void GameObject::_OnRender()
{
    PROPAGATE_EVENT(_OnRender, comps);
    PROPAGATE_EVENT(_OnRender, children);
    OnRender();
}

void GameObject::_OnDestroy()
{
    PROPAGATE_EVENT(_OnDestroy, comps);
    //No need to propagate _OnDestroy to children,
    //since the "delete child" itself propagates it (look at the destructor)
    OnDestroy();
}
