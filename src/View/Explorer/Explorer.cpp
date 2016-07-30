#include "Explorer.h"

#include <QScrollBar>

#include "WindowMain.h"

Explorer::Explorer(QWidget *parent) : QListView(parent)
{
    m_fileSystemModel = new QFileSystemModel();

    m_buttonDirUp = WindowMain::GetInstance()->buttonExplorerDirUp;
    m_buttonChangeViewMode =
            WindowMain::GetInstance()->buttonExplorerChangeViewMode;

    connect(m_buttonDirUp, SIGNAL(clicked()), this, SLOT(OnButtonDirUpClicked()));
    connect(m_buttonChangeViewMode, SIGNAL(clicked()),
            this, SLOT(OnButtonChangeViewModeClicked()));

    setModel(m_fileSystemModel);
    m_fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDot);
    m_fileSystemModel->setReadOnly(false);

    connect(m_fileSystemModel, SIGNAL(directoryLoaded(QString)),
            this, SLOT(OnDirLoaded(QString)));
    SetDir(Persistence::GetAssetsPathAbsolute());

    m_updateTimer = new QTimer(this); //Every X secs, update all the slots values
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(Refresh()));
    m_updateTimer->start(100);
}

Explorer::~Explorer()
{
   delete m_fileSystemModel;
}

void Explorer::OnButtonDirUpClicked()
{
    std::string rootPath = GetCurrentDir();
    std::string parentDirPath = "";

    for (int i = rootPath.length() - 1; i >= 0; --i)
    {
        if (rootPath[i] == '/')
        {
            parentDirPath = rootPath.substr(0, i);
            break;
        }
    }

    SetDir(parentDirPath);
}

void Explorer::OnButtonChangeViewModeClicked()
{
    if (viewMode() == ViewMode::ListMode)
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
    if (e->button() == Qt::LeftButton)
    {
        RefreshInspector();
    }
}

void Explorer::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        if (this->selectedIndexes().length() <= 0) return;

        QModelIndex clickedIndex = this->selectedIndexes().at(0);
        bool isDir = m_fileSystemModel->isDir(clickedIndex);
        if (isDir)
        {
            std::string clickedDirName =
                    m_fileSystemModel->fileName(clickedIndex).toStdString();
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
    if (this->selectedIndexes().size() <= 0) return;

    QModelIndex clickedIndex = this->selectedIndexes().at(0);
    File f(m_fileSystemModel, &clickedIndex);

    InspectorWidget *fileWidget = nullptr;
    if (f.IsImageFile())
    {
        FileImage fi(m_fileSystemModel, &clickedIndex);
        fileWidget = new InspectorImageFileWidget(fi);
    }
    else if (f.IsTexture2DAsset())
    {
        FileTexture2DAsset ft(m_fileSystemModel, &clickedIndex);
        fileWidget = new InspectorTexture2DAssetWidget(ft);
    }
    else if (f.IsMeshFile())
    {
        FileMesh fm(m_fileSystemModel, &clickedIndex);
        WindowMain::GetInstance()->widgetInspector->SetInspectable(new InspectorMeshFileWidget(fm));
        //fileWidget = new InspectorMeshFileWidget(fm);
    }
    /*else if (f.IsMeshAsset())
    {
        FileMeshAsset ft(fileSystemModel, &clickedIndex);
        fileWidget = new InspectorMeshFileWidget(ft);
    }*/
    else
    {
        WindowMain::GetInstance()->widgetInspector->Clear();
    }

    if (fileWidget)
    {
        //WindowMain::GetInstance()->widgetInspector->AddWidget(fileWidget);
    }
}

void Explorer::Refresh()
{
    if (selectedIndexes().length() > 0)
    {
       QModelIndex index = selectedIndexes().at(0);
       File f(m_fileSystemModel, &index);
       if (f.GetName() != m_lastSelectedFileName)
       {
           m_lastSelectedFileName = f.GetName();
           RefreshInspector();
       }
    }
}

void Explorer::SetDir(const std::string &path)
{
    setRootIndex(m_fileSystemModel->setRootPath(QString::fromStdString(path)));
}

void Explorer::OnDirLoaded(QString dir)
{
    NONULL(WindowMain::GetInstance());

    if (GetCurrentDir().length() <= Persistence::GetAssetsPathAbsolute().length())
    {
        m_buttonDirUp->setEnabled(false);
        m_fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    }
    else
    {
        m_buttonDirUp->setEnabled(true);
        m_fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDot);
    }
}

std::string Explorer::GetCurrentDir() const
{
    return m_fileSystemModel->rootPath().toStdString();
}

void Explorer::updateGeometries()
{
    QListView::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}
