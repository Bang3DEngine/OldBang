#include "ExplorerContextMenu.h"

#include "Explorer.h"

ExplorerContextMenu::ExplorerContextMenu(Explorer *explorer) :
    ContextMenu(explorer), m_explorer(explorer)
{
}

void ExplorerContextMenu::OnCustomContextMenuRequested(QPoint point)
{
    QMenu contextMenu(tr("Explorer context menu"), m_explorer);

    QAction actionDuplicate("Duplicate", m_explorer);
    QAction    actionDelete("Delete",    m_explorer);

    connect(&actionDuplicate, SIGNAL(triggered()), this, SLOT(OnDuplicateClicked()));
    connect(&actionDelete,    SIGNAL(triggered()), this, SLOT(OnDeleteClicked()));

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

    contextMenu.exec(m_explorer->mapToGlobal(point));
}

void ExplorerContextMenu::OnDuplicateClicked()
{
    File source = m_explorer->GetSelectedFile();
    String filepath = source.GetRelativePath();
    while (m_explorer->Exists(filepath))
    {
        filepath = Persistence::GetNextDuplicateName(filepath);
    }

    FileWriter::WriteToFile(filepath, source.GetContents());
}

void ExplorerContextMenu::OnDeleteClicked()
{
    Inspector *inspector = Inspector::GetInstance();
    IInspectable *lastInspectable = m_explorer->m_lastIInspectableInInspector;
    if (inspector->IsShowingInspectable(lastInspectable))
    {
        inspector->Clear();
        delete lastInspectable;
        m_explorer->m_lastIInspectableInInspector = nullptr;
    }
    m_explorer->m_fileSystemModel->remove(m_explorer->currentIndex());
}
