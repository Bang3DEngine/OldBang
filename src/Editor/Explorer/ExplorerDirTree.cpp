#include "Bang/ExplorerDirTree.h"

#include <QTimer>
#include <QMouseEvent>
#include <QToolButton>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/Debug.h"
#include "Bang/Explorer.h"
#include "Bang/EditorWindow.h"

ExplorerDirTree::ExplorerDirTree(QWidget *parent) : QTreeView(parent)
{
    m_fileSystemModel = new QFileSystemModel();

    m_topPath = Paths::EngineAssets();

    setModel(m_fileSystemModel);
    SetDir(m_topPath);

    m_fileSystemModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    this->setColumnHidden(1, true);
    this->setColumnHidden(2, true);
    this->setColumnHidden(3, true);

    connect(m_fileSystemModel, SIGNAL(directoryLoaded(QString)),
            this, SLOT(OnDirLoaded(QString)));

    //Every X seconds, update all the slots values
    connect(&m_checkSelectionTimer, SIGNAL(timeout()),
            this, SLOT(CheckSelection()));
    m_checkSelectionTimer.start(100);
}

ExplorerDirTree::~ExplorerDirTree()
{
   if (m_fileSystemModel) { delete m_fileSystemModel; }
}

void ExplorerDirTree::CheckSelection()
{
    SetDir(Paths::ProjectAssets());

    m_explorer = EditorWindow::GetInstance()->widgetListExplorer;
    if (selectedIndexes().size() > 0 && selectedIndexes().at(0).isValid())
    {
        QModelIndex index = selectedIndexes().at(0);
        if (m_lastSelectedModelIndexPointer != index.internalPointer())
        {
            m_lastSelectedModelIndexPointer = index.internalPointer();
            String selectedDirPath =
                    m_fileSystemModel->filePath(index).toStdString();
            m_explorer->SetDir( Path(selectedDirPath) );
        }
    }
}

void ExplorerDirTree::OnDirLoaded(QString dir)
{
    this->expandAll();
}

void ExplorerDirTree::SetDir(const Path &path)
{
    setRootIndex(
            m_fileSystemModel->setRootPath(path.GetAbsolute().ToQString()));
}
