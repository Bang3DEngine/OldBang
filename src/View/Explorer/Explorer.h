#ifndef EXPLORER_H
#define EXPLORER_H

#include <QListView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QToolButton>

#include "File.h"
#include "String.h"
#include "DragDropQListView.h"
#include "IDragDropListener.h"
#include "IShortcutListener.h"
#include "ExplorerContextMenu.h"
#include "IWindowEventManagerListener.h"

class IFileable;
class FileSystemModel;
class FileReferencesManager;
class Explorer : public DragDropQListView,
                 public IDragDropListener,
                 public IShortcutListener,
                 public IWindowEventManagerListener
{
    Q_OBJECT

public:
    Explorer(QWidget *parent);
    virtual ~Explorer();

    void OnWindowShown();

    String GetCurrentDir() const;
    String GetSelectedFileOrDirPath() const;
    File GetSelectedFile() const;
    bool IsSelectedAFile() const;
    bool IsSelectedADir() const;
    const QFileSystemModel* GetFileSystemModel() const;
    FileReferencesManager *GetFileReferencesManager() const;

    void StartRenaming(const String &filepath);

    virtual void OnDragStart(const DragDropInfo &ddi) override;
    virtual void OnDragMove(const DragDropInfo &ddi) override;
    virtual void OnDrop(const DragDropInfo &ddi) override;

    void dropEvent(QDropEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void currentChanged(const QModelIndex &current,
                        const QModelIndex &previous) override;

    void OnShortcutPressed() override;
    void OnDirDoubleClicked(const String &dirpath);
    void OnFileDoubleClicked(const String &filepath);

    //Updates the Inspector with the selected file info
    void RefreshInspector();

    void SelectFile(const String &path);

    static Explorer* GetInstance();
    void SetDir(const String &path);

    bool IsEditing() const;
    bool IsInListMode() const;

    void SetLabelText(const String &absPath);

public slots:

    void OnDirLoaded(QString dir);
    void OnButtonDirUpClicked();
    void OnButtonChangeViewModeClicked();
    void OnFileRenamed(const QString &path,
                       const QString &oldName,
                       const QString &newName);
    void UpdateLabelText();
    void OnIconSizeSliderValueChanged(int value);

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

private:
    FileReferencesManager *m_fileRefsManager = nullptr;

    const int c_minIconSize = 20;
    const int c_maxIconSize = 100;

    ExplorerContextMenu m_eContextMenu;

    String m_fileBeingDragged = "";
    String m_fileUnderMouse = "";

    FileSystemModel *m_fileSystemModel = nullptr;
    QToolButton *m_buttonDirUp = nullptr;
    QToolButton *m_buttonChangeViewMode = nullptr;
    IInspectable *m_lastIInspectableInInspector = nullptr;

    String m_lastSelectedPath = "";
    QLabel *m_labelCurrentPath = nullptr;
    QLabel *m_labelFileName    = nullptr;

    String GetFilepathFromModelIndex(const QModelIndex &qmi) const;
    String GetRelativeFilepathFromModelIndex(const QModelIndex &qmi) const;
    String GetDirFromModelIndex(const QModelIndex &qmi) const;
    String GetRelativeDirFromModelIndex(const QModelIndex &qmi) const;
    QModelIndex GetModelIndexFromFilepath(const String &filepath) const;

    friend class File;
    friend class IFileable;
    friend class FileSystemModel;
    friend class ExplorerDirTree;
    friend class ExplorerContextMenu;
    friend class FileReferencesManager;
};

#endif // EXPLORER_H













