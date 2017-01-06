#ifndef LISTINSPECTORITEMWIDGET_H
#define LISTINSPECTORITEMWIDGET_H

#include "InspectorWidget.h"
#include "ComponentWidgetContextMenu.h"

class Component;
class ComponentWidget : public InspectorWidget
{
friend class InspectorContextMenu;
friend class ComponentWidgetContextMenu;

    Q_OBJECT

private:

    ComponentWidgetContextMenu m_cwContextMenu;

    Component *m_relatedComponent = nullptr;
    QCheckBox *m_enabledCheckbox = nullptr;

public:
    explicit ComponentWidget(Component *relatedComponent);
    virtual ~ComponentWidget();

public slots:
    void OnEnabledCheckboxPressed(bool checked);

protected:
    void SetClosed(bool closed) override;
};

#endif // LISTINSPECTORITEM_H
