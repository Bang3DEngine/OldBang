#ifndef COMPONENT_H
#define COMPONENT_H

#include "Bang.h"

#include "Logger.h"
#include "IFileable.h"
#include "IToString.h"
#include "ISceneEventListener.h"

#ifdef BANG_EDITOR
#include "FileWriter.h"
#include "IInspectable.h"
#include "InspectorWidget.h"
#include "IWindowEventManagerListener.h"
#endif

#define CAN_USE_COMPONENT(comp)  ( comp != nullptr && comp->IsEnabled() )

class GameObject;

class Component : public ISceneEventListener, public IToString, public IFileable, public IWindowEventManagerListener
             #ifdef BANG_EDITOR
             , public IInspectable
             #endif
{
friend class GameObject;
protected:
    GameObject *owner;
    bool enabled = true;

    Component();
    virtual ~Component();

public:
    GameObject* GetOwner() const;

    virtual const std::string ToString() const override;

    virtual void Write(std::ostream &f) const override {}
    virtual void Read(std::istream &f) override {}

    virtual void _OnStart() override { OnStart(); }
    virtual void _OnRender() override { OnRender(); }
    virtual void _OnUpdate() override { OnUpdate(); }
    virtual void _OnDestroy() override { OnDestroy(); }

    virtual std::string GetName() const { return "Component"; }

    void SetEnabled(bool enabled) { this->enabled = enabled; }
    bool IsEnabled() { return enabled; }
};

#endif // COMPONENT_H
