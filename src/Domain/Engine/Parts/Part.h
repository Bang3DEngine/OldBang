#ifndef PART_H
#define PART_H

#include "Entity.h"
#include "Logger.h"
#include "IStageEventListener.h"

class Part : public IStageEventListener
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
