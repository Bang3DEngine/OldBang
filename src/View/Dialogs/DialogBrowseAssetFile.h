#ifndef DIALOGBROWSEASSETFILE_H
#define DIALOGBROWSEASSETFILE_H

#include <QLabel>
#include <QTimer>
#include <QDialog>
#include <QModelIndex>
#include "Bang/WinUndef.h"

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/String.h"

class QTabWidget;
class QListWidget;
class QListWidgetItem;
class DialogBrowseAssetFile : public QDialog
{
    Q_OBJECT

public:
    DialogBrowseAssetFile(String *resultFile);

    void Show(QWidget *parent = nullptr,
              const String &assetNameForTitle = "Asset",
              const List<String> &extensions = {});

private:
    String *m_resultFile = nullptr;
    Map<QListWidgetItem*, String> m_item_To_absoluteFilepath;

    const int c_iconSize           = 40;
    QTabWidget *m_tabWidget        = nullptr;
    QListWidget *m_projectFileList = nullptr;
    QListWidget *m_engineFileList  = nullptr;
    QPushButton *m_openButton      = nullptr;
    QLabel *m_botFilepathLabel     = nullptr;

    void ShowOnList(QListWidget *listWidget,
                    bool engineFilepaths,
                    const List<String> &filepaths);

    QListWidget *CreateList() const;

private slots:
    void OnSelectionChange(int newIndex);
    void OnFileAccepted();
    void OnFileAccepted(QModelIndex modelIndex);
};

#endif // DIALOGBROWSEASSETFILE_H
