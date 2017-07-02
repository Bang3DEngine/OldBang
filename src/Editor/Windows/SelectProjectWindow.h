#ifndef SELECTPROJECTWINDOW_H
#define SELECTPROJECTWINDOW_H

#include <QMainWindow>
#include "Bang/ui_SelectProjectWindow.h"
#include "Bang/WinUndef.h"

#include "Bang/Path.h"
#include "Bang/String.h"

class SelectProjectWindow :
        public QObject,
        public Ui_SelectProjectWindow
{
    Q_OBJECT

public:
    SelectProjectWindow();

    static Path ExecAndGetProjectFilepath(QMainWindow *window,
                                          QApplication *app);
    static SelectProjectWindow *GetInstance();
    QMainWindow* GetMainWindow() const;
    void FillRecentProjectsList();

public slots:
    void OnRecentProjectListSelectionChanged(int row, int column);
    void OnRecentProjectDoubleClicked(int row, int column);

    void OnLoadRecentProject();
    void OnCreateNewProject();
    void OnBrowseProject();

private:
    static SelectProjectWindow *s_instance;

    Path m_loadedProjectFile;
    QMainWindow *m_window = nullptr;
    bool m_directlyClosedByUser = false;
    Path m_selectedRecentProjectFilepath;
};

#endif // SELECTPROJECTWINDOW_H
