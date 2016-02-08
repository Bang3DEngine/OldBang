#ifndef PART_H
#define PART_H

#include "StageEventListener.h"

class Entity;
class Part : public StageEventListener
{
friend class Entity;
private:
    Entity *owner;

protected:
    Part();
    virtual ~Part();

public:
    Entity* GetOwner() const { return owner; }
};

#endif // PART_H
