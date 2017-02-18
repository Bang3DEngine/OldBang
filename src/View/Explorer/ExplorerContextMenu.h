#ifndef EXPLORERCONTEXTMENU_H
#define EXPLORERCONTEXTMENU_H

#include "ContextMenu.h"

class Explorer;
class ExplorerContextMenu : public ContextMenu
{
    Q_OBJECT

public:
    ExplorerContextMenu(Explorer *explorer);

public slots:
    void OnCustomContextMenuRequested(QPoint point) override;

    void OnDuplicateClicked();
    void OnDeleteClicked();
    void OnCreateDirClicked();

private:
    Explorer *m_explorer = nullptr;
};

#endif // EXPLORERCONTEXTMENU_H
