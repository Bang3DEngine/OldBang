#ifndef PART_H
#define PART_H

#include "Entity.h"
#include "Logger.h"
#include "IFileable.h"
#include "IToString.h"
#include "IStageEventListener.h"

class Part : public IStageEventListener, public IToString, public IFileable
{
friend class Entity;
protected:
    Entity *parent;

    Part();
    virtual ~Part();

public:
    Entity* GetParent() const;

    virtual const std::string ToString() const override;

    virtual void Write(std::ostream &f) const override {}
    virtual void Read(std::istream &f) override {}
};

#endif // PART_H
