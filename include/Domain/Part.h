#ifndef PART_H
#define PART_H

#include "Entity.h"
#include "Logger.h"
#include "StageEventListener.h"

class Part : public StageEventListener
{
friend class Entity;
protected:
    Entity *parent;

    Part();
    virtual ~Part();

public:
    Entity* GetParent() const;
};

#endif // PART_H
