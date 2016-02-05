#ifndef PART_H
#define PART_H

#include "StageEventListener.h"

class Part : public StageEventListener
{
friend class Entity;
protected:
    Part();
    virtual ~Part();
};

#endif // PART_H
