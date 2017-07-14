#include "Bang/Explorer.h"

#include <QScrollBar>
#include "Bang/WinUndef.h"

#include "Bang/Paths.h"
#include "Bang/Input.h"
#include "Bang/Debug.h"
#include "Bang/Prefab.h"
#include "Bang/TextFile.h"
#include "Bang/MeshFile.h"
#include "Bang/ImageFile.h"
#include "Bang/Hierarchy.h"
#include "Bang/GameObject.h"
#include "Bang/FileTracker.h"
#include "Bang/SystemUtils.h"
#include "Bang/SceneManager.h"
#include "Bang/EditorWindow.h"
#include "Bang/MaterialFile.h"
#include "Bang/Texture2DFile.h"
#include "Bang/FileSystemModel.h"
#include "Bang/QtProjectManager.h"
#include "Bang/WindowEventManager.h"
#include "Bang/ExplorerContextMenu.h"
#include "Bang/FileReferencesManager.h"

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
    SetDir(Paths::ProjectAssets());
}

void Explorer::OnButtonDirUpClicked()
{
    SetDir( GetCurrentDir().GetDirectory() );
}

void Explorer::OnRenameClicked()
{
    Path selectedPath = GetSelectedPath();
    if (!selectedPath.IsEmpty())
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

    Path newPath(dirPath + "/" + newName);
    SelectPath(newPath);

    Path oldPath(dirPath + "/" + oldName);
    m_fileRefsManager->OnFileOrDirNameMoved(oldPath, newPath);
}

void Explorer::UpdateLabelText()
{
    String absPath = GetSelectedFile().GetPath().GetAbsolute();
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
        Path selectedPath = GetSelectedPath();
        if (selectedPath.Exists())
        {
            if (selectedPath.IsDir())
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

    if ( Path(selectedPath).IsFile() )
    {
        Hierarchy::GetInstance()->clearSelection();
    }

    SetLabelText(selectedPath);
    RefreshInspector();
}

void Explorer::OnDirDoubleClicked(const Path &dirpath)
{
    SetDir(dirpath);
}

void Explorer::OnFileDoubleClicked(const Path &filepath)
{
    BFile f(filepath);
    if (f.IsScene())
    {
        MenuBar::GetInstance()->OpenScene(f.GetPath());
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
    BFile f(m_fileSystemModel, selectedIndex);
    if (selectedIndex.isValid() && f.GetPath().IsFile() && f.GetPath().Exists())
    {
        Inspector::GetInstance()->Clear();

        BFile *specificFile = BFile::GetSpecificFile(f);
        Inspector::GetInstance()->Show(specificFile, true);
        delete specificFile;
    }
}

void Explorer::SelectPath(const Path &path)
{
    SetDir(path.GetDirectory());

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

QModelIndex Explorer::GetModelIndexFromFilepath(const Path &filepath) const
{
    return m_fileSystemModel->index(filepath.GetAbsolute().ToQString());
}

void Explorer::SetDir(const Path &dirPath)
{
    if (!selectedIndexes().empty())
    {
        closePersistentEditor( selectedIndexes().front() );
        clearSelection();
    }

    String dir = dirPath.GetAbsolute();
    setRootIndex(m_fileSystemModel->setRootPath(dir.ToQString()));
    SetLabelText(dir);
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

void Explorer::SetLabelText(const String &text)
{
	if (!m_labelCurrentPath) { return; }
    String textDir = text.ElideLeft(65);
    m_labelCurrentPath->setText(textDir.ToQString());

    String fileNameExt = Path(text).GetNameExt();
    m_labelFileName->setText( fileNameExt.ToQString() );
}

void Explorer::OnDirLoaded(QString dir)
{
    ENSURE(EditorWindow::GetInstance());

    if (GetCurrentDir().GetAbsolute().Length() <=
        Paths::ProjectAssets().GetAbsolute().Length())
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

Path Explorer::GetCurrentDir() const
{
    return Path( String(m_fileSystemModel->rootPath()) );
}

Path Explorer::GetSelectedPath() const
{
    if (!selectedIndexes().empty())
    {
        return Path(m_fileSystemModel->filePath(selectedIndexes().front()));
    }
    return Path();
}

BFile Explorer::GetSelectedFile() const
{
    BFile f;
    if (!selectedIndexes().empty())
    {
        QModelIndex qmi = selectedIndexes().front();
        return BFile(m_fileSystemModel, qmi);
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

void Explorer::StartRenaming(const Path &filepath)
{
    SelectPath(filepath);
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
        // setStyleSheet(IDragDropListener::acceptDragStyle);
    }
}

void Explorer::OnDrop(const DragDropInfo &ddi)
{
    setStyleSheet("/* */");

    QTimer::singleShot(300, FileReferencesManager::GetInstance(),
                       SLOT(CheckForMovedFiles()));

    if (ddi.currentObject == this)
    {
        Hierarchy *hierarchy = Hierarchy::GetInstance();
        if (ddi.sourceObject == hierarchy)
        {
            // Create a prefab of selected on the current directory
            GameObject *selected = hierarchy->GetFirstSelectedGameObject();
            ENSURE(selected);

            Path path = GetCurrentDir().Append(selected->name);
            path = path.AppendExtension(Prefab::GetFileExtensionStatic());
            File::Write(path, selected->GetSerializedString());
        }
    }
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
        Path selectedPath = GetSelectedPath();
        if (selectedPath.Exists())
        {
            if (selectedPath.IsDir())
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
