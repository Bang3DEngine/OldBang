#ifndef DRAGDROPMANAGER_H
#define DRAGDROPMANAGER_H

#include <QObject>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>

#include <QTimer>

#include "List.h"
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

    // Custom content you want to put here (data being dragged).
    void *content = nullptr;
};

// Receives drag/drop events and manages them.
// Notifies all the existing IDroppable's too
class DragDropAgent;
class IDragDropListener;
class DragDropManager : public QObject
{
    Q_OBJECT

public:
    static void RegisterDragDropAgent(IDragDropListener *dragDropListener);
    static void UnregisterDragDropAgent(IDragDropListener *dragDropListener);

    static void HandleGlobalMousePress(QObject *obj, QEvent *e);
    static void HandleGlobalMouseRelease(QObject *obj, QEvent *e);

    static QObject* GetDragSource();
    static bool IsDragGoingOn();

    static DragDropAgent* GetDragDropAgentBelowMouse();
    static DragDropInfo* GetDragDropInfo();

protected slots:
    void InstallEventFilters();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    static DragDropManager *s_ddManager;

    DragDropInfo m_ddInfo;
    const float c_UpdateTime = 0.05f;
    QTimer m_updateTimer;

    float m_timeSinceLastMouseDown = 0.0f;
    const float c_TimeToStartDrag = 0.1f;
    bool m_mouseDown = false;
    bool m_dragging  = false, m_latestUpdateDragging  = false;

    DragDropAgent *m_latestDDAgentBelowMouse = nullptr;
    List<IDragDropListener*> m_dragDropListeners;

    DragDropManager();

private slots:
    void Update();
};

#endif // DRAGDROPMANAGER_H
