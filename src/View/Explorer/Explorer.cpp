#include "Explorer.h"

#include <QScrollBar>

#include "WindowMain.h"
#include "Hierarchy.h"
#include "DragDropAgent.h"

Explorer::Explorer(QWidget *parent)
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

    m_buttonDirUp = WindowMain::GetInstance()->buttonExplorerDirUp;
    m_buttonChangeViewMode = WindowMain::GetInstance()->buttonExplorerChangeViewMode;

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

void Explorer::mousePressEvent(QMouseEvent *e)
{
    QListView::mousePressEvent(e);

    if (e->button() == Qt::RightButton)
    {
        OnCustomContextMenuRequested(e->pos());
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
            std::string clickedDirName =
                    m_fileSystemModel->fileName(clickedIndex).toStdString();
            SetDir(GetCurrentDir() + "/" + clickedDirName);
        }
        else
        {

        }
    }
}

void Explorer::OnCustomContextMenuRequested(QPoint point)
{
    QMenu contextMenu(tr("Explorer context menu"), this);

    QAction actionDuplicate("Duplicate", this);
    QAction actionDelete("Delete", this);

    connect(&actionDuplicate, SIGNAL(triggered()), this, SLOT(OnContextMenuDuplicateClicked()));
    connect(&actionDelete, SIGNAL(triggered()), this, SLOT(OnContextMenuDeleteClicked()));

    if (IsSelectedAFile())
    {
        contextMenu.addAction(&actionDuplicate);
        contextMenu.addAction(&actionDelete);
    }
    else if (IsSelectedADir())
    {
        // contextMenu.addAction(&actionDuplicate);
        contextMenu.addAction(&actionDelete);
    }

    contextMenu.exec(mapToGlobal(point));
}

void Explorer::OnContextMenuDuplicateClicked()
{
    File source = GetSelectedFile();
    std::string filepath = source.GetRelativePath();
    while (Exists(filepath))
    {
        filepath = Persistence::GetNextDuplicateName(filepath);
    }

    FileWriter::WriteToFile(filepath, source.GetContents());
}

void Explorer::OnContextMenuDeleteClicked()
{
    Inspector *inspector = Inspector::GetInstance();
    if (inspector->IsShowingInspectable(m_lastIInspectableInInspector))
    {
        inspector->Clear();
        delete m_lastIInspectableInInspector;
        m_lastIInspectableInInspector = nullptr;
    }
    m_fileSystemModel->remove(currentIndex());
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

void Explorer::SelectFile(const std::string &path)
{
    std::string absPath = Persistence::ToAbsolute(path);
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
    return WindowMain::GetInstance()->widgetListExplorer;
}

void Explorer::Refresh()
{
    // If needed in a future
}

std::string Explorer::GetFilepathFromModelIndex(const QModelIndex &qmi) const
{
    std::string f = m_fileSystemModel->fileInfo(qmi).absoluteFilePath().toStdString();
    return f;
}

std::string Explorer::GetRelativeFilepathFromModelIndex(const QModelIndex &qmi) const
{
    std::string f = GetFilepathFromModelIndex(qmi);
    return Persistence::ToRelative(f);
}

std::string Explorer::GetDirFromModelIndex(const QModelIndex &qmi) const
{
    std::string f = m_fileSystemModel->fileInfo(qmi).absoluteDir()
                    .absolutePath().toStdString();
    return f;
}

std::string Explorer::GetRelativeDirFromModelIndex(const QModelIndex &qmi) const
{
    std::string f = GetDirFromModelIndex(qmi);
    return Persistence::ToRelative(f);
}

QModelIndex Explorer::GetModelIndexFromFilepath(const std::string &filepath) const
{
    std::string absFilepath = Persistence::ToAbsolute(filepath);
    return m_fileSystemModel->index(QString::fromStdString(absFilepath));
}

void Explorer::SetDir(const std::string &path)
{
    std::string absDir = Persistence::ToAbsolute(path);
    setRootIndex(m_fileSystemModel->setRootPath(QString::fromStdString(absDir)));
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

std::string Explorer::GetSelectedFileOrDirPath() const
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

bool Explorer::Exists(const std::string &filepath) const
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

void Explorer::StartRenaming(const std::string &filepath)
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

        std::string path = GetCurrentDir() + "/";
        std::string gameObjectName = selected->name;
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

QVariant FileSystemModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DecorationRole)
    {
        File file(this, index);
        File *f = File::GetSpecificFile(file);
        if (f)
        {
            QPixmap pm = f->GetIcon();
            return pm.scaled(32, 32, Qt::IgnoreAspectRatio, Qt::TransformationMode::SmoothTransformation);
        }
    }
    return QFileSystemModel::data(index, role);
}
