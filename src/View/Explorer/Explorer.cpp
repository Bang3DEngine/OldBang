#include "Explorer.h"

#include <QScrollBar>

#include "EditorWindow.h"
#include "Hierarchy.h"
#include "DragDropAgent.h"

Explorer::Explorer(QWidget *parent) : m_eContextMenu(this)
{
    setAcceptDrops(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
    setDragEnabled(true);

    setDropIndicatorShown(true);
    setDefaultDropAction(Qt::DropAction::MoveAction);
    setSelectionMode(QAbstractItemView::SingleSelection);

    m_fileSystemModel = new FileSystemModel();
    setModel(m_fileSystemModel);
    SetDir(Persistence::GetAssetsPathAbsolute());

    m_buttonDirUp = EditorWindow::GetInstance()->buttonExplorerDirUp;
    m_buttonChangeViewMode = EditorWindow::GetInstance()->buttonExplorerChangeViewMode;

    m_updateTimer = new QTimer(this); //Every X secs, update all the slots values
    m_updateTimer->start(100);

    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(Refresh()));

    connect(m_buttonDirUp, SIGNAL(clicked()), this, SLOT(OnButtonDirUpClicked()));
    connect(m_buttonChangeViewMode, SIGNAL(clicked()),
            this, SLOT(OnButtonChangeViewModeClicked()));

    connect(m_fileSystemModel, SIGNAL(directoryLoaded(QString)),
            this, SLOT(OnDirLoaded(QString)));
}

Explorer::~Explorer()
{
   delete m_fileSystemModel;
}

