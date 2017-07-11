#include "Bang/IDragDropListener.h"

#include "Bang/Object.h"
#include "Bang/DragDropManager.h"

QString IDragDropListener::acceptDragStyleOnlyBorder =
     "border: 1px solid #0F0";

QString IDragDropListener::AcceptDragStyle =
     "background-color: #151; " + IDragDropListener::acceptDragStyleOnlyBorder;

QString IDragDropListener::overDragStyle   =
     "background-color: #1B1; " + IDragDropListener::acceptDragStyleOnlyBorder;

QString IDragDropListener::RejectDragStyle =
     "background-color: #511; border: 1px solid #F00";

IDragDropListener::IDragDropListener()
{
    DragDropManager::RegisterDragDropAgent(this);
}

IDragDropListener::~IDragDropListener()
{
    DragDropManager::UnregisterDragDropAgent(this);
}

bool IDragDropListener::IsMouseOverMe() const
{
    const QWidget *w = Object::ConstCast<QWidget>(this);
    return IDragDropListener::IsMouseOver(w);
}

bool IDragDropListener::IsMouseOver(const QWidget *w)
{
    return w->rect().contains(w->mapFromGlobal(QCursor::pos()));
}
