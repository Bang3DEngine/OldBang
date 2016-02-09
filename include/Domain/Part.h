#ifndef PART_H
#define PART_H

#include "Entity.h"
#include "StageEventListener.h"

class Part : public StageEventListener
{
friend class Entity;
private:
    Entity *parent;

protected:
    Part();
    virtual ~Part();

public:
    Entity* GetParent() const;
};

#endif // PART_H
