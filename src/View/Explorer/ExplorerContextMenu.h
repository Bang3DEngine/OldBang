#ifndef EXPLORERCONTEXTMENU_H
#define EXPLORERCONTEXTMENU_H

#include "ContextMenu.h"

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
    void OnCreateFolderClicked();
};

#endif // EXPLORERCONTEXTMENU_H
