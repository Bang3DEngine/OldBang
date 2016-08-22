#ifndef EXPLORER_H
#define EXPLORER_H



#include <QListView>
#include <QMouseEvent>
#include <QToolButton>
#include <QFileSystemModel>

#include "Bang.h"

#include "Logger.h"

#include "Persistence.h"

#include "File.h"
#include "TextFile.h"
#include "MeshFile.h"
#include "ImageFile.h"
#include "MeshAssetFile.h"
#include "DragDropAgent.h"
#include "IDragDropListener.h"
#include "MaterialAssetFile.h"
#include "Texture2DAssetFile.h"
#include "MeshFileInspectable.h"
#include "TextFileInspectable.h"
#include "ExplorerContextMenu.h"
#include "ImageFileInspectable.h"
#include "MeshAssetFileInspectable.h"
#include "PrefabAssetFileInspectable.h"
#include "MaterialAssetFileInspectable.h"
#include "Texture2DAssetFileInspectable.h"


class FileSystemModel;
class Explorer : public DragDropQListView,
                 public IDragDropListener,
                 public IWindowEventManagerListener
{
    Q_OBJECT

friend class File;
friend class FileSystemModel;
friend class ExplorerDirTree;
friend class ExplorerContextMenu;

private:

    ExplorerContextMenu m_eContextMenu;

    FileSystemModel *m_fileSystemModel = nullptr;
    QToolButton *m_buttonDirUp = nullptr;
    QToolButton *m_buttonChangeViewMode = nullptr;
    IInspectable *m_lastIInspectableInInspector = nullptr;

    String m_lastSelectedPath = "";
    QTimer *m_updateTimer = nullptr;

    String GetFilepathFromModelIndex(const QModelIndex &qmi) const;
    String GetRelativeFilepathFromModelIndex(const QModelIndex &qmi) const;
    String GetDirFromModelIndex(const QModelIndex &qmi) const;
    String GetRelativeDirFromModelIndex(const QModelIndex &qmi) const;
    QModelIndex GetModelIndexFromFilepath(const String &filepath) const;
    void SetDir(const String &path);

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

public:
    Explorer(QWidget *parent);
    virtual ~Explorer();

    String GetCurrentDir() const;
    String GetSelectedFileOrDirPath() const;
    File GetSelectedFile() const;
    bool Exists(const String &filepath) const;
    bool IsSelectedAFile() const;
    bool IsSelectedADir() const;
    const QFileSystemModel* GetFileSystemModel() const;

    void StartRenaming(const String &filepath);

    virtual void OnDragStart(const DragDropInfo &ddi) override;
    virtual void OnDropHere(const DragDropInfo &ddi) override;
    virtual void OnDrop(const DragDropInfo &ddi) override;

    void dropEvent(QDropEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;

public:
    //Updates the Inspector with the selected file info
    void RefreshInspector();

    void SelectFile(const String &path);

    static Explorer* GetInstance();

public slots:
    void Refresh();

    void OnDirLoaded(QString dir);
    void OnButtonDirUpClicked();
    void OnButtonChangeViewModeClicked();
};

class FileSystemModel : public QFileSystemModel
{
public:
    FileSystemModel();

protected:
    Qt::DropActions supportedDropActions() const override;

    QVariant data(const QModelIndex& index, int role) const;
};

#endif // EXPLORER_H













