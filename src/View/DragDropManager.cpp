#include "DragDropManager.h"

#include "Logger.h"
#include "WindowMain.h"
#include "DragDropAgent.h"
#include "IDragDropListener.h"

DragDropManager *DragDropManager::s_ddManager = nullptr;

DragDropManager::DragDropManager()
{
    InstallEventFilters();
}

void DragDropManager::InstallEventFilters()
{
    // InstallEventFilter of all the widgets of the window :)
    // Do this every certain time, just in case new Widgets appear hehe
    QTimer::singleShot(500, this, SLOT(InstallEventFilters()));

    WindowMain *wm = WindowMain::GetInstance();
    if (wm && wm->GetMainWindow())
    {
        QMainWindow *mw = wm->GetMainWindow();
        QList<QWidget*> widgets = qFindChildren<QWidget*>(mw);
        foreach(QWidget *w, widgets)
        {
            w->setAcceptDrops(true);
            w->setMouseTracking(true);
            w->installEventFilter(DragDropManager::s_ddManager);
            w->installEventFilter(mw);
        }
    }
}

bool DragDropManager::eventFilter(QObject *obj, QEvent *e)
{
    /*
    // Here we receive all the events for all the IDroppable's
    // (since each of them has installed an event filter on us)

    if (e->type() == QEvent::MouseButtonRelease)
    {
        HandleDropEvent(nullptr, e);
    }
    */

    return QObject::eventFilter(obj, e);
}

void DragDropManager::RegisterDragDropAgent(IDragDropListener *dragDropListener)
{
    if (!DragDropManager::s_ddManager)
    {
        DragDropManager::s_ddManager = new DragDropManager();
    }
    DragDropManager::s_ddManager->m_dragDropListeners.push_back(dragDropListener);
}

void DragDropManager::UnregisterDragDropAgent(IDragDropListener *dragDropListener)
{
    DragDropManager::s_ddManager->m_dragDropListeners.remove(dragDropListener);
}

void DragDropManager::HandleDragEnterEvent(QObject *obj, QDragEnterEvent *e)
{
    DragDropManager *m = DragDropManager::s_ddManager;
    m->m_ddInfo.currentEvent = e;

    if (obj == e->source())
    {   // DRAG START
        m->m_ddInfo.currentObject  = obj;
        m->m_ddInfo.previousObject = obj;
        m->m_ddInfo.sourceObject   = obj;

        for (IDragDropListener *d : m->m_dragDropListeners)
        {
            d->OnDragStart(m->m_ddInfo);
            if ( d->MouseOverMe() )
            {
                d->OnDragStartHere(m->m_ddInfo);
            }
        }

        m->m_dragGoingOn = true;
    }
    else
    {   // DRAG ENTER
        m->m_ddInfo.currentObject   = obj;
        m->m_ddInfo.previousObject  = obj;

        for (IDragDropListener *d : m->m_dragDropListeners)
        {
            d->OnDragEnter(m->m_ddInfo);
            if ( d->MouseOverMe() )
            {
                d->OnDragEnterHere(m->m_ddInfo);
            }
        }

    }

    m->m_ddInfo.previousObject = m->m_ddInfo.currentObject;
}

void DragDropManager::HandleDragMoveEvent(QObject *obj, QDragMoveEvent *e)
{
    DragDropManager *m = DragDropManager::s_ddManager;
    m->m_ddInfo.currentObject   = obj;
    m->m_ddInfo.currentEvent = e;

    for (IDragDropListener *d : m->m_dragDropListeners)
    {
        d->OnDragMove(m->m_ddInfo);
        if ( d->MouseOverMe() )
        {
            d->OnDragMoveHere(m->m_ddInfo);
        }
    }

    m->m_ddInfo.previousObject = m->m_ddInfo.currentObject;
}

void DragDropManager::HandleDragLeaveEvent(QObject *obj, QDragLeaveEvent *e)
{
    DragDropManager *m = DragDropManager::s_ddManager;
    m->m_ddInfo.currentObject = obj;
    m->m_ddInfo.currentEvent = e;

    for (IDragDropListener *d : m->m_dragDropListeners)
    {
        d->OnDragLeave(m->m_ddInfo);
        if ( d->MouseOverMe() )
        {
            d->OnDragLeaveHere(m->m_ddInfo);
        }
    }

    m->m_ddInfo.previousObject = m->m_ddInfo.currentObject;
}

void DragDropManager::HandleDropEvent(QObject *obj, QEvent *e)
{
    DragDropManager *m = DragDropManager::s_ddManager;

    if (m->m_dragGoingOn)
    {
        m->m_ddInfo.currentEvent = e;
        m->m_ddInfo.currentObject = obj;

        for (IDragDropListener *d : m->m_dragDropListeners)
        {
            d->OnDrop(m->m_ddInfo);
            if ( d->MouseOverMe() )
            {
                d->OnDropHere(m->m_ddInfo);
            }
        }

        m->m_ddInfo.currentEvent  = nullptr;
        m->m_ddInfo.currentObject = nullptr;
        m->m_ddInfo.sourceObject  = nullptr;
        m->m_dragGoingOn = false;
    }
    e->accept();
}

void DragDropManager::HandleGlobalMousePress(QObject *obj, QEvent *e)
{
    DragDropManager *m = DragDropManager::s_ddManager;
    m->m_ddInfo.currentObject = obj;
    m->m_ddInfo.sourceObject  = obj;
    m->m_dragGoingOn = true;
}

void DragDropManager::HandleGlobalMouseRelease(QObject *obj, QEvent *e)
{
    DragDropManager::HandleDropEvent(obj, e);
}

QObject *DragDropManager::GetDragSource()
{
    DragDropManager *m = DragDropManager::s_ddManager;
    return m->m_ddInfo.sourceObject;
}

bool DragDropManager::IsDragGoingOn()
{
    DragDropManager *m = DragDropManager::s_ddManager;
    return m->m_dragGoingOn;
}

