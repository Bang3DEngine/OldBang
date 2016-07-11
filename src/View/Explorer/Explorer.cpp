#include "Explorer.h"

#include <QScrollBar>

#include "WindowMain.h"

Explorer::Explorer(QWidget *parent) : QListView(parent)
{
    p_fileSystemModel = new QFileSystemModel();

    p_buttonDirUp = WindowMain::GetInstance()->buttonExplorerDirUp;
    p_buttonChangeViewMode =
            WindowMain::GetInstance()->buttonExplorerChangeViewMode;

    connect(p_buttonDirUp, SIGNAL(clicked()), this, SLOT(OnButtonDirUpClicked()));
    connect(p_buttonChangeViewMode, SIGNAL(clicked()),
            this, SLOT(OnButtonChangeViewModeClicked()));

    setModel(p_fileSystemModel);
    p_fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDot);
    p_fileSystemModel->setReadOnly(false);

    connect(p_fileSystemModel, SIGNAL(directoryLoaded(QString)),
            this, SLOT(OnDirLoaded(QString)));
    SetDir(Persistence::GetAssetsPathAbsolute());

    p_updateTimer = new QTimer(this); //Every X secs, update all the slots values
    connect(p_updateTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
    p_updateTimer->start(100);
}

Explorer::~Explorer()
{
   delete p_fileSystemModel;
}

void Explorer::OnButtonDirUpClicked()
{
    std::string rootPath = GetCurrentDir();
    std::string parentDirPath = "";

    for(int i = rootPath.length() - 1; i >= 0; --i)
    {
        if(rootPath[i] == '/')
        {
            parentDirPath = rootPath.substr(0, i);
            break;
        }
    }

    SetDir(parentDirPath);
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
        bool isDir = p_fileSystemModel->isDir(clickedIndex);
        if(isDir)
        {
            std::string clickedDirName =
                    p_fileSystemModel->fileName(clickedIndex).toStdString();
            SetDir(GetCurrentDir() + "/" + clickedDirName);
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
    File f(p_fileSystemModel, &clickedIndex);

    InspectorWidget *fileWidget = nullptr;
    if(f.IsImageFile())
    {
        FileImage fi(p_fileSystemModel, &clickedIndex);
        fileWidget = new InspectorImageFileWidget(fi);
    }
    else if(f.IsTexture2DAsset())
    {
        FileTexture2DAsset ft(p_fileSystemModel, &clickedIndex);
        fileWidget = new InspectorTexture2DAssetWidget(ft);
    }
    else if(f.IsMeshFile())
    {
        FileMesh fm(p_fileSystemModel, &clickedIndex);
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
       File f(p_fileSystemModel, &index);
       if(f.GetName() != m_lastSelectedFileName)
       {
           m_lastSelectedFileName = f.GetName();
           RefreshInspector();
       }
    }
}

void Explorer::SetDir(const std::string &path)
{
    setRootIndex(p_fileSystemModel->setRootPath(QString::fromStdString(path)));
}

void Explorer::OnDirLoaded(QString dir)
{
    NONULL(WindowMain::GetInstance());

    if(GetCurrentDir().length() <= Persistence::GetAssetsPathAbsolute().length())
    {
        p_buttonDirUp->setEnabled(false);
        p_fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    }
    else
    {
        p_buttonDirUp->setEnabled(true);
        p_fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDot);
    }
}

std::string Explorer::GetCurrentDir() const
{
    return p_fileSystemModel->rootPath().toStdString();
}

void Explorer::updateGeometries()
{
    QListView::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}
