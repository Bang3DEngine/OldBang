#ifndef BEHAVIOURCONTAINER_H
#define BEHAVIOURCONTAINER_H

#include "Bang/Path.h"
#include "Bang/Component.h"

FORWARD NAMESPACE_BANG_BEGIN
FORWARD NAMESPACE_BANG_END

NAMESPACE_BANG_BEGIN

FORWARD class Library;
FORWARD class XMLNode;
FORWARD class Behaviour;

class BehaviourContainer : public Component
{
    COMPONENT(BehaviourContainer)

public:
    void SetSourceFilepath(const Path &sourceFilepath);
    Behaviour* CreateBehaviourInstance(Library *behavioursLibrary) const;

    String GetBehaviourName() const;
    const Path& GetSourceFilepath() const;

    void SubstituteByBehaviourInstance(Library *behavioursLibrary);

private:
    Path m_sourceFilepath;
    Behaviour *p_behaviour = nullptr;

    BehaviourContainer();
    virtual ~BehaviourContainer();

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

    friend class ComponentFactory;
};

NAMESPACE_BANG_END

#endif // BEHAVIOURCONTAINER_H

