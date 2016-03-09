#include "Explorer.h"

Explorer::Explorer(QWidget *parent) : QListView(parent)
{
    fileSystemModel = new QFileSystemModel();

    rootPath = QT_PROJECT_PATH;

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

