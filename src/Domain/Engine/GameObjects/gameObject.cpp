#include "gameObject.h"
#include "Scene.h"
#include "Component.h"
#include "SceneReader.h"
#include "WindowEventManager.h"

#include "EditorAxis.h"

gameObject::gameObject() : gameObject("")
{
}

gameObject::gameObject(const std::string &name) : name(name), parent(nullptr), isScene(false)
{
}

gameObject::~gameObject()
{
    this->_OnDestroy();
    for(auto it = children.begin(); it != children.end();)
    {
        gameObject *child = *it;
        it = this->RemoveChildWithoutNotifyingHierarchy(it);
        delete child;
    }
}


Scene *gameObject::GetScene()
{
    if(isScene) { return (Scene*) this; }
    if(parent != nullptr) return parent->GetScene();
    return nullptr;
}

gameObject *gameObject::GetParent() const
{
    return parent;
}

void gameObject::AddComponent(Component *p)
{
    p->owner = this;
    comps.push_back(p);
}

void gameObject::MoveComponent(Component *p, int distance)
{
    for(auto comp = comps.begin(); comp != comps.end(); ++comp)
    {
        if(p == *comp)
        {
            auto comp1 = comp;
            std::advance(comp1, 1);
            comps.erase(comp, comp1);
            std::advance(comp1, distance);
            comps.insert(comp1, 1, p);
            break;
        }
    }
}

void gameObject::RemoveComponent(Component *p)
{
    for(auto comp = comps.begin(); comp != comps.end(); ++comp)
    {
        if(p == *comp)
        {
            comps.erase(comp);
            break;
        }
    }
}


void gameObject::AddChildWithoutNoifyingHierarchy(gameObject *child)
{
    child->parent = this;
    children.push_back(child);
}

void gameObject::AddChild(gameObject *child)
{
    AddChildWithoutNoifyingHierarchy(child);

    #ifdef BANG_EDITOR
    WindowEventManager::NotifyChildAdded(child);
    #endif
}

gameObject *gameObject::GetChild(const std::string &name) const
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        gameObject *child = (*it);
        if(child->name == name)
        {
            return child;
        }
    }
    return nullptr;
}

void gameObject::MoveChild(gameObject *child, gameObject *newParent)
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

std::list<gameObject*>::iterator gameObject::RemoveChildWithoutNotifyingHierarchy(
        std::list<gameObject*>::iterator &it)
{
    gameObject *child = (*it);
    child->parent = nullptr;
    return children.erase(it);
}


std::list<gameObject*>::iterator gameObject::RemoveChild(
        std::list<gameObject*>::iterator &it)
{
    auto itret = RemoveChildWithoutNotifyingHierarchy(it);
    #ifdef BANG_EDITOR
    WindowEventManager::NotifyChildRemoved((*it));
    #endif
    return itret;
}

void gameObject::RemoveChild(const std::string &name)
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        gameObject *child = (*it);
        if(child->name == name)
        {
            RemoveChild(it);
            break;
        }
    }
}

void gameObject::RemoveChild(gameObject *child)
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

void gameObject::SetParent(gameObject *newParent)
{
    gameObject *previousParent = parent;
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

void gameObject::SetName(const std::string &name)
{
    this->name = name;
}

bool gameObject::IsScene() const
{
    return isScene;
}

#ifdef BANG_EDITOR
void gameObject::OnTreeHierarchyEntitiesSelected(const std::list<gameObject*> &selectedEntities)
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

void gameObject::Write(std::ostream &f) const
{
    f << "<gameObject>" << std::endl;
    f << ((void*)this) << std::endl;   //internal file id
    f << this->GetName() << std::endl; //scene name

    //Children
    f << "<children>" << std::endl;
    for(gameObject *e : children)
    {
        e->Write(f);
    }
    f << "</children>" << std::endl;

    //Components
    f << "<comps>" << std::endl;
    for(Component *p : comps)
    {
        p->Write(f);
    }
    f << "</comps>" << std::endl;

    f << "</gameObject>" << std::endl;
}

void gameObject::Read(std::istream &f)
{
    SceneReader::RegisterNextPointerId(f, this); //Read gameObject id
    SetName( FileReader::ReadString(f) );  //Read gameObject name
    std::string line;
    while( (line = FileReader::ReadNextLine(f)) != "</gameObject>")
    {
        if(line == "<children>")
        {
            SceneReader::ReadChildren(f, this);
        }
        else if(line == "<comps>")
        {
            SceneReader::ReadComponents(f, this);
        }
        else
        {

        }
    }
}

#endif


const std::string gameObject::ToString() const
{
    return " [gameObject:'" + name + "']";
}

void gameObject::_OnStart()
{
    PROPAGATE_EVENT(_OnStart, comps);
    PROPAGATE_EVENT(_OnStart, children);
    OnStart();
}


void gameObject::_OnUpdate()
{
    PROPAGATE_EVENT(_OnUpdate, comps);
    PROPAGATE_EVENT(_OnUpdate, children);
    OnUpdate();
}

void gameObject::_OnRender()
{
    PROPAGATE_EVENT(_OnRender, comps);
    PROPAGATE_EVENT(_OnRender, children);
    OnRender();
}

void gameObject::_OnDestroy()
{
    PROPAGATE_EVENT(_OnDestroy, comps);
    //No need to propagate _OnDestroy to children,
    //since the "delete child" itself propagates it (look at the destructor)
    OnDestroy();
}
