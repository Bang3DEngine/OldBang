#ifndef COMPONENTWIDGET_H
#define COMPONENTWIDGET_H

#include "InspectorWidget.h"
#include "ComponentWidgetContextMenu.h"

class Component;
class ComponentWidget : public InspectorWidget
{
    Q_OBJECT

public:
    explicit ComponentWidget(Component *relatedComponent);
    virtual ~ComponentWidget();

    int GetHeightSizeHint() override;

public slots:
    void Start();
    void OnEnabledCheckboxPressed(bool checked);

protected:
    void SetClosed(bool closed) override;

private:

    ComponentWidgetContextMenu m_cwContextMenu;

    Component *m_relatedComponent = nullptr;
    QCheckBox *m_enabledCheckbox = nullptr;

    friend class InspectorContextMenu;
    friend class ComponentWidgetContextMenu;
};

#endif // COMPONENTWIDGET_H
