#include "ExplorerDirTree.h"

#include "EditorWindow.h"

ExplorerDirTree::ExplorerDirTree(QWidget *parent) : QTreeView(parent)
{
    m_fileSystemModel = new QFileSystemModel();

    m_topPath = QT_PROJECT_PATH;
    m_topPath += "/Assets";

    setModel(m_fileSystemModel);
    setDir(m_topPath);

    m_fileSystemModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    this->setColumnHidden(1, true);
    this->setColumnHidden(2, true);
    this->setColumnHidden(3, true);

    connect(m_fileSystemModel, SIGNAL(directoryLoaded(QString)), this, SLOT(OnDirLoaded(QString)));

    m_checkSelectionTimer = new QTimer(this); //Every X seconds, update all the slots values
    connect(m_checkSelectionTimer, SIGNAL(timeout()), this, SLOT(CheckSelection()));
    m_checkSelectionTimer->start(100);
}

ExplorerDirTree::~ExplorerDirTree()
{
   delete m_fileSystemModel;
}

void ExplorerDirTree::CheckSelection()
{
    m_explorer = EditorWindow::GetInstance()->widgetListExplorer;
    if (selectedIndexes().size() > 0 && selectedIndexes().at(0).isValid())
    {
        QModelIndex index = selectedIndexes().at(0);
        if (m_lastSelectedModelIndexPointer != index.internalPointer())
        {
            m_lastSelectedModelIndexPointer = index.internalPointer();
            String selectedDirPath =
                    m_fileSystemModel->filePath(index).toStdString();
            m_explorer->SetDir(selectedDirPath);
        }
    }
}

void ExplorerDirTree::OnDirLoaded(QString dir)
{
    this->expandAll();
}

void ExplorerDirTree::setDir(const String &path)
{
    setRootIndex(m_fileSystemModel->setRootPath(QString::fromStdString(path)));
}
