#ifndef PART_H
#define PART_H

#include "Bang.h"

#include "Logger.h"
#include "IFileable.h"
#include "IToString.h"
#include "IStageEventListener.h"

class Entity;

class Part : public IStageEventListener, public IToString, public IFileable
{
friend class Entity;
protected:
    Entity *owner;

    Part();
    virtual ~Part();

public:
    Entity* GetOwner() const;

    virtual const std::string ToString() const override;

    virtual void Write(std::ostream &f) const override {}
    virtual void Read(std::istream &f) override {}

    virtual void _OnStart() override { OnStart(); }
    virtual void _OnRender() override { OnRender(); }
    virtual void _OnUpdate() override { OnUpdate(); }
    virtual void _OnDestroy() override { OnDestroy(); }
};

#endif // PART_H
