#ifndef EXPLORER_H
#define EXPLORER_H

#include <string>

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
#include "MaterialAssetFile.h"
#include "Texture2DAssetFile.h"
#include "TextFileInspectable.h"
#include "MeshAssetFileInspectable.h"
#include "PrefabAssetFileInspectable.h"
#include "MaterialAssetFileInspectable.h"

#include "MeshFileInspectable.h"
#include "ImageFileInspectable.h"
#include "Texture2DAssetFileInspectable.h"

#include "IDroppableWidget.h"

class FileSystemModel;
class Explorer : public IDroppableQListView
{
    Q_OBJECT

    friend class FileSystemModel;
    friend class ExplorerDirTree;

private:

    FileSystemModel *m_fileSystemModel = nullptr;
    QToolButton *m_buttonDirUp = nullptr;
    QToolButton *m_buttonChangeViewMode = nullptr;
    IInspectable *lastIInspectableInInspector = nullptr;

    std::string m_lastSelectedPath = "";
    QTimer *m_updateTimer = nullptr;

    std::string GetFilepathFromModelIndex(const QModelIndex &qmi);
    std::string GetRelativeFilepathFromModelIndex(const QModelIndex &qmi);
    std::string GetDirFromModelIndex(const QModelIndex &qmi);
    std::string GetRelativeDirFromModelIndex(const QModelIndex &qmi);
    QModelIndex GetModelIndexFromFilepath(const std::string &filepath);
    void SetDir(const std::string &path);

protected:
    //To set the scroll step to a smaller one
    virtual void updateGeometries() override;

public:
    Explorer(QWidget *parent);
    virtual ~Explorer();

    std::string GetCurrentDir() const;
    std::string GetSelectedFileOrDirPath() const;
    File GetSelectedFile() const;
    bool IsSelectedAFile() const;
    bool IsSelectedADir() const;

    void StartRenaming(const std::string &filepath);

    void dropEvent(QDropEvent *e) override;

    void OnDropFromHierarchy(GameObject *go,  QDropEvent *e) override;

    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;

    //Updates the Inspector with the selected file info
    void RefreshInspector();

    void SelectFile(const std::string &path);

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
};

#endif // EXPLORER_H













