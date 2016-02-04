#ifndef PART_H
#define PART_H

#include "IStageEventListener.h"

class Part : public IStageEventListener
{
friend class Entity;

private:

    virtual void _OnStart() override;
    virtual void _OnUpdate() override;
    virtual void _OnDestroy() override;

protected:
    Part();
    virtual ~Part();
};

#endif // PART_H
