#ifndef COMPONENTWIDGET_H
#define COMPONENTWIDGET_H

#include <QCheckBox>
#include "Bang/WinUndef.h"

#include "Bang/InspectorWidget.h"
#include "Bang/ComponentWidgetContextMenu.h"

class Component;
class ComponentWidget : public InspectorWidget,
                        public IDestroyListener
{
    Q_OBJECT

public:
    explicit ComponentWidget(Component *relatedComponent);
    virtual ~ComponentWidget();

    virtual void InitExtra() override;
    virtual void OnDestroy() override;
    int GetHeightSizeHint() const override;

public slots:
    void SetEnabledCheckboxVisible(bool visible);
    void OnEnabledCheckboxPressed(bool checked);

protected:
    void SetClosed(bool closed) override;

private:
    QLabel m_iconLabel;
    ComponentWidgetContextMenu m_cwContextMenu;

    Component *p_component = nullptr;
    QCheckBox m_enabledCheckbox;

    friend class InspectorContextMenu;
    friend class ComponentWidgetContextMenu;

protected: // IDestroyListener
   void OnDestroyableDestroyed(Destroyable *destroyedObject) override;
};

#endif // COMPONENTWIDGET_H
