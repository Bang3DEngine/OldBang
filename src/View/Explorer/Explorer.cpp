#include "Bang/Explorer.h"

#include <QScrollBar>
#include "Bang/WinUndef.h"

#include "Bang/IO.h"
#include "Bang/Input.h"
#include "Bang/Debug.h"
#include "Bang/Prefab.h"
#include "Bang/TextFile.h"
#include "Bang/MeshFile.h"
#include "Bang/ImageFile.h"
#include "Bang/Hierarchy.h"
#include "Bang/GameObject.h"
#include "Bang/SystemUtils.h"
#include "Bang/SceneManager.h"
#include "Bang/EditorWindow.h"
#include "Bang/MeshAssetFile.h"
#include "Bang/FileSystemModel.h"
#include "Bang/QtProjectManager.h"
#include "Bang/MaterialAssetFile.h"
#include "Bang/Texture2DAssetFile.h"
#include "Bang/WindowEventManager.h"
#include "Bang/MeshFileInspectable.h"
#include "Bang/TextFileInspectable.h"
#include "Bang/ExplorerContextMenu.h"
#include "Bang/ImageFileInspectable.h"
#include "Bang/FileReferencesManager.h"
#include "Bang/MeshAssetFileInspectable.h"
#include "Bang/PrefabAssetFileInspectable.h"
#include "Bang/MaterialAssetFileInspectable.h"
#include "Bang/Texture2DAssetFileInspectable.h"

Explorer::Explorer(QWidget *parent)
    : m_renameShortcut    (this, KSeq("F2"),     SLOT(OnRenameClicked())),
      m_duplicateShortcut (this, KSeq("Ctrl+D"), SLOT(OnDuplicateClicked())),
      m_deleteShortcut    (this, KSeq("Del"),    SLOT(OnDeleteClicked())),
      m_eContextMenu(this)
{
    setAcceptDrops(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDropMode::DragDrop);
    setDragEnabled(true);

    setDropIndicatorShown(true);
    setDefaultDropAction(Qt::DropAction::MoveAction);
    setSelectionMode(QAbstractItemView::SingleSelection);

    m_fileRefsManager = new FileReferencesManager();

    m_fileSystemModel = new FileSystemModel(this);
    setModel(m_fileSystemModel);
    setWordWrap(true); // Needed to be able to wrap manually, dont know why
    setResizeMode(ResizeMode::Adjust);
    setTextElideMode(Qt::TextElideMode::ElideNone);

    EditorWindow *win = EditorWindow::GetInstance();
    m_buttonDirUp = win->buttonExplorerDirUp;
    m_buttonChangeViewMode = win->buttonExplorerChangeViewMode;

    m_labelCurrentPath = win->labelCurrentPath;

    connect(m_buttonDirUp, SIGNAL(clicked()),
            this, SLOT(OnButtonDirUpClicked()));
    connect(m_buttonChangeViewMode, SIGNAL(clicked()),
            this, SLOT(OnButtonChangeViewModeClicked()));
    connect(m_fileSystemModel, SIGNAL(directoryLoaded(QString)),
            this, SLOT(OnDirLoaded(QString)));
    connect(m_fileSystemModel, SIGNAL(fileRenamed(QString,QString,QString)),
            this, SLOT(OnFileRenamed(QString,QString,QString)) );
}

Explorer::~Explorer()
{
    delete m_fileSystemModel;
    delete m_fileRefsManager;
}

void Explorer::OnWindowShown()
{
    this->SetDragDropEventPossibleSources({
          Hierarchy::GetInstance(), this });

    EditorWindow *win = EditorWindow::GetInstance();

    m_labelFileName = win->labelFileName;

    QObject::connect(win->sliderExplorerIconSize, SIGNAL(sliderMoved(int)),
                     this, SLOT(OnIconSizeSliderValueChanged(int)));
    QObject::connect(win->sliderExplorerIconSize, SIGNAL(valueChanged(int)),
                     this, SLOT(OnIconSizeSliderValueChanged(int)));

    win->sliderExplorerIconSize->setValue(30);
    SetDir(IO::GetProjectAssetsRootAbs());
}

