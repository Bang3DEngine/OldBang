#ifndef EXPLORERCONTEXTMENU_H
#define EXPLORERCONTEXTMENU_H

#include "ContextMenu.h"

class Explorer;
class Material;
class Texture2D;
class ExplorerContextMenu : public ContextMenu
{
    Q_OBJECT

public:
    ExplorerContextMenu(Explorer *explorer);

public slots:
    void OnCustomContextMenuRequested(QPoint point) override;

    Texture2D* OnCreateTextureFromImageClicked();
    Material* OnCreateMaterialFromImageClicked();
    Material* OnCreateMaterialFromTextureClicked(Texture2D *tex = nullptr);

    void OnDuplicateClicked();
    void OnDeleteClicked();
    void OnCreateDirClicked();

private:
    Explorer *p_explorer = nullptr;
};

#endif // EXPLORERCONTEXTMENU_H
