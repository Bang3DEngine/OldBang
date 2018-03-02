#ifndef WINDOWMANAGER_TCC
#define WINDOWMANAGER_TCC

#include "Bang.h"

NAMESPACE_BANG_BEGIN

template<class WinT>
WinT* WindowManager::CreateWindow(uint flags)
{
    WinT *win = new WinT();
    SetupWindow(win, flags);
    return win;
}

NAMESPACE_BANG_END

#endif // WINDOWMANAGER_TCC

