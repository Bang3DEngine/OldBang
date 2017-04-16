#include "Bang/IDragDropListener.h"

#include "Bang/Object.h"

#ifdef BANG_EDITOR
#include "Bang/DragDropManager.h"
#endif

QString IDragDropListener::acceptDragStyleOnlyBorder =
     "border: 1px solid #0F0";

QString IDragDropListener::acceptDragStyle =
     "background-color: #151; " + IDragDropListener::acceptDragStyleOnlyBorder;

QString IDragDropListener::overDragStyle   =
     "background-color: #1B1; " + IDragDropListener::acceptDragStyleOnlyBorder;

QString IDragDropListener::rejectDragStyle =
     "background-color: #511; border: 1px solid #F00";

IDragDropListener::IDragDropListener()
{
    DragDropManager::RegisterDragDropAgent(this);
}

IDragDropListener::~IDragDropListener()
{
    DragDropManager::UnregisterDragDropAgent(this);
}

bool IDragDropListener::MouseOverMe() const
{
    IDragDropListener *noConstThis = const_cast<IDragDropListener*>(this);
    QWidget *w = Object::Cast<QWidget>(noConstThis);
    if (w)
    {
        return IDragDropListener::MouseOver(w);
    }
    return false;
}

bool IDragDropListener::MouseOver(QWidget *w)
{
    return w->rect().contains(w->mapFromGlobal(QCursor::pos()));
}
