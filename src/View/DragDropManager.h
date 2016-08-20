#ifndef DRAGDROPMANAGER_H
#define DRAGDROPMANAGER_H

#include <QObject>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

#include "Vector2.h"

class DragDropInfo
{
public:

    // The source of the drag (where it started)
    QObject *sourceObject = nullptr;

    // The currentWidget in the previous event
    // (useful to know where you come from in DragEnterEvent for example)
    QObject *previousObject = nullptr;

    // The widget that the mouse is overing
    QObject *currentObject = nullptr;

    // The latest event
    QEvent *currentEvent;
};

// Receives drag/drop events and manages them.
// Notifies all the existing IDroppable's too
class DragDropAgent;
class IDragDropListener;
class DragDropManager : public QObject
{
    Q_OBJECT

private:
    static DragDropManager *s_ddManager;

    DragDropInfo m_ddInfo;
    bool m_dragGoingOn = false;
    std::list<IDragDropListener*> m_dragDropListeners;

    DragDropManager();

protected slots:
    void InstallEventFilters();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

public:
    static void RegisterDragDropAgent(IDragDropListener *dragDropListener);
    static void UnregisterDragDropAgent(IDragDropListener *dragDropListener);

    static void HandleDragEnterEvent(QObject *obj, QDragEnterEvent *e);
    static void HandleDragMoveEvent(QObject *obj, QDragMoveEvent *e);
    static void HandleDragLeaveEvent(QObject *obj, QDragLeaveEvent *e);
    static void HandleDropEvent(QObject *obj, QEvent *e);

    static void HandleGlobalMousePress(QObject *obj, QEvent *e);
    static void HandleGlobalMouseRelease(QObject *obj, QEvent *e);
};

#endif // DRAGDROPMANAGER_H
