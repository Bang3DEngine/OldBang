#ifndef INSPECTORCONTEXTMENU_H
#define INSPECTORCONTEXTMENU_H

#include <QPoint>

#include "ContextMenu.h"

class Inspector;
class InspectorContextMenu : public ContextMenu
{
    Q_OBJECT

private:
    Inspector *m_inspector;

public:
    InspectorContextMenu(Inspector *inspector);

public slots:
    void OnCustomContextMenuRequested(QPoint point) override;
};

#endif // INSPECTORCONTEXTMENU_H
