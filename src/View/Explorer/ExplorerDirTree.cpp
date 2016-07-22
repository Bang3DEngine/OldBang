#include "ExplorerDirTree.h"

#include "WindowMain.h"

ExplorerDirTree::ExplorerDirTree(QWidget *parent) : QTreeView(parent)
{
    p_fileSystemModel = new QFileSystemModel();

    m_topPath = QT_PROJECT_PATH;
    m_topPath += "/Assets";

    setModel(p_fileSystemModel);
    setDir(m_topPath);

    p_fileSystemModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    this->setColumnHidden(1, true);
    this->setColumnHidden(2, true);
    this->setColumnHidden(3, true);

    connect(p_fileSystemModel, SIGNAL(directoryLoaded(QString)), this, SLOT(OnDirLoaded(QString)));

    p_checkSelectionTimer = new QTimer(this); //Every X seconds, update all the slots values
    connect(p_checkSelectionTimer, SIGNAL(timeout()), this, SLOT(CheckSelection()));
    p_checkSelectionTimer->start(100);
}

ExplorerDirTree::~ExplorerDirTree()
{
   delete p_fileSystemModel;
}

void ExplorerDirTree::CheckSelection()
{
    p_explorer = WindowMain::GetInstance()->widgetListExplorer;
    if (selectedIndexes().size() > 0 && selectedIndexes().at(0).isValid())
    {
        QModelIndex index = selectedIndexes().at(0);
        if (p_lastSelectedModelIndexPointer != index.internalPointer())
        {
            p_lastSelectedModelIndexPointer = index.internalPointer();
            std::string selectedDirPath =
                    p_fileSystemModel->filePath(index).toStdString();
            p_explorer->SetDir(selectedDirPath);
        }
    }
}

void ExplorerDirTree::OnDirLoaded(QString dir)
{
    this->expandAll();
}

void ExplorerDirTree::setDir(const std::string &path)
{
    setRootIndex(p_fileSystemModel->setRootPath(QString::fromStdString(path)));
}
