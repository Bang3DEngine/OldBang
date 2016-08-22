#ifndef EXPLORERCONTEXTMENU_H
#define EXPLORERCONTEXTMENU_H

#include <QTreeWidgetItem>

#include "Prefab.h"
#include "Inspector.h"
#include "FileDialog.h"
#include "ContextMenu.h"
#include "GameObjectClipboard.h"

class Explorer;
class ExplorerContextMenu : public ContextMenu
{
    Q_OBJECT

private:
    Explorer *m_explorer;

public:
    ExplorerContextMenu(Explorer *explorer);

public slots:
    void OnCustomContextMenuRequested(QPoint point) override;

    void OnDuplicateClicked();
    void OnDeleteClicked();
};

#endif // EXPLORERCONTEXTMENU_H