void Explorer::OnButtonDirUpClicked()
{
    String rootPath = GetCurrentDir();
    String parentDirPath = IO::GetDirUp(rootPath);
    SetDir(parentDirPath);
}

void Explorer::OnRenameClicked()
{
    String selectedPath = GetSelectedFileOrDirPath();
    if (!selectedPath.Empty())
    {
        StartRenaming(selectedPath);
    }
}

void Explorer::OnDeleteClicked()
{
    m_eContextMenu.OnDeleteClicked();
}

void Explorer::OnDuplicateClicked()
{
    m_eContextMenu.OnDuplicateClicked();
}

void Explorer::OnButtonChangeViewModeClicked()
{
    setViewMode(IsInListMode() ? ViewMode::IconMode : ViewMode::ListMode);
}

void Explorer::OnFileRenamed(const QString &dirPath,
                             const QString &oldName,
                             const QString &newName)
{
    UpdateLabelText();

    String newAbsPath = dirPath + "/" + newName;
    SelectFile(newAbsPath);

    String oldAbsPath = dirPath + "/" + oldName;
    m_fileRefsManager->OnFileOrDirNameAboutToBeChanged(oldAbsPath, newAbsPath);
}

void Explorer::UpdateLabelText()
{
    String absPath = GetSelectedFile().GetAbsolutePath();
    SetLabelText(absPath);
}

void Explorer::OnIconSizeSliderValueChanged(int value)
{
    const int iconSizeRange = (c_maxIconSize - c_minIconSize);
    const int iconSize = c_minIconSize + (float(value) / 100 * iconSizeRange);
    m_fileSystemModel->SetIconSize(iconSize);

    setGridSize(QSize(iconSize * 2.1f, iconSize + 50));
}

