#ifndef INSPECTORWIDGETGROUP_H
#define INSPECTORWIDGETGROUP_H

#include <QObject>
#include "Bang/WinUndef.h"

#include "Bang/List.h"

class InspectorWidget;
class IInspectorWidget;
class InspectorWidgetGroup : public QObject
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

public:
    void InsertInspectorWidget(InspectorWidget *inspectorWidget);
    void InsertInspectorWidgets(const List<InspectorWidget*>& inspectorWidgets);
    const List<InspectorWidget*>& GetInspectorWidgets() const;

protected slots:
    virtual void OnInspectorWidgetChanged(IInspectorWidget *changedInspWidget);

private:
    List<InspectorWidget*> m_inspectorWidgets;
};

#endif // INSPECTORWIDGETGROUP_H
