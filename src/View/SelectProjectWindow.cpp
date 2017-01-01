#include "SelectProjectWindow.h"

#include "Debug.h"
#include "Project.h"
#include "Persistence.h"
#include "EngineConfig.h"
#include "ProjectManager.h"

SelectProjectWindow *SelectProjectWindow::s_instance = nullptr;

SelectProjectWindow::SelectProjectWindow()
{
}

String SelectProjectWindow::ExecAndGetProjectFilepath(QMainWindow *window, QApplication *app)
{
    SelectProjectWindow::s_instance = new SelectProjectWindow();
    SelectProjectWindow *inst = SelectProjectWindow::s_instance;
    inst->setupUi( window );

    inst->m_window = window;

    connect(inst->listRecentProjects, SIGNAL(cellClicked(int,int)),
            inst, SLOT(OnRecentProjectListSelectionChanged(int,int)) );

    connect(inst->buttonCreateNewProject, SIGNAL(clicked()),
            inst, SLOT(OnCreateNewProject()));

    connect(inst->buttonBrowseProject, SIGNAL(clicked()),
            inst, SLOT(OnBrowseProject()));

    connect(inst->buttonLoadRecentProject, SIGNAL(clicked()),
            inst, SLOT(OnLoadRecentProject()));

    // Create recent project list and select first item
    inst->FillRecentProjectsList();
    inst->listRecentProjects->setCurrentItem(inst->listRecentProjects->item(0,0));
    inst->OnRecentProjectListSelectionChanged(0,0);

    while ( inst->m_loadedProjectFile.Empty() && !inst->m_directlyClosedByUser)
    {
        inst->m_directlyClosedByUser = true;
        window->show();
        app->exec();
    }

    if (inst->m_directlyClosedByUser)
    {
        return "";
    }
    else
    {
        EngineConfig::RegisterInRecentProjectsList(inst->m_loadedProjectFile);
        return inst->m_loadedProjectFile;
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

void SelectProjectWindow::FillRecentProjectsList()
{
    listRecentProjects->clear();
    EngineConfig::CleanOutdatedRecentProjectList();

    int i = 0;
    List<String> recentProjectsFilepath = EngineConfig::GetRecentProjectsList();
    listRecentProjects->setRowCount(recentProjectsFilepath.Size());
    listRecentProjects->setColumnCount(2);
    for (String recentProject : recentProjectsFilepath)
    {
        QTableWidgetItem *projNameItem = new QTableWidgetItem(
                    Persistence::GetFileName(recentProject).ToQString());
        QTableWidgetItem *projPathItem = new QTableWidgetItem(
                    recentProject.ToQString());
        listRecentProjects->setItem(i, 0, projNameItem);
        listRecentProjects->setItem(i, 1, projPathItem);
        ++i;
    }
}

void SelectProjectWindow::OnRecentProjectListSelectionChanged(int row, int column)
{
    QTableWidgetItem *selectedItem = listRecentProjects->item(row, 1);
    NONULL(selectedItem);

    m_selectedRecentProjectFilepath = selectedItem->text();

    String newButtonText =
        "Load " + Persistence::GetFileName(m_selectedRecentProjectFilepath);

    buttonLoadRecentProject->setText(newButtonText.ToQString());
}

void SelectProjectWindow::OnLoadRecentProject()
{
    if (m_selectedRecentProjectFilepath.Empty()) return;

    m_loadedProjectFile = m_selectedRecentProjectFilepath;
    m_directlyClosedByUser = false;
    m_window->close();
}

void SelectProjectWindow::OnCreateNewProject()
{
    m_loadedProjectFile = ProjectManager::DialogCreateNewProject();
    if (!m_loadedProjectFile.Empty())
    {
        m_directlyClosedByUser = false;
        m_window->close();
    }
}

void SelectProjectWindow::OnBrowseProject()
{
    m_loadedProjectFile = ProjectManager::DialogOpenProject();
    if (!m_loadedProjectFile.Empty())
    {
        m_directlyClosedByUser = false;
        m_window->close();
    }
}
