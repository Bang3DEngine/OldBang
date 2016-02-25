#include "Entity.h"
#include "Stage.h"
#include "Part.h"
#include "StageReader.h"
#include "WindowEventManager.h"

Entity::Entity() : Entity("")
{
}

Entity::Entity(const std::string &name) : name(name), parent(nullptr), isStage(false)
{
}

Entity::~Entity()
{
    this->_OnDestroy();
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        Entity *child = *it;
        this->RemoveChild(child->GetName());
        delete child;
    }
}


Stage *Entity::GetStage()
{
    if(isStage) { return (Stage*) this; }
    if(parent != nullptr) return parent->GetStage();
    return nullptr;
}

Entity *Entity::GetParent() const
{
    return parent;
}

void Entity::AddPart(Part *p)
{
    p->owner = this;
    parts.push_back(p);
}

void Entity::AddChildWithoutNoifyingHierarchy(Entity *child)
{
    child->parent = this;
    children.push_back(child);
}

void Entity::AddChild(Entity *child)
{
    AddChildWithoutNoifyingHierarchy(child);

    #ifdef BANG_EDITOR
    WindowEventManager::NotifyChildAdded(child);
    #endif
}

Entity *Entity::GetChild(const std::string &name) const
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        Entity *child = (*it);
        if(child->name == name)
        {
            return child;
        }
    }
    return nullptr;
}

void Entity::MoveChild(Entity *child, Entity *newParent)
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        if((*it) == child)
        {
            RemoveChildWithoutNoifyingHierarchy(it);
            newParent->AddChildWithoutNoifyingHierarchy(child);

            #ifdef BANG_EDITOR
            WindowEventManager::NotifyChildChangedParent(child, this);
            #endif

            break;
        }
    }
}

void Entity::RemoveChildWithoutNoifyingHierarchy(std::list<Entity*>::iterator &it)
{
    Entity *child = (*it);
    child->parent = nullptr;
    children.erase(it);
}


void Entity::RemoveChild(std::list<Entity*>::iterator &it)
{
    RemoveChildWithoutNoifyingHierarchy(it);
    #ifdef BANG_EDITOR
    WindowEventManager::NotifyChildRemoved((*it));
    #endif
}

void Entity::RemoveChild(const std::string &name)
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        Entity *child = (*it);
        if(child->name == name)
        {
            RemoveChild(it);
            break;
        }
    }
}

void Entity::RemoveChild(Entity *child)
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

void Entity::SetParent(Entity *newParent)
{
    Entity *previousParent = parent;
    if(parent != nullptr)
    {
        if(newParent != nullptr)
        {
            parent->MoveChild(this, newParent);
        }
        else
        {
            Stage *st = GetStage();
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

void Entity::SetName(const std::string &name)
{
    this->name = name;
}

bool Entity::IsStage() const
{
    return isStage;
}

void Entity::Write(std::ostream &f) const
{

}

void Entity::Read(std::istream &f)
{
    StageReader::RegisterNextPointerId(f, this); //Read Entity id
    SetName( FileReader::ReadString(f) );  //Read Entity name
    std::string line;
    while( (line = FileReader::ReadNextLine(f)) != "</Entity>")
    {
        if(line == "<children>")
        {
            StageReader::ReadChildren(f, this);
        }
        else if(line == "<parts>")
        {
            StageReader::ReadParts(f, this);
        }
        else
        {

        }
    }
}

#ifdef BANG_EDITOR
void Entity::OnTreeHierarchyEntitiesSelected(const std::list<Entity*> &selectedEntities)
{
    bool isSelected = false;
    for(auto it = selectedEntities.begin(); it != selectedEntities.end(); ++it)
    {
        if((*it) == this)
        {
            isSelected = true;
            break;
        }
    }

    Material *mat = nullptr;
    if(this->HasPart<MeshRenderer>())
    {
        mat = this->GetPart<MeshRenderer>()->GetMaterial();
    }

    if(isSelected)
    {
        if(mat != nullptr)
        {
            mat->SetDiffuseColor(glm::vec4(0.0f, 1.0f, 0.0f, 0.7f));
        }
    }
    else
    {
        if(mat != nullptr)
        {
            mat->SetDiffuseColor(glm::vec4(0.0f));
        }
    }
}
#endif


const std::string Entity::ToString() const
{
    return " [Entity:'" + name + "']";
}

void Entity::_OnStart()
{
    PROPAGATE_EVENT(_OnStart, parts);
    PROPAGATE_EVENT(_OnStart, children);
    OnStart();
}


void Entity::_OnUpdate()
{
    PROPAGATE_EVENT(_OnUpdate, parts);
    PROPAGATE_EVENT(_OnUpdate, children);
    OnUpdate();
}

void Entity::_OnRender()
{
    PROPAGATE_EVENT(_OnRender, parts);
    PROPAGATE_EVENT(_OnRender, children);
    OnRender();
}

void Entity::_OnDestroy()
{
    PROPAGATE_EVENT(_OnDestroy, parts);
    PROPAGATE_EVENT(_OnDestroy, children);
    OnDestroy();
}
