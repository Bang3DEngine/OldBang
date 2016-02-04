#ifndef STAGE_H
#define STAGE_H

#include <list>
#include <string>

#include "Canvas.h"
#include "Entity.h"

class Stage
{
friend class Canvas;

private:
    std::list<Entity> entities;

    void OnStart();
    void OnUpdate();
    void OnDestroy();

public:
    Stage();

    Entity* AddEntity();
    Entity* GetEntity(const std::string &name) const;

    const std::list<Entity>* GetEntities() const { return &entities; }
};

#endif // STAGE_H
