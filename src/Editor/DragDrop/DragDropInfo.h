#ifndef DRAGDROPINFO_H
#define DRAGDROPINFO_H

#include <QObject>
#include "Bang/WinUndef.h"

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

#endif // DRAGDROPINFO_H
