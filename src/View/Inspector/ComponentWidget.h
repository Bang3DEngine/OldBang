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

public:
    explicit ComponentWidget(Component *relatedComponent);
    virtual ~ComponentWidget();

public slots:
    void Start();
    void OnEnabledCheckboxPressed(bool checked);

protected:
    void SetClosed(bool closed) override;

private:

    ComponentWidgetContextMenu m_cwContextMenu;

    Component *m_relatedComponent = nullptr;
    QCheckBox *m_enabledCheckbox = nullptr;

};

#endif // LISTINSPECTORITEM_H
