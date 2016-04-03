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

    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(OnDirSelected()));
    connect(fileSystemModel, SIGNAL(directoryLoaded(QString)), this, SLOT(OnDirLoaded(QString)));


    checkSelectionTimer = new QTimer(this); //Every X seconds, update all the slots values
    connect(checkSelectionTimer, SIGNAL(timeout()), this, SLOT(CheckSelection()));
    checkSelectionTimer->start(100);
}

ExplorerDirTree::~ExplorerDirTree()
{
   delete fileSystemModel;
}

void ExplorerDirTree::CheckSelection()
{
    explorer = WindowMain::GetInstance()->widgetListExplorer;
    if(selectedIndexes().size() > 0 && selectedIndexes().at(0).isValid())
    {
        QModelIndex index = selectedIndexes().at(0);
        if(lastSelectedModelIndexPointer != index.internalPointer())
        {
            lastSelectedModelIndexPointer = index.internalPointer();
            std::string selectedDirPath =
                    fileSystemModel->filePath(index).toStdString();
            explorer->setDir(selectedDirPath);
        }
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
