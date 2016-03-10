#include "Explorer.h"

Explorer::Explorer(QWidget *parent) : QListView(parent)
{
    fileSystemModel = new QFileSystemModel();

    QString rootPath = QT_PROJECT_PATH;

    Logger_Log(rootPath.toStdString());

    setModel(fileSystemModel);
    setRootIndex(fileSystemModel->setRootPath(rootPath));
    fileSystemModel->setReadOnly(false);
    setAcceptDrops(true);
    setDragDropOverwriteMode(false);
    setAlternatingRowColors(true);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    setDefaultDropAction(Qt::DropAction::MoveAction);
    setDragDropMode(DragDropMode::InternalMove);
    setDropIndicatorShown(true);

    setViewMode(ViewMode::IconMode);
    setResizeMode(ResizeMode::Adjust);
}

void Explorer::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        QModelIndex clickedIndex = this->selectedIndexes().at(0);
        bool isDir = fileSystemModel->isDir(clickedIndex);
        if(isDir)
        {
            std::string clickedDirName = fileSystemModel->fileName(clickedIndex).toStdString();
            setRootIndex(fileSystemModel->setRootPath(
                             QString::fromStdString(fileSystemModel->rootPath().toStdString() + "/" + clickedDirName)
                             )
                         );
        }
    }
}

