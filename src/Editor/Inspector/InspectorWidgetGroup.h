#ifndef INSPECTORWIDGETGROUP_H
#define INSPECTORWIDGETGROUP_H

#include <QObject>
#include "Bang/WinUndef.h"

#include "Bang/List.h"
#include "Bang/Destroyable.h"

class InspectorWidget;
class IInspectorWidget;
class InspectorWidgetGroup : public QObject,
                             public IDestroyListener
{
    Q_OBJECT

public:
    InspectorWidgetGroup();
    virtual ~InspectorWidgetGroup();

    virtual void Init();
    virtual void OnUpdate();
    virtual String GetTitle() const;
    virtual bool NeedsEnableCheckBox() const;
    virtual void OnEnableCheckBoxChanged(bool enabled);
    virtual bool IsEnabled() const;

    void InsertInspectorWidget(InspectorWidget *inspectorWidget);
    void InsertInspectorWidgets(const List<InspectorWidget*>& inspectorWidgets);
    void MoveInspectorWidget(InspectorWidget *inspWidget, int newIndex);
    virtual List<InspectorWidget*>::Iterator RemoveInspectorWidget(
                                                InspectorWidget *inspWidget);
    const List<InspectorWidget*>& GetInspectorWidgets() const;

protected slots:
    virtual void OnInspectorWidgetChanged(IInspectorWidget *changedInspWidget);

private:
    List<InspectorWidget*> m_inspectorWidgets;

protected: // IDestroyListener
    void OnDestroyDemanded(Destroyable *objectDemandingDestroy) override;

signals:
    void RequestInsert(InspectorWidget *InspectorWidget);
    void RequestMove(InspectorWidget *InspectorWidget, int row);
    void RequestRemove(InspectorWidget *InspectorWidget);
};

#endif // INSPECTORWIDGETGROUP_H
