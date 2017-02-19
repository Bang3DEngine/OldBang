#ifndef HIERARCHYCONTEXTMENU_H
#define HIERARCHYCONTEXTMENU_H

#include "ContextMenu.h"

class Hierarchy;
class HierarchyContextMenu : public ContextMenu
{
    Q_OBJECT

private:

    Hierarchy *m_hierarchy;

public:
    HierarchyContextMenu(Hierarchy *hierarchy);

public slots:

    void OnCustomContextMenuRequested(QPoint point) override;

    void OnCreateEmptyClicked();
    void OnCopyClicked();
    void OnPasteClicked();
    void OnDuplicateClicked();
    void OnCreatePrefab();
    void OnDeleteClicked();
};

#endif // HIERARCHYCONTEXTMENU_H
