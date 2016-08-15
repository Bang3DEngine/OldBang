#ifndef LISTINSPECTORITEMWIDGET_H
#define LISTINSPECTORITEMWIDGET_H

#include "Bang.h"

#include "GameObject.h"
#include "InspectorWidget.h"
#include "ComponentClipboard.h"

class Component;
class ComponentWidget : public InspectorWidget
{
    Q_OBJECT

private:
    Component *m_relatedComponent = nullptr;
    QCheckBox *m_enabledCheckbox = nullptr;

public:
    explicit ComponentWidget(Component *relatedComponent);
    virtual ~ComponentWidget();

    virtual void OnCustomContextMenuRequested(QPoint point) override;

public slots:
    void OnEnabledCheckboxPressed(bool checked);

    void OnContextMenuMoveUpSelected();
    void MenuMoveUpSelected();
    void OnContextMenuMoveDownSelected();
    void MenuMoveDownSelected();

    void OnContextMenuCopyComponentSelected();
    void CopyComponentSelected();
    void OnContextMenuPasteComponentSelected();
    void PasteComponentSelected();
    void OnContextMenuPasteComponentValuesSelected();
    void PasteComponentValuesSelected();

    void OnContextMenuRemoveComponentSelected();
    void RemoveComponentSelected();
};

#endif // LISTINSPECTORITEM_H
