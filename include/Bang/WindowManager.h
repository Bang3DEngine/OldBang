#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <stack>

#include "Bang/Bang.h"
#include "Bang/List.h"

NAMESPACE_BANG_BEGIN

FORWARD class Window;
FORWARD class DialogWindow;

class WindowManager
{
public:
	WindowManager();
	virtual ~WindowManager();

    void Init();
    int  MainLoop();
    bool MainLoopIteration();
    void OnBlockingWaitBegin(Window *win);
    void OnBlockingWaitEnd();
    void DestroyWindow(Window *window);

    template<class WinT = Window>
    static WinT* CreateWindow(uint flags = 0);
    static DialogWindow *CreateDialogWindow(Window *parentWindow,
                                            bool resizable);
    static Window* GetTopWindow();

    static WindowManager *GetInstance();

private:
    std::stack< List<Window*> > m_windowsStack;
    List<Window*> p_windowsToBeDestroyed;

    static void SetupWindow(Window *window, uint flags);

    bool HandleEvents();
    void DestroyQueuedWindows();

    List<Window*>& GetCurrentWindows();
    const List<Window*>& GetCurrentWindows() const;
};

NAMESPACE_BANG_END

#include "WindowManager.tcc"

#endif // WINDOWMANAGER_H

