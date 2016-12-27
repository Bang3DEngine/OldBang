#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <QPoint>
#include <QWidget>

class ContextMenu : public QObject
{
    Q_OBJECT

public:
    ContextMenu(QWidget *widget);

public slots:
    virtual void OnCustomContextMenuRequested(QPoint point) = 0;
};

#endif // CONTEXTMENU_H
