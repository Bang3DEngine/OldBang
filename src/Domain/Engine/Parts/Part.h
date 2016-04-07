#ifndef PART_H
#define PART_H

#include "Bang.h"

#include "Logger.h"
#include "IFileable.h"
#include "IToString.h"
#include "IStageEventListener.h"

#ifdef BANG_EDITOR
#include "IInspectable.h"
#include "InspectorWidget.h"
#include "IWindowEventManagerListener.h"
#endif

#define CAN_USE_PART(part)  ( part != nullptr && part->IsEnabled() )

class Entity;

class Part : public IStageEventListener, public IToString, public IFileable, public IWindowEventManagerListener
             #ifdef BANG_EDITOR
             , public IInspectable
             #endif
{
friend class Entity;
protected:
    Entity *owner;
    bool enabled = true;

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

    virtual std::string GetName() const { return "Part"; }

    void SetEnabled(bool enabled) { this->enabled = enabled; }
    bool IsEnabled() { return enabled; }
};

#endif // PART_H
