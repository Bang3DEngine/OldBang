#ifndef BEHAVIOURHOLDER_H
#define BEHAVIOURHOLDER_H

#include <functional>

#include <QThread>
#include <QInputDialog>

#include "Bang.h"
#include "Component.h"
#include "IAttrWidgetButtonListener.h"

#include "Behaviour.h"
#include "SystemUtils.h"

class CompileBehaviourThread : public QThread
{
protected:
    BehaviourHolder *m_behaviourHolder = nullptr;
    void run() override;

public:
    void SetBehaviourHolder(BehaviourHolder *bh);
};

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
                        ,public IAttrWidgetButtonListener
{
friend class GameObject;

private:
    CompileBehaviourThread m_compileThread;

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
    String m_sourceFilepath = "";

    void ChangeBehaviour(Behaviour *newBehaviour);

protected:
    BehaviourHolder();

public:
    virtual ~BehaviourHolder();

    virtual const String ToString() const override;
    virtual String GetName() const override;

    virtual void CloneInto(ICloneable *clone) const override;
    virtual ICloneable *Clone() const override;

    const String &GetSourceFilepath() const;

    #ifdef BANG_EDITOR
    virtual void OnInspectorXMLNeeded(XMLNode *xmlInfo) const override;
    virtual void OnInspectorXMLChanged(const XMLNode *xmlInfo) override;
    #endif

    void OnButtonClicked(const String &attrName) override;

    void Refresh();

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    virtual void _OnStart () override;
    virtual void _OnUpdate () override;
    virtual void _OnPreRender () override;
    virtual void _OnRender () override;
    virtual void _OnDestroy () override;

    void OnBehaviourFinishedCompiling(String soFilepath);

    // TODO: Add windowEventListener events pass to Behaviour too
};

#endif
