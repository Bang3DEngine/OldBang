#ifndef CURSOR_H
#define CURSOR_H


class Cursor
{
public:
    enum CursorIcon
    {
        Arrow,
        PointingHand,
        MoveArrows,
        OpenHand,
        ClosedHand
    };

    static void SetIcon(CursorIcon icon);

private:
    Cursor();

    static Cursor *GetInstance();

    friend class Input;
};

#endif // CURSOR_H
