#ifndef BEHAVIOURINSPECTORWIDGET_H
#define BEHAVIOURINSPECTORWIDGET_H

#include "Bang/ComponentWidget.h"

class Behaviour;
class BehaviourInspectorWidget : public ComponentWidget
{
public:
    BehaviourInspectorWidget(Behaviour *behaviour);
    virtual ~BehaviourInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnAttrWidgetValueChanged(IAttributeWidget *attrWidget) override;
    void OnDestroy() override;

private:
    Behaviour *p_behaviour;
    AttrWidgetFile *m_sourceFileAW = nullptr;
};

#endif // BEHAVIOURINSPECTORWIDGET_H
