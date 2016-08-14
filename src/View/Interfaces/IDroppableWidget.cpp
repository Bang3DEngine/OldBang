#include "IDroppableWidget.h"

#include "WindowMain.h"
#include "WindowEventManager.h"

#include "File.h"
#include "Explorer.h"
#include "Hierarchy.h"
#include "GameObject.h"

#include "WindowMain.h"
#include "WindowEventManager.h"

// IDROPPABLE
QString IDroppable::acceptDragStyle =
        "background-color: #151; border: 1px solid #0F0";

QString IDroppable::overDragStyle   =
        "background-color: #1B1; border: 1px solid #0F0";

QString IDroppable::rejectDragStyle =
        "background-color: #511; border: 1px solid #F00";


IDroppable::IDroppable()
{
}

IDroppable::~IDroppable()
{
}

void IDroppable::Init(QWidget *qw)
{
    if (!m_inited)
    {
        if (WindowMain::GetInstance() &&
            WindowMain::GetInstance()->GetMainWindow())
        {
            QMainWindow *mw = WindowMain::GetInstance()->GetMainWindow();
            QList<QWidget*> widgets = qFindChildren<QWidget*>(mw);
            foreach(QWidget *w, widgets)
            {
                w->setAcceptDrops(true);
                w->installEventFilter(qw);
            }
            m_inited = true;
        }
    }
}

void IDroppable::OnDragEnterEvent(QWidget *qw, QDragEnterEvent *e)
{
    Init(qw);
    if (e->source() == qw)
    {
        m_dragOriginWidget = qw;
        m_dragThatStartedHereIsGoingOn = true;
        WindowEventManager::NotifyDragStarted(qw);
    }
    e->accept();
}

void IDroppable::OnDragMoveEvent(QWidget *qw, QDragMoveEvent *e)
{
    Init(qw);
    e->accept();
}

void IDroppable::OnDragLeaveEvent(QWidget *qw, QDragLeaveEvent *e)
{
    Init(qw);
    e->accept();
}

void IDroppable::OnDropEvent(QWidget *qw, QDropEvent *e)
{
    Init(qw);
    e->accept();
    Explorer *explorer   = Explorer::GetInstance();
    Hierarchy *hierarchy = Hierarchy::GetInstance();
    if (e->source() == explorer && explorer->IsSelectedAFile())
    {
        OnDropFromExplorer(explorer->GetSelectedFile(), e);
    }
    else if (e->source() == hierarchy)
    {
        OnDropFromHierarchy(hierarchy->GetFirstSelectedGameObject(), e);
    }
}
//


// CTORS
IDroppableQWidget::IDroppableQWidget() : IDroppable()
{
    setAcceptDrops(true);
}

IDroppableQListView::IDroppableQListView() : IDroppable()
{
    setAcceptDrops(true);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
}


IDroppableQTreeWidget::IDroppableQTreeWidget() : IDroppable()
{
    setAcceptDrops(true);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);

}

IDroppableQWidget::~IDroppableQWidget()
{
}
IDroppableQListView::~IDroppableQListView()
{
}
IDroppableQTreeWidget::~IDroppableQTreeWidget()
{
}
//


// DRAG ENTER
void IDroppableQWidget::dragEnterEvent(QDragEnterEvent *e)
{
    QWidget::dragEnterEvent(e);
    IDroppable::OnDragEnterEvent(this, e);
}
void IDroppableQListView::dragEnterEvent(QDragEnterEvent *e)
{
    QListView::dragEnterEvent(e);
    IDroppable::OnDragEnterEvent(this, e);
}
void IDroppableQTreeWidget::dragEnterEvent(QDragEnterEvent *e)
{
    QTreeWidget::dragEnterEvent(e);
    IDroppable::OnDragEnterEvent(this, e);
}
//


// DRAG MOVE
void IDroppableQWidget::dragMoveEvent(QDragMoveEvent *e)
{
    QWidget::dragMoveEvent(e);
    IDroppable::OnDragMoveEvent(this, e);
}
void IDroppableQListView::dragMoveEvent(QDragMoveEvent *e)
{
    QListView::dragMoveEvent(e);
    IDroppable::OnDragMoveEvent(this, e);
}
void IDroppableQTreeWidget::dragMoveEvent(QDragMoveEvent *e)
{
    QTreeWidget::dragMoveEvent(e);
    IDroppable::OnDragMoveEvent(this, e);
}
//


// DRAG LEAVE
void IDroppableQWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    QWidget::dragLeaveEvent(e);
    IDroppable::OnDragLeaveEvent(this, e);
}
void IDroppableQListView::dragLeaveEvent(QDragLeaveEvent *e)
{
    QListView::dragLeaveEvent(e);
    IDroppable::OnDragLeaveEvent(this, e);
}
void IDroppableQTreeWidget::dragLeaveEvent(QDragLeaveEvent *e)
{
    QTreeWidget::dragLeaveEvent(e);
    IDroppable::OnDragLeaveEvent(this, e);
}
//


// DROP
void IDroppableQWidget::dropEvent(QDropEvent *e)
{
    QWidget::dropEvent(e);
    IDroppable::OnDropEvent(this, e);
}
void IDroppableQListView::dropEvent(QDropEvent *e)
{
    QListView::dropEvent(e);
    IDroppable::OnDropEvent(this, e);
}
void IDroppableQTreeWidget::dropEvent(QDropEvent *e)
{
    QTreeWidget::dropEvent(e);
    IDroppable::OnDropEvent(this, e);
}
//
