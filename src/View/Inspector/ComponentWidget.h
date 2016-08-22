#ifndef LISTINSPECTORITEMWIDGET_H
#define LISTINSPECTORITEMWIDGET_H

#include "Bang.h"

#include "GameObject.h"
#include "InspectorWidget.h"
#include "ComponentClipboard.h"
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
};

#endif // LISTINSPECTORITEM_H
