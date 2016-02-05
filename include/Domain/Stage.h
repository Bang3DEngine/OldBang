#ifndef STAGE_H
#define STAGE_H

#include <list>
#include <string>

#include "Canvas.h"
#include "Entity.h"
#include "StageEventListener.h"

class Stage : public StageEventListener
{
friend class Canvas;

private:
    std::string name;

protected:
    Stage();
    std::list<Entity*> entities;

    void _OnStart() override;
    void _OnUpdate() override;
    void _OnDrawing() override;
    void _OnDestroy() override;

public:
    virtual ~Stage();

    Entity* AddEntity();
    Entity* GetEntity(const std::string &name) const;

    const std::list<Entity*>* GetEntities() const { return &entities; }
};

#endif // STAGE_H
