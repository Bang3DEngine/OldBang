#ifndef LISTINSPECTORITEMWIDGET_H
#define LISTINSPECTORITEMWIDGET_H

#include "Bang.h"

#include "InspectorWidget.h"

class Component;
class ComponentWidget : public InspectorWidget
{
    Q_OBJECT

private:
    Component *m_relatedComponent;
    QCheckBox *m_IsEnabledCheckbox;

public:
    explicit ComponentWidget(Component *relatedComponent);
    virtual ~ComponentWidget();

    virtual void OnCustomContextMenuRequested(QPoint point) override;

public slots:
    void OnEnabledCheckboxPressed(bool checked);

    void OnContextMenuRemoveComponentSelected();
    void OnContextMenuMoveUpSelected();
    void OnContextMenuMoveDownSelected();

};

#endif // LISTINSPECTORITEM_H
