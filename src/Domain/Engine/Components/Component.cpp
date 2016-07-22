#include "Component.h"
#include "GameObject.h"

Component::Component()
{
}

Component::~Component()
{
#ifdef BANG_EDITOR
    for (int i = 0; i < m_inspectorComponentInfo.GetSlotInfos().size(); ++i)
        delete m_inspectorComponentInfo.GetSlotInfo(i);
#endif
}

void Component::CloneInto(ICloneable *clone) const
{
    Component *c = static_cast<Component*>(clone);
    c->m_enabled = m_enabled;
    c->p_gameObject = p_gameObject;
}

const std::string Component::ToString() const
{
    std::ostringstream msg;
    msg << "Component";
    return msg.str();
}
