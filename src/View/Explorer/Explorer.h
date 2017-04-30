#ifndef EXPLORER_H
#define EXPLORER_H

#include <QLabel>
#include <QListView>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QToolButton>
#include "Bang/WinUndef.h"

#include "Bang/Path.h"
#include "Bang/File.h"
#include "Bang/String.h"
#include "Bang/Shortcut.h"
#include "Bang/DragDropQListView.h"
#include "Bang/IDragDropListener.h"
#include "Bang/SerializableObject.h"
#include "Bang/ExplorerContextMenu.h"
#include "Bang/IWindowEventManagerListener.h"

class SerializableObject;
class FileSystemModel;
class FileReferencesManager;
class Explorer : public DragDropQListView,
                 public IDragDropListener,
                 public IWindowEventManagerListener
{
    Q_OBJECT

public:
    Explorer(QWidget *parent);
    virtual ~Explorer();

    void OnWindowShown();

    String GetCurrentDir() const;
    Path GetSelectedFileOrDirPath() const;
    File GetSelectedFile() const;
    bool IsSelectedAFile() const;
    bool IsSelectedADir() const;
    const QFileSystemModel* GetFileSystemModel() const;
    FileReferencesManager *GetFileReferencesManager() const;

    void StartRenaming(const Path &filepath);

    virtual void OnDragStart(const DragDropInfo &ddi) override;
    virtual void OnDrop(const DragDropInfo &ddi) override;

    void dropEvent(QDropEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void currentChanged(const QModelIndex &current,
                        const QModelIndex &previous) override;

    void OnDirDoubleClicked(const Path &dirpath);
    void OnFileDoubleClicked(const Path &filepath);

    //Updates the Inspector with the selected file info
    void RefreshInspector();

    void SelectPath(const Path &path);

    static Explorer* GetInstance();
    void SetDir(const Path &path);

    bool IsEditing() const;
    bool IsInListMode() const;

    void SetLabelText(const String &text);

public slots:
    void OnDirLoaded(QString dir);
    void OnButtonDirUpClicked();
    void OnRenameClicked();
    void OnDeleteClicked();
    void OnDuplicateClicked();
    void OnButtonChangeViewModeClicked();
    void OnFileRenamed(const QString &dirPath,
                       const QString &oldName,
                       const QString &newName);
    void UpdateLabelText();
    void OnIconSizeSliderValueChanged(int value);

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

private:
    LocalShortcut m_renameShortcut, m_duplicateShortcut, m_deleteShortcut;

    FileReferencesManager *m_fileRefsManager = nullptr;

    const int c_minIconSize = 20;
    const int c_maxIconSize = 100;

    ExplorerContextMenu m_eContextMenu;

    FileSystemModel *m_fileSystemModel               = nullptr;
    QToolButton *m_buttonDirUp                       = nullptr;
    QToolButton *m_buttonChangeViewMode              = nullptr;

    QLabel *m_labelCurrentPath = nullptr;
    QLabel *m_labelFileName    = nullptr;

    QModelIndex GetModelIndexFromFilepath(const Path &filepath) const;

    friend class File;
    friend class SerializableObject;
    friend class FileSystemModel;
    friend class ExplorerDirTree;
    friend class ExplorerContextMenu;
    friend class FileReferencesManager;
};

#endif // EXPLORER_H













