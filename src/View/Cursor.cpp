#include "Bang/Cursor.h"

#include <QCursor>

#include "Bang/Input.h"
#include "Bang/Application.h"

Cursor::Cursor()
{
}

#include "Bang/Debug.h"
void Cursor::SetIcon(Cursor::CursorIcon icon)
{
    QCursor cursor;
    if (icon == Cursor::CursorIcon::Arrow)
    {
        cursor.setShape(Qt::CursorShape::ArrowCursor);
    }
    else if (icon == Cursor::CursorIcon::PointingHand)
    {
        cursor.setShape(Qt::CursorShape::PointingHandCursor);
    }
    else if (icon == Cursor::CursorIcon::MoveArrows)
    {
        cursor.setShape(Qt::CursorShape::SizeAllCursor);
    }
    else if (icon == Cursor::CursorIcon::OpenHand)
    {
        cursor.setShape(Qt::CursorShape::OpenHandCursor);
    }
    else if (icon == Cursor::CursorIcon::ClosedHand)
    {
        cursor.setShape(Qt::CursorShape::ClosedHandCursor);
    }

    Application::GetInstance()->setOverrideCursor(cursor);
}

Cursor *Cursor::GetInstance()
{
    return Input::GetInstance()->m_cursor;
}

