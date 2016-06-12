#ifndef BEHAVIOURHOLDER_H
#define BEHAVIOURHOLDER_H

#include "Bang.h"
#include "Component.h"

#include "Behaviour.h"
#include "SystemUtils.h"

/**
 * @brief The BehaviourHolder class is the responsible of holding
 * dynamically loaded Behaviours, loading them, and passing its events to them,
 * in case they do exist.
 */
class BehaviourHolder : public Component
{
private:
    /**
     * @brief The dynamically loaded Behaviour
     */
    Behaviour *behaviour = nullptr;
    void ChangeBehaviour(Behaviour *newBehaviour);

public:
    BehaviourHolder();
    virtual ~BehaviourHolder();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override { return "Behaviour"; }

    #ifdef BANG_EDITOR
        void OnCompileButtonClicked();
        virtual InspectorWidgetInfo* GetComponentInfo() override;
        virtual void OnSlotValueChanged(InspectorWidget *source) override;

        void Write(std::ostream &f) const override;
        void Read(std::istream &f) override;
    #endif

    virtual void _OnStart () override;
    virtual void _OnUpdate () override;
    virtual void _OnPreRender () override;
    virtual void _OnRender () override;
    virtual void _OnDestroy () override;

    // TODO: Add windowEventListener events pass to Behaviour too
};

#endif
