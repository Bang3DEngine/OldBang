#ifndef DIALOG_H
#define DIALOG_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

FORWARD class DialogWindow;

class Dialog
{
public:
    static DialogWindow* Error(const String &title,
                               const String &msg);

    Dialog() = delete;
};

NAMESPACE_BANG_END

#endif // DIALOG_H
