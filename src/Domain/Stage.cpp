#include "Stage.h"
#include "Entity.h"

Stage::Stage() : cameraEntity(nullptr)
{
}

Stage::~Stage()
{
    for(auto it = children.begin(); it != children.end(); ++it)
    {
        Entity *child = *it;
        child->_OnDestroy();
        this->RemoveChild(child->GetName());
        delete child;
    }
}

void Stage::SetCameraEntity(const Entity *cameraEntity)
{
    if(cameraEntity->HasPart<Camera>())
    {
        this->cameraEntity = cameraEntity;
    }
    else
    {
        Logger_Error("Can't set " << cameraEntity <<
                     " as camera because it does not have a Camera part.");
    }
}

Camera *Stage::GetCamera() const
{
    if(cameraEntity == nullptr) return nullptr;
    return cameraEntity->GetPart<Camera>();
}

