#include "Explorer.h"

#include <QScrollBar>

#include "WindowMain.h"

Explorer::Explorer(QWidget *parent) : QListView(parent)
{
    fileSystemModel = new QFileSystemModel();

    buttonDirUp = WindowMain::GetInstance()->buttonExplorerDirUp;
    buttonChangeViewMode =
            WindowMain::GetInstance()->buttonExplorerChangeViewMode;

    connect(buttonDirUp, SIGNAL(clicked()), this, SLOT(OnButtonDirUpClicked()));
    connect(buttonChangeViewMode, SIGNAL(clicked()),
            this, SLOT(OnButtonChangeViewModeClicked()));

    setModel(fileSystemModel);
    fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDot);
    fileSystemModel->setReadOnly(false);

    connect(fileSystemModel, SIGNAL(directoryLoaded(QString)),
            this, SLOT(OnDirLoaded(QString)));
    setDir(Persistence::GetAssetsPathAbsolute());

    updateTimer = new QTimer(this); //Every X secs, update all the slots values
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
    updateTimer->start(100);
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
        RefreshInspector();
    }
}

void Explorer::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        if(this->selectedIndexes().length() <= 0) return;

        QModelIndex clickedIndex = this->selectedIndexes().at(0);
        bool isDir = fileSystemModel->isDir(clickedIndex);
        if(isDir)
        {
            std::string clickedDirName =
                    fileSystemModel->fileName(clickedIndex).toStdString();
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

void Explorer::RefreshInspector()
{
    if(this->selectedIndexes().size() <= 0) return;

    QModelIndex clickedIndex = this->selectedIndexes().at(0);
    File f(fileSystemModel, &clickedIndex);

    InspectorWidget *fileWidget = nullptr;
    if(f.IsImageFile())
    {
        FileImage fi(fileSystemModel, &clickedIndex);
        fileWidget = new InspectorImageFileWidget(fi);
    }
    else if(f.IsTexture2DAsset())
    {
        FileTexture2DAsset ft(fileSystemModel, &clickedIndex);
        fileWidget = new InspectorTexture2DAssetWidget(ft);
    }
    else if(f.IsMeshFile())
    {
        FileMesh fm(fileSystemModel, &clickedIndex);
        fileWidget = new InspectorMeshFileWidget(fm);
    }
    /*else if(f.IsMeshAsset())
    {
        FileMeshAsset ft(fileSystemModel, &clickedIndex);
        fileWidget = new InspectorMeshFileWidget(ft);
    }*/
    else
    {
        WindowMain::GetInstance()->widgetInspector->Clear();
    }

    if(fileWidget )
    {
        WindowMain::GetInstance()->widgetInspector->SetWidget(fileWidget);
    }
}

void Explorer::Refresh()
{
    if(selectedIndexes().length() > 0)
    {
       QModelIndex index = selectedIndexes().at(0);
       File f(fileSystemModel, &index);
       if(f.GetName() != lastSelectedFileName)
       {
           lastSelectedFileName = f.GetName();
           RefreshInspector();
       }
    }
}

void Explorer::setDir(const std::string &path)
{
    setRootIndex(fileSystemModel->setRootPath(QString::fromStdString(path)));
}

void Explorer::OnDirLoaded(QString dir)
{
    if(WindowMain::GetInstance() == nullptr) return;

    if(getCurrentDir().length() <= Persistence::GetAssetsPathAbsolute().length())
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

void Explorer::updateGeometries()
{
    QListView::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}
