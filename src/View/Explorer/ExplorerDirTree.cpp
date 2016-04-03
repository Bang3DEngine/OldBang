#include "ExplorerDirTree.h"

#include "WindowMain.h"

ExplorerDirTree::ExplorerDirTree(QWidget *parent) : QTreeView(parent)
{
    fileSystemModel = new QFileSystemModel();

    topPath = QT_PROJECT_PATH;
    topPath += "/res/Assets";

    setModel(fileSystemModel);
    setDir(topPath);

    fileSystemModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);

    this->setColumnHidden(1, true);
    this->setColumnHidden(2, true);
    this->setColumnHidden(3, true);

    connect(this, SIGNAL(currentChanged(const QModelIndex, const QModelIndex)),
            this, SLOT(OnDirSelected(const QModelIndex, const QModelIndex)));
    connect(fileSystemModel, SIGNAL(directoryLoaded(QString)), this, SLOT(OnDirLoaded(QString)));
}

ExplorerDirTree::~ExplorerDirTree()
{
   delete fileSystemModel;
}

void ExplorerDirTree::OnDirSelected(const QModelIndex & current, const QModelIndex & previous)
{
    explorer = WindowMain::GetInstance()->widgetListExplorer;
    if(current.isValid())
    {
        std::string selectedDirPath = fileSystemModel->filePath(current).toStdString();
        explorer->setDir(selectedDirPath);
    }
}

void ExplorerDirTree::OnDirLoaded(QString dir)
{
    this->expandAll();
}

void ExplorerDirTree::setDir(const std::string &path)
{
    setRootIndex(fileSystemModel->setRootPath(QString::fromStdString(path)));
}
