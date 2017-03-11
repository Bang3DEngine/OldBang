#include "ExplorerContextMenu.h"

#include <QTreeWidgetItem>

#include "Debug.h"
#include "Dialog.h"
#include "Prefab.h"
#include "Explorer.h"
#include "Inspector.h"
#include "EditorWindow.h"
#include "GameObjectClipboard.h"
#include "IO.h"

ExplorerContextMenu::ExplorerContextMenu(Explorer *explorer) :
    ContextMenu(explorer), m_explorer(explorer)
{
}

void ExplorerContextMenu::OnCustomContextMenuRequested(QPoint point)
{
    QMenu contextMenu(tr("Explorer context menu"), m_explorer);

    QAction       actionDuplicate("Duplicate",     m_explorer);
    QAction          actionDelete("Delete",        m_explorer);
    QAction    actionCreateDir("Create directory", m_explorer);

    connect(&actionDuplicate,SIGNAL(triggered()),
            this, SLOT(OnDuplicateClicked()));
    connect(&actionDelete, SIGNAL(triggered()),
            this, SLOT(OnDeleteClicked()));
    connect(&actionCreateDir, SIGNAL(triggered()),
            this, SLOT(OnCreateDirClicked()));

    QPoint mousePoint = m_explorer->viewport()->mapFromGlobal(point);
    if (m_explorer->indexAt(mousePoint).isValid())
    {
        if (m_explorer->IsSelectedAFile())
        {
            contextMenu.addAction(&actionDuplicate);
            contextMenu.addAction(&actionDelete);
        }
        else if (m_explorer->IsSelectedADir())
        {
            contextMenu.addAction(&actionDuplicate);
            contextMenu.addAction(&actionDelete);
        }
    }
    else
    {
        QMenu *assetsMenu = EditorWindow::GetInstance()->menuAssets;
        contextMenu.addActions(assetsMenu->actions());
        contextMenu.addSeparator();
        contextMenu.addAction(&actionCreateDir);
    }

    contextMenu.exec(m_explorer->viewport()->mapToGlobal(point));
}

void ExplorerContextMenu::OnDuplicateClicked()
{
    ASSERT(!m_explorer->GetSelectedFileOrDirPath().Empty());

    String fromPath = m_explorer->GetSelectedFileOrDirPath();
    String toPath = IO::GetDuplicateName(fromPath);
    if (IO::IsFile(fromPath))
    {
        IO::DuplicateFile(fromPath, toPath);
    }
    else if (IO::IsDir(fromPath))
    {
        IO::DuplicateDir(fromPath, toPath);
    }

    m_explorer->SelectFile(toPath);
}

void ExplorerContextMenu::OnDeleteClicked()
{
    String path = m_explorer->GetSelectedFile().GetAbsolutePath();
    String name = IO::GetFileNameWithExtension(path);
    ASSERT( IO::Exists(path) );

    Dialog::Reply reply = Dialog::GetYesNo(
                "Delete file or directory",
                "Are you sure you want to remove '" + name + "' ? \n" +
                "This can NOT be undone.");

    if (reply == Dialog::Reply::Yes)
    {
        Inspector *inspector = Inspector::GetInstance();
        IInspectable *lastInspectable =
                m_explorer->m_lastIInspectableInInspector;
        if (inspector->IsShowingInspectable(lastInspectable))
        {
            inspector->Clear();
            delete lastInspectable;
            m_explorer->m_lastIInspectableInInspector = nullptr;
        }
        IO::Remove(path);
    }
}

void ExplorerContextMenu::OnCreateDirClicked()
{
    String currentDir = m_explorer->GetCurrentDir();
    String dirPath = currentDir + "/New_Folder";
    dirPath = IO::GetDuplicateName(dirPath);
    dirPath = IO::ToAbsolute(dirPath, false);

    IO::CreateDirectory(dirPath);
    m_explorer->StartRenaming(dirPath);
}