void Explorer::mousePressEvent(QMouseEvent *e)
{
    QListView::mousePressEvent(e);

    // Clear selection when pressing nowhere
    if (!indexAt(e->pos()).isValid())
    {
        clearSelection();
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
    if (e->button() == Qt::LeftButton)
    {
        ENSURE(selectedIndexes().length() > 0);
        String selectedPath = GetSelectedFileOrDirPath();
        if (IO::Exists(selectedPath))
        {
            if (IO::IsDir(selectedPath))
            {
                OnDirDoubleClicked(selectedPath);
            }
            else // File
            {
                OnFileDoubleClicked(selectedPath);
            }
        }
    }
    else
    {
        QListView::mouseDoubleClickEvent(e);
    }
}

void Explorer::currentChanged(const QModelIndex &current,
                              const QModelIndex &previous)
{
    QListView::currentChanged(current, previous);

    scrollTo(current);

    String selectedPath(m_fileSystemModel->filePath(current));

    if ( IO::IsFile(selectedPath) )
    {
        Hierarchy::GetInstance()->clearSelection();
    }

    SetLabelText(selectedPath);
    RefreshInspector();
}

void Explorer::OnDirDoubleClicked(const String &dirpath)
{
    SetDir(dirpath);
}

void Explorer::OnFileDoubleClicked(const String &filepath)
{
    File f(filepath);
    if (f.IsScene())
    {
        MenuBar::GetInstance()->OpenScene(f.GetAbsolutePath());
    }
    else if (f.IsBehaviour())
    {
        QtProjectManager::OpenBehaviourInQtCreator(filepath);
    }
}

void Explorer::RefreshInspector()
{
    if (selectedIndexes().empty()) return;

    QModelIndex selectedIndex = selectedIndexes().front();
    File f(m_fileSystemModel, selectedIndex);
    if (selectedIndex.isValid() && f.IsFile() &&
        IO::Exists(f.GetAbsolutePath()))
    {
        m_lastSelectedPath = f.GetRelativePath();

        Inspector *inspector = Inspector::GetInstance();
        if (m_lastInspectableInInspector)
        {
            inspector->Clear();
            // delete m_lastSerializableObjectInInspector;
            m_lastInspectableInInspector = nullptr;
        }

        SerializableObject *newInspectable = nullptr;
        File *specificFile = File::GetSpecificFile(f);
        if (specificFile)
        {
            newInspectable = specificFile->GetInspectable();
            delete specificFile;
        }

        if (!newInspectable && !f.IsDir())
        {
            inspector->Clear();
        }

        if (f.IsPrefabAsset()) // bprefab special case
        {
            File f(m_fileSystemModel, selectedIndex);
            PrefabAssetFileInspectable *prefabInspectable =
                    new PrefabAssetFileInspectable(f);
            newInspectable = prefabInspectable;
            prefabInspectable->ShowInInspector();
        }
        else
        {
            if (newInspectable)
            {
                inspector->SetInspectable(newInspectable,
                                          f.GetNameAndExtension());
            }
        }

        if (newInspectable)
        {
            m_lastInspectableInInspector = newInspectable;
        }
    }
}

void Explorer::SelectFile(const String &path)
{
    SetDir(IO::GetDir(path));

    QModelIndex ind = GetModelIndexFromFilepath(path);
    if (ind.isValid())
    {
        setCurrentIndex(ind);
        selectionModel()->select(ind, QItemSelectionModel::SelectionFlag::
                                      SelectCurrent);
        scrollTo(ind);
        RefreshInspector();
    }
}

Explorer *Explorer::GetInstance()
{
    EditorWindow *ew = EditorWindow::GetInstance();
    return ew ? ew->widgetListExplorer : nullptr;
}

String Explorer::GetFilepathFromModelIndex(const QModelIndex &qmi) const
{
    String f = m_fileSystemModel->fileInfo(qmi)
               .absoluteFilePath().toStdString();
    return f;
}

String Explorer::GetRelativeFilepathFromModelIndex(const QModelIndex &qmi) const
{
    String f = GetFilepathFromModelIndex(qmi);
    return IO::ToRelative(f);
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
    return IO::ToRelative(f);
}

QModelIndex Explorer::GetModelIndexFromFilepath(const String &filepath) const
{
    String absFilepath = IO::ToAbsolute(filepath, false);
    return m_fileSystemModel->index(absFilepath.ToQString());
}

void Explorer::SetDir(const String &path)
{
    if (!selectedIndexes().empty())
    {
        closePersistentEditor( selectedIndexes().front() );
        clearSelection();
    }

    String absDir = IO::ToAbsolute(path, false);
    setRootIndex(m_fileSystemModel->setRootPath(absDir.ToQString()));
    SetLabelText(absDir);
    clearSelection();
}

bool Explorer::IsEditing() const
{
    return state() == QAbstractItemView::EditingState;
}

bool Explorer::IsInListMode() const
{
    return viewMode() == ViewMode::ListMode;
}

void Explorer::SetLabelText(const String &absPath)
{
	if (!m_labelCurrentPath) { return; }
    String textDir = absPath.ElideLeft(65);
    m_labelCurrentPath->setText(textDir.ToQString());

    String fileNameExt = IO::GetFileNameWithExtension(absPath);
    m_labelFileName->setText( fileNameExt.ToQString() );
}

void Explorer::OnDirLoaded(QString dir)
{
    ENSURE(EditorWindow::GetInstance());

    if (GetCurrentDir().Length() <=
        IO::GetProjectAssetsRootAbs().Length())
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
    if (!selectedIndexes().empty())
    {
        return String(m_fileSystemModel->filePath(selectedIndexes().front()));
    }
    return "";
}

File Explorer::GetSelectedFile() const
{
    File f;
    if (!selectedIndexes().empty())
    {
        QModelIndex qmi = selectedIndexes().front();
        return File(m_fileSystemModel, qmi);
    }
    return f;
}

bool Explorer::IsSelectedAFile() const
{
    return !selectedIndexes().empty() &&
           !m_fileSystemModel->isDir(selectedIndexes().front());
}

bool Explorer::IsSelectedADir() const
{
    return !selectedIndexes().empty() &&
            m_fileSystemModel->isDir(selectedIndexes().front());
}

const QFileSystemModel *Explorer::GetFileSystemModel() const
{
    return m_fileSystemModel;
}

FileReferencesManager *Explorer::GetFileReferencesManager() const
{
    return m_fileRefsManager;
}

void Explorer::StartRenaming(const String &filepath)
{
    SelectFile(filepath);
    if (!selectedIndexes().empty())
    {
        QModelIndex selectedIndex = selectedIndexes().front();
        edit(selectedIndex);
    }
}


void Explorer::OnDragStart(const DragDropInfo &ddi)
{
    Hierarchy *hierarchy = Hierarchy::GetInstance();
    if (ddi.sourceObject == hierarchy)
    {
        setStyleSheet(IDragDropListener::acceptDragStyle);
    }

    m_fileBeingDragged = GetSelectedFileOrDirPath();
}

void Explorer::OnDragMove(const DragDropInfo &ddi)
{

    File destDir(m_fileSystemModel,
                 indexAt( viewport()->mapFromGlobal(QCursor::pos()) ));
    m_fileUnderMouse = destDir.GetAbsolutePath();
}

void Explorer::OnDrop(const DragDropInfo &ddi)
{
    setStyleSheet("/* */");

    if (ddi.sourceObject == this && underMouse()) // Handle file moving
    {
        String oldMovedFileOrDirPath = m_fileBeingDragged;
        String destDirPath = m_fileUnderMouse;

        if (!destDirPath.Empty())
        {
            String movedFileOrDirName =
                    IO::GetFileNameWithExtension(oldMovedFileOrDirPath);
            String newMovedFileOrDirPath = destDirPath + "/" + movedFileOrDirName;
            m_fileRefsManager->OnFileOrDirNameAboutToBeChanged(
                                                      oldMovedFileOrDirPath,
                                                      newMovedFileOrDirPath);
        }
    }

    if (ddi.currentObject == this)
    {
        Hierarchy *hierarchy = Hierarchy::GetInstance();
        if (ddi.sourceObject == hierarchy)
        {
            // Create a prefab of selected on the current directory
            GameObject *selected = hierarchy->GetFirstSelectedGameObject();
            ENSURE(selected);

            String path = GetCurrentDir() + "/" + selected->name;
            path = IO::AppendExtension(path,
                          Prefab::GetFileExtensionStatic());
            IO::WriteToFile(path, selected->GetSerializedString());
        }
    }

    m_fileBeingDragged = "";
}

void Explorer::dropEvent(QDropEvent *e)
{
    DragDropQListView::dropEvent(e);
    e->ignore(); // If we dont ignore, objects in the source list get removed
}

void Explorer::keyPressEvent(QKeyEvent *e)
{
    DragDropQListView::keyPressEvent(e);

    if (!IsEditing() &&
         (static_cast<Input::Key>(e->key()) == Input::Key::Space ||
          static_cast<Input::Key>(e->key()) == Input::Key::Return)
        )
    {
        String selectedPath = GetSelectedFileOrDirPath();
        if (IO::Exists(selectedPath))
        {
            if (IO::IsDir(selectedPath))
            {
                OnDirDoubleClicked(selectedPath);
            }
            else // File
            {
                OnFileDoubleClicked(selectedPath);
            }
        }
    }
}

void Explorer::updateGeometries()
{
    QListView::updateGeometries();
    verticalScrollBar()->setSingleStep(3);
}