void Explorer::OnButtonDirUpClicked()
{
    String rootPath = GetCurrentDir();
    String parentDirPath = "";

    for (int i = rootPath.Length() - 1; i >= 0; --i)
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

void Explorer::mousePressEvent(QMouseEvent *e)
{
    QListView::mousePressEvent(e);

    if (e->button() == Qt::RightButton)
    {
        m_eContextMenu.OnCustomContextMenuRequested(e->pos());
    }
}

void Explorer::mouseReleaseEvent(QMouseEvent *e)
{
    QListView::mouseReleaseEvent(e);

    if (e->button() == Qt::LeftButton)
    {
        if (indexAt(e->pos()) == currentIndex())
        {
            if (IsSelectedAFile())
            {
                RefreshInspector();
            }
        }
    }
}

void Explorer::mouseDoubleClickEvent(QMouseEvent *e)
{
    QListView::mouseDoubleClickEvent(e);

    if (e->button() == Qt::LeftButton)
    {
        if (this->selectedIndexes().length() <= 0) return;

        QModelIndex clickedIndex = this->selectedIndexes().at(0);
        bool isDir = m_fileSystemModel->isDir(clickedIndex);
        if (isDir)
        {
            String clickedDirName =
                    m_fileSystemModel->fileName(clickedIndex).toStdString();
            SetDir(GetCurrentDir() + "/" + clickedDirName);
        }
        else
        {

        }
    }
}

void Explorer::RefreshInspector()
{
    if (selectedIndexes().size() <= 0) return;

    QModelIndex clickedIndex = selectedIndexes().at(0);
    File f(m_fileSystemModel, clickedIndex);
    m_lastSelectedPath = f.GetRelativePath();

    Inspector *inspector = Inspector::GetInstance();
    if (m_lastIInspectableInInspector)
    {
        inspector->Clear();
        delete m_lastIInspectableInInspector;
        m_lastIInspectableInInspector = nullptr;
    }

    IInspectable *newInspectable = nullptr;
    File *specificFile = File::GetSpecificFile(f);
    newInspectable = specificFile->GetInspectable();
    if (!newInspectable && !f.IsDir())
    {
        inspector->Clear();
    }

    if (f.IsPrefabAsset()) // bprefab special case
    {
        File f(m_fileSystemModel, clickedIndex);
        PrefabAssetFileInspectable *prefabInspectable =
                new PrefabAssetFileInspectable(f);
        newInspectable = prefabInspectable;
        prefabInspectable->ShowInInspector();
    }
    else
    {
        if (newInspectable)
        {
            inspector->SetInspectable(newInspectable, f.GetNameAndExtension());
        }
    }

    if (newInspectable)
    {
        m_lastIInspectableInInspector = newInspectable;
    }
}

void Explorer::SelectFile(const String &path)
{
    String absPath = Persistence::ToAbsolute(path);
    SetDir(Persistence::GetDir(absPath));

    QModelIndex ind = GetModelIndexFromFilepath(absPath);
    if (ind.isValid())
    {
        setCurrentIndex(ind);
        selectionModel()->select(ind, QItemSelectionModel::SelectionFlag::ClearAndSelect);
    }
    else
    {
    }

    RefreshInspector();
}

Explorer *Explorer::GetInstance()
{
    return EditorWindow::GetInstance()->widgetListExplorer;
}

void Explorer::Refresh()
{
    // If needed in a future
}

String Explorer::GetFilepathFromModelIndex(const QModelIndex &qmi) const
{
    String f = m_fileSystemModel->fileInfo(qmi).absoluteFilePath().toStdString();
    return f;
}

String Explorer::GetRelativeFilepathFromModelIndex(const QModelIndex &qmi) const
{
    String f = GetFilepathFromModelIndex(qmi);
    return Persistence::ToRelative(f);
}

String Explorer::GetDirFromModelIndex(const QModelIndex &qmi) const
{
    String f = m_fileSystemModel->fileInfo(qmi).absoluteDir()
                    .absolutePath().toStdString();
    return f;
}

String Explorer::GetRelativeDirFromModelIndex(const QModelIndex &qmi) const
{
    String f = GetDirFromModelIndex(qmi);
    return Persistence::ToRelative(f);
}

QModelIndex Explorer::GetModelIndexFromFilepath(const String &filepath) const
{
    String absFilepath = Persistence::ToAbsolute(filepath);
    return m_fileSystemModel->index(QString::fromStdString(absFilepath));
}

void Explorer::SetDir(const String &path)
{
    String absDir = Persistence::ToAbsolute(path);
    setRootIndex(m_fileSystemModel->setRootPath(QString::fromStdString(absDir)));
}

void Explorer::OnDirLoaded(QString dir)
{
    NONULL(EditorWindow::GetInstance());

    if (GetCurrentDir().Length() <= Persistence::GetAssetsPathAbsolute().Length())
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

String Explorer::GetCurrentDir() const
{
    return m_fileSystemModel->rootPath().toStdString();
}

String Explorer::GetSelectedFileOrDirPath() const
{
    if (!currentIndex().isValid()) return "";
    return Persistence::ToRelative(
                m_fileSystemModel->filePath(currentIndex()).toStdString());
}

File Explorer::GetSelectedFile() const
{
    File f;
    if (!currentIndex().isValid()) return f;
    QModelIndex qmi = currentIndex();
    return File(m_fileSystemModel, qmi);
}

bool Explorer::Exists(const String &filepath) const
{
    return GetModelIndexFromFilepath(filepath).isValid();
}

bool Explorer::IsSelectedAFile() const
{
    if (!currentIndex().isValid()) return false;
    return !m_fileSystemModel->isDir(currentIndex());
}

bool Explorer::IsSelectedADir() const
{
    if (!currentIndex().isValid()) return false;
    return m_fileSystemModel->isDir(currentIndex());
}

const QFileSystemModel *Explorer::GetFileSystemModel() const
{
    return m_fileSystemModel;
}

void Explorer::StartRenaming(const String &filepath)
{
    SelectFile(filepath);
    edit(currentIndex());
}


void Explorer::OnDragStart(const DragDropInfo &ddi)
{
    Hierarchy *hierarchy = Hierarchy::GetInstance();
    if (ddi.sourceObject == hierarchy)
    {
        setStyleSheet(IDragDropListener::acceptDragStyle);
    }
}

void Explorer::OnDropHere(const DragDropInfo &ddi)
{
    Hierarchy *hierarchy = Hierarchy::GetInstance();
    if (ddi.sourceObject == hierarchy)
    {
        // Create a prefab of selected on the current directory
        GameObject *selected = hierarchy->GetFirstSelectedGameObject();
        NONULL(selected);

        String path = GetCurrentDir() + "/";
        String gameObjectName = selected->name;
        path += gameObjectName;
        path = Persistence::AppendExtension(path,
                      Prefab::GetFileExtensionStatic());
        FileWriter::WriteToFile(path, selected);
    }
}

void Explorer::OnDrop(const DragDropInfo &ddi)
{
    setStyleSheet("/* */");
}

void Explorer::dropEvent(QDropEvent *e)
{
    DragDropQListView::dropEvent(e);
    e->ignore(); // If we dont ignore, objects in the source list get removed
}

void Explorer::updateGeometries()
{
    QListView::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}

///////////////////////////////////////////////////////////////////////

FileSystemModel::FileSystemModel()
{
    setFilter(QDir::AllEntries | QDir::NoDot);
    setReadOnly(false);
}

Qt::DropActions FileSystemModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

QVariant FileSystemModel::data(const QModelIndex &index,
                               int role) const
{
    if (role == Qt::DecorationRole)
    {
        File file(this, index);
        File *f = File::GetSpecificFile(file);
        if (f)
        {
            const QPixmap &pm = f->GetIcon();
            Qt::TransformationMode transMode =
                    Qt::TransformationMode::SmoothTransformation;
            if (f->IsTexture2DAsset() || f->IsImageFile())
            {
                transMode = Qt::TransformationMode::FastTransformation;
            }

            return pm.scaled(32, 32, Qt::IgnoreAspectRatio, transMode);
        }
    }
    return QFileSystemModel::data(index, role);
}
