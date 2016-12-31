#include "SelectProjectWindow.h"

#include "ProjectManager.h"

SelectProjectWindow *SelectProjectWindow::s_instance = nullptr;

SelectProjectWindow::SelectProjectWindow()
{
}

String SelectProjectWindow::ExecAndGetProjectFilepath(QMainWindow *window, QApplication *app)
{
    SelectProjectWindow::s_instance = new SelectProjectWindow();
    SelectProjectWindow::s_instance->setupUi( window );

    SelectProjectWindow::s_instance->m_window = window;


    connect(SelectProjectWindow::s_instance->buttonCreateNewProject, SIGNAL(clicked()),
            SelectProjectWindow::s_instance, SLOT(OnCreateNewProject()));

    connect(SelectProjectWindow::s_instance->buttonLoadProject, SIGNAL(clicked()),
            SelectProjectWindow::s_instance, SLOT(OnLoadProject()));

    connect(app, SIGNAL(aboutToQuit()),
            SelectProjectWindow::s_instance, SLOT(OnClosed()));


    while ( SelectProjectWindow::s_instance->m_loadedProjectFile.Empty() &&
           !SelectProjectWindow::s_instance->m_directlyClosedByUser)
    {
        SelectProjectWindow::s_instance->m_directlyClosedByUser = true;
        window->show();
        app->exec();
    }

    if (SelectProjectWindow::s_instance->m_directlyClosedByUser)
    {
        return "";
    }
    else
    {
        return SelectProjectWindow::s_instance->m_loadedProjectFile;
    }
}

SelectProjectWindow *SelectProjectWindow::GetInstance()
{
    return SelectProjectWindow::s_instance;
}

QMainWindow* SelectProjectWindow::GetMainWindow() const
{
    return m_window;
}

void SelectProjectWindow::OnCreateNewProject()
{
    m_loadedProjectFile = ProjectManager::CreateNewProjectAndDialogs();
    if (!m_loadedProjectFile.Empty())
    {
        m_directlyClosedByUser = false;
        m_window->close();
    }
}

void SelectProjectWindow::OnLoadProject()
{
    m_loadedProjectFile = ProjectManager::OpenProjectAndDialogs();
    if (!m_loadedProjectFile.Empty())
    {
        m_directlyClosedByUser = false;
        m_window->close();
    }
}

#include <iostream>
void SelectProjectWindow::OnClosed()
{
    std::cout << "CLOSED!" << std::endl;
}
