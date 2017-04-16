#ifndef DRAGDROPMANAGER_H
#define DRAGDROPMANAGER_H

#include <QTimer>
#include <QObject>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include "Bang/WinUndef.h"

#include "Bang/List.h"
#include "Bang/Vector2.h"
#include "Bang/DragDropInfo.h"

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

private:
    static DragDropManager *s_ddManager;

    DragDropInfo m_ddInfo;
    QTimer m_updateTimer;

    const int c_updateTime = 100;
    int m_timeSinceLastMouseDown = 0;
    const int c_TimeToStartDrag = 100;
    bool m_mouseDown = false;
    bool m_dragging  = false;
    bool m_latestUpdateDragging  = false;

    DragDropAgent *m_latestDDAgentBelowMouse = nullptr;
    List<IDragDropListener*> m_dragDropListeners;

    DragDropManager();

private slots:
    void Update();
};

#endif // DRAGDROPMANAGER_H
