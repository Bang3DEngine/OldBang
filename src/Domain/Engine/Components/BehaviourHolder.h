#ifndef BEHAVIOURHOLDER_H
#define BEHAVIOURHOLDER_H

#include "Bang.h"
#include "Component.h"

#include "Behaviour.h"
#include "SystemUtils.h"

/**
 * @brief The BehaviourHolder class is the responsible of holding
 * a UNIQUE dynamically loaded Behaviour, loading them, and passing its events to them,
 * in case they do exist.
 *
 * A GameObject actually contains a Component that is a BehaviourHolder.
 * In case the BehaviourHolder has a Behaviour correctly loaded, it will
 * pass the events to the behaviour. Otherwise, it will just do nothing.
 */
class BehaviourHolder : public Component
{
private:
    /**
     * @brief The dynamically loaded Behaviour
     */
    Behaviour *m_behaviour = nullptr;

    /**
     * @brief The currently open library to load the Behaviour, if any.
     */
    void *m_currentOpenLibrary = nullptr;

    /**
     * @brief Path to the Behaviour source file.
     * It is indicated using the Inspector
     */
    std::string m_sourceFilepath = "";

    void ChangeBehaviour(Behaviour *newBehaviour);

public:
    BehaviourHolder();
    virtual ~BehaviourHolder();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override { return "Behaviour"; }

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    #ifdef BANG_EDITOR
    virtual InspectorWidgetInfo* GetComponentInfo() override;
    virtual void OnSlotValueChanged(InspectorWidget *source) override;
    #endif

    void Refresh();

    void Write(std::ostream &f) const override;
    void Read(std::istream &f) override;

    virtual void _OnStart () override;
    virtual void _OnUpdate () override;
    virtual void _OnPreRender () override;
    virtual void _OnRender () override;
    virtual void _OnDestroy () override;

    // TODO: Add windowEventListener events pass to Behaviour too
};

#endif
