#ifndef COMPONENTWIDGET_H
#define COMPONENTWIDGET_H

#include <QCheckBox>
#include "Bang/WinUndef.h"

#include "Bang/InspectorWidget.h"
#include "Bang/ComponentWidgetContextMenu.h"

class Component;
template <class Component> class Inspectable;
class ComponentWidget : public InspectorWidget
{
    Q_OBJECT

public:
    explicit ComponentWidget(Component *relatedComponent);
    virtual ~ComponentWidget();

    int GetHeightSizeHint() override;

public slots:
    void OnEnabledCheckboxPressed(bool checked);

protected:
    void SetClosed(bool closed) override;
    virtual void CreateWidgetSlots(XMLNode &xmlInfo) override;

private:

    QLabel m_iconLabel;
    ComponentWidgetContextMenu m_cwContextMenu;

    Component *p_component = nullptr;
    Inspectable<Component> *m_componentInspectable = nullptr;
    QCheckBox m_enabledCheckbox;

    friend class InspectorContextMenu;
    friend class ComponentWidgetContextMenu;
};

#endif // COMPONENTWIDGET_H
