#include "ExplorerContextMenu.h"

#include <QTreeWidgetItem>

#include "Debug.h"
#include "Dialog.h"
#include "Prefab.h"
#include "Explorer.h"
#include "Inspector.h"
#include "GameObjectClipboard.h"

#include "Persistence.h"

ExplorerContextMenu::ExplorerContextMenu(Explorer *explorer) :
    ContextMenu(explorer), m_explorer(explorer)
{
}

void ExplorerContextMenu::OnCustomContextMenuRequested(QPoint point)
{
    QMenu contextMenu(tr("Explorer context menu"), m_explorer);

    QAction       actionDuplicate("Duplicate",     m_explorer);
    QAction          actionDelete("Delete",        m_explorer);
    QAction    actionCreateFolder("Create folder", m_explorer);

    connect(&actionDuplicate,SIGNAL(triggered()),
            this, SLOT(OnDuplicateClicked()));
    connect(&actionDelete, SIGNAL(triggered()),
            this, SLOT(OnDeleteClicked()));
    connect(&actionCreateFolder, SIGNAL(triggered()),
            this, SLOT(OnCreateFolderClicked()));

    if (m_explorer->indexAt(point).isValid())
    {
        if (m_explorer->IsSelectedAFile())
        {
            contextMenu.addAction(&actionDuplicate);
            contextMenu.addAction(&actionDelete);
        }
        else if (m_explorer->IsSelectedADir())
        {
            // contextMenu.addAction(&actionDuplicate);
            contextMenu.addAction(&actionDelete);
        }
    }
    else
    {
        contextMenu.addAction(&actionCreateFolder);
    }

    contextMenu.exec(m_explorer->mapToGlobal(point));
}

void ExplorerContextMenu::OnDuplicateClicked()
{
    ASSERT(!m_explorer->GetSelectedFileOrDirPath().Empty());

    String fromPath = m_explorer->GetSelectedFileOrDirPath();
    String toPath = Persistence::GetDuplicateName(fromPath, m_explorer);
    Persistence::CopyFile(fromPath, toPath);
    m_explorer->SelectFile(toPath);
}

void ExplorerContextMenu::OnDeleteClicked()
{
    String path = m_explorer->GetSelectedFile().GetAbsolutePath();
    String name = Persistence::GetFileNameWithExtension(path);
    Dialog::Reply reply = Dialog::GetYesNo(
                "Delete file or directory",
                "Are you sure you want to remove '" + name + "' ? \n" +
                "This can NOT be undone.");

    if (reply == Dialog::Reply::Yes)
    {
        Inspector *inspector = Inspector::GetInstance();
        IInspectable *lastInspectable = m_explorer->m_lastIInspectableInInspector;
        if (inspector->IsShowingInspectable(lastInspectable))
        {
            inspector->Clear();
            delete lastInspectable;
            m_explorer->m_lastIInspectableInInspector = nullptr;
        }
        Persistence::Remove(path);
    }
}

void ExplorerContextMenu::OnCreateFolderClicked()
{
    String currentDir = m_explorer->GetCurrentDir();
    String dirPath = currentDir + "/New_Folder";
    dirPath = Persistence::GetDuplicateName(dirPath, m_explorer);
    dirPath = Persistence::ToAbsolute(dirPath, false);

    QDir dir = QDir::root();
    dir.mkpath(dirPath.ToQString());

    m_explorer->StartRenaming(dirPath);
}
