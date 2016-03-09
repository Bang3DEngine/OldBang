#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include "Bang.h"

#include  "Logger.h"
#include "WindowMainAutoGenerated.h"

class WindowMain : public Ui_WindowMain
{
public:
    enum MenuAction
    {
        CREATE_PREFAB
    };

private:
    static WindowMain *win;

public:
    WindowMain();

    static void Init(QMainWindow *window);
    static WindowMain *GetInstance();
};

#endif // WINDOWMAIN_H
