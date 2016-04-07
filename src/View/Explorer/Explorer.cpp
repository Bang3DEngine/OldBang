#include "Explorer.h"

#include "WindowMain.h"

std::string Explorer::topPath = "";

Explorer::Explorer(QWidget *parent) : QListView(parent)
{
    fileSystemModel = new QFileSystemModel();

    topPath = QT_PROJECT_PATH;
    topPath += "/res/Assets";


    buttonDirUp = WindowMain::GetInstance()->buttonExplorerDirUp;
    buttonChangeViewMode = WindowMain::GetInstance()->buttonExplorerChangeViewMode;

    connect(buttonDirUp, SIGNAL(clicked()), this, SLOT(OnButtonDirUpClicked()));
    connect(buttonChangeViewMode, SIGNAL(clicked()), this, SLOT(OnButtonChangeViewModeClicked()));

    setModel(fileSystemModel);
    fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDot);
    fileSystemModel->setReadOnly(false);

    connect(fileSystemModel, SIGNAL(directoryLoaded(QString)), this, SLOT(OnDirLoaded(QString)));
    setDir(topPath);
}

Explorer::~Explorer()
{
   delete fileSystemModel;
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

void Explorer::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        QModelIndex clickedIndex = this->selectedIndexes().at(0);
        std::string clickedName = fileSystemModel->fileName(clickedIndex).toStdString();

        bool isFile = !fileSystemModel->isDir(clickedIndex);
        InspectorWidget *fileWidget = nullptr;
        if(isFile)
        {

            fileWidget = new InspectorTexture2DWidget(clickedName);
        }

        if(fileWidget != nullptr)
        {
            WindowMain::GetInstance()->widgetInspector->SetWidget(fileWidget);
        }
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

void Explorer::dropEvent(QDropEvent *e)
{
    if (e->source() != this)
    {
        e->accept();
    }
    else
    {
        //e->ignore();
        e->accept();
    }

    Logger_Log("Drop event in explorer " << e->source());
}

void Explorer::setDir(const std::string &path)
{
    setRootIndex(fileSystemModel->setRootPath(QString::fromStdString(path)));
}

void Explorer::OnDirLoaded(QString dir)
{
    if(getCurrentDir().length() <= topPath.length())
    {
        buttonDirUp->setEnabled(false);
        fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    }
    else
    {
        buttonDirUp->setEnabled(true);
        fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDot);
    }
}

std::string Explorer::getCurrentDir() const
{
    return fileSystemModel->rootPath().toStdString();
}
