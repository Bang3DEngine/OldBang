#include "Bang/AttrWidgetEnum.h"

ComboBox::ComboBox(QWidget *parent)
    : QComboBox(parent)
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
}

void ComboBox::wheelEvent(QWheelEvent *event)
{
    if (hasFocus())
    {
        QComboBox::wheelEvent(event);
    }
    else
    {
        event->ignore();
    }
}


