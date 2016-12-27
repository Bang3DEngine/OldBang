#ifndef COMPONENTWIDGETCONTEXTMENU_H
#define COMPONENTWIDGETCONTEXTMENU_H

#include <QTimer>
#include <QTreeWidgetItem>

#include "ContextMenu.h"

class ComponentWidget;
class ComponentWidgetContextMenu : public ContextMenu
{
friend class InspectorContextMenu; // For the PasteComponent

    Q_OBJECT

private:
    ComponentWidget *m_cWidget;

public:
    ComponentWidgetContextMenu(ComponentWidget *componentWidget);

public slots:
    void OnCustomContextMenuRequested(QPoint point) override;

    void OnMoveUpSelected();
    void MenuMoveUpSelected();
    void OnMoveDownSelected();
    void MenuMoveDownSelected();

    void OnCopyComponentSelected();
    void CopyComponentSelected();
    void OnPasteComponentSelected();
    void PasteComponentSelected();
    void OnPasteComponentValuesSelected();
    void PasteComponentValuesSelected();

    void OnRemoveComponentSelected();
    void RemoveComponentSelected();
};

#endif // COMPONENTWIDGETCONTEXTMENU_H
