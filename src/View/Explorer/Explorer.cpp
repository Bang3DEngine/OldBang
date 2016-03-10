#include "Explorer.h"

#include "WindowMain.h"

Explorer::Explorer(QWidget *parent) : QListView(parent)
{
    fileSystemModel = new QFileSystemModel();

    topPath = QT_PROJECT_PATH;

    buttonDirUp = WindowMain::GetInstance()->buttonExplorerDirUp;
    buttonChangeViewMode = WindowMain::GetInstance()->buttonExplorerChangeViewMode;
    setModel(fileSystemModel);
    setDir(topPath);
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

    connect(buttonDirUp, SIGNAL(clicked()), this, SLOT(OnButtonDirUpClicked()));
    connect(buttonChangeViewMode, SIGNAL(clicked()), this, SLOT(OnButtonChangeViewModeClicked()));
}

void Explorer::OnButtonDirUpClicked()
{
    std::string rootPath = getCurrentDir();
    std::string parentDirPath = "";

    for(int i = rootPath.length() - 1; i >= 0; --i)
    {
        if(rootPath[i] == '/')
        {
            parentDirPath = rootPath.substr(0, i);
            break;
        }
    }

    setDir(parentDirPath);
}

void Explorer::OnButtonChangeViewModeClicked()
{
    if(viewMode() == ViewMode::ListMode)
    {
        setViewMode(ViewMode::IconMode);
    }
    else
    {
        setViewMode(ViewMode::ListMode);
    }
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
            setDir(getCurrentDir() + "/" + clickedDirName);
        }
    }
}

void Explorer::setDir(const std::string &path)
{
    setRootIndex(fileSystemModel->setRootPath(QString::fromStdString(path)));

    if(getCurrentDir().length() <= topPath.length())
    {
        buttonDirUp->setEnabled(false);
    }
    else
    {
         buttonDirUp->setEnabled(true);
    }
}

std::string Explorer::getCurrentDir() const
{
    return fileSystemModel->rootPath().toStdString();
}
