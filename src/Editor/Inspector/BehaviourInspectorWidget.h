#ifndef BEHAVIOURINSPECTORWIDGET_H
#define BEHAVIOURINSPECTORWIDGET_H

#include "Bang/ComponentWidget.h"
#include "Bang/IAttrWidgetButtonListener.h"

class Behaviour;
class AttrWidgetButton;
class BehaviourInspectorWidget : public ComponentWidget,
                                 public IAttrWidgetButtonListener
{
public:
    BehaviourInspectorWidget(Behaviour *behaviour);
    virtual ~BehaviourInspectorWidget();

protected:
    void InitExtra() override;
    void Refresh() override;
    void OnDestroy() override;
};

#endif // BEHAVIOURINSPECTORWIDGET_H
