#include "Bang/DialogBrowseAssetFile.h"

#include <QTabWidget>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "Bang/WinUndef.h"

#include "Bang/IO.h"
#include "Bang/File.h"
#include "Bang/Color.h"
#include "Bang/Debug.h"
#include "Bang/Object.h"
#include "Bang/IconManager.h"

DialogBrowseAssetFile::DialogBrowseAssetFile(String *resultFile)
{
    QVBoxLayout *vLayout = new QVBoxLayout();

    m_projectFileList = CreateList();
    m_engineFileList  = CreateList();

    m_tabWidget= new QTabWidget();
    m_tabWidget->addTab(m_projectFileList, "Project");
    m_tabWidget->addTab(m_engineFileList,  "Engine");
    m_tabWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    vLayout->addWidget(m_tabWidget, 9999);

    QHBoxLayout *botHLayout = new QHBoxLayout();

    m_botFilepathLabel = new QLabel();
    m_botFilepathLabel->setText("");
    botHLayout->addWidget(m_botFilepathLabel);

    m_openButton = new QPushButton();
    m_openButton->setText("Open");
    m_openButton->setEnabled(false);
    QObject::connect(m_openButton, SIGNAL(pressed()),
                     this, SLOT(OnFileAccepted()));
    botHLayout->addStretch(999);
    botHLayout->addWidget(m_openButton);

    vLayout->addLayout(botHLayout);
    setLayout(vLayout);

    m_resultFile = resultFile;
    resize(400, 400);
}

void DialogBrowseAssetFile::Show(QWidget *parent,
                                 const String &assetNameForTitle,
                                 const List<String> &extensions)
{
    setModal(parent != nullptr);
    String title = "Browse Asset ( " + String::Join(extensions, ", ") + " )";
    setWindowTitle(title.ToQString());

    List<String> projectFilepaths =
            IO::GetFiles(IO::GetProjectAssetsRootAbs(),
                                  true, extensions);
    List<String> engineFilepaths =
            IO::GetFiles(IO::GetEngineAssetsRootAbs(),
                                  true, extensions);

    engineFilepaths.PushFront("None");
    projectFilepaths.PushFront("None");
    ShowOnList(m_engineFileList,  true,  engineFilepaths);
    ShowOnList(m_projectFileList, false, projectFilepaths);
    show();
}

void DialogBrowseAssetFile::ShowOnList(QListWidget *listWidget,
                                       bool engineFilepaths,
                                       const List<String> &filepaths)
{
    for (const String &filepath : filepaths)
    {
        String fileName = IO::GetFileName(filepath);

        String absFilepath = IO::ToAbsolute(filepath, engineFilepaths);
        File file(absFilepath);

        QPixmap filePixmap;
        File *sFile = File::GetSpecificFile(file);
        if (sFile)
        {
            filePixmap = sFile->GetIcon();

            QPixmap pmEmpty(c_iconSize, c_iconSize);
            pmEmpty.fill( Color(1.0f, 1.0f, 1.0f, 0.0f).ToQColor() );

            filePixmap = filePixmap.scaled(
                        c_iconSize, c_iconSize,
                        Qt::KeepAspectRatio,
                        Qt::TransformationMode::SmoothTransformation);
            filePixmap = IconManager::CenterPixmapInEmptyPixmap(pmEmpty,
                                                                filePixmap);
            delete sFile;
        }

        if (filePixmap.size().isEmpty())
        {
            filePixmap = QPixmap(c_iconSize, c_iconSize);
            filePixmap.fill( Color::White.ToQColor() );
        }

        QIcon fileIcon(filePixmap);
        QListWidgetItem *item = new QListWidgetItem(fileIcon,
                                                    fileName.ToQString());
        item->setSizeHint(QSize(c_iconSize * 1.5f, c_iconSize * 1.5f));
        listWidget->addItem(item);

        m_item_To_absoluteFilepath.Set(item, absFilepath);
    }
}

QListWidget *DialogBrowseAssetFile::CreateList() const
{
    QListWidget *listWidget = new QListWidget();
    listWidget->setViewMode(QListWidget::ViewMode::IconMode);
    listWidget->setMovement(QListWidget::Static);
    listWidget->setWrapping(true);
    listWidget->setResizeMode(QListWidget::ResizeMode::Adjust);
    listWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    listWidget->setTextElideMode(Qt::ElideLeft);
    listWidget->setWordWrap(true);
    listWidget->setIconSize( QSize(c_iconSize, c_iconSize) );
    listWidget->setSpacing(20);

    QObject::connect(listWidget, SIGNAL(doubleClicked(QModelIndex)),
                     this, SLOT(OnFileAccepted(QModelIndex)));
    QObject::connect(listWidget, SIGNAL(currentRowChanged(int)),
                     this, SLOT(OnSelectionChange(int)));

    return listWidget;
}

void DialogBrowseAssetFile::OnSelectionChange(int newIndex)
{
    m_openButton->setEnabled(true);

    QListWidgetItem *item = nullptr;
    if (m_tabWidget->currentWidget() == m_projectFileList)
    {
        item = m_projectFileList->item(newIndex);
    }
    else { item = m_engineFileList->item(newIndex); }

    String filepath = "";
    if (item && m_item_To_absoluteFilepath.ContainsKey(item))
    {
        filepath = m_item_To_absoluteFilepath[item];
    }

    filepath = filepath.ElideLeft(35);
    m_botFilepathLabel->setText(filepath.ToQString());
}

void DialogBrowseAssetFile::OnFileAccepted()
{
    QListWidget *currentListWidget =
            Object::SCast<QListWidget>(m_tabWidget->currentWidget());

    String selectedFilepath = "";
    QList<QListWidgetItem*> selectedItems = currentListWidget->selectedItems();
    if (!selectedItems.empty())
    {
        QListWidgetItem *selectedItem = selectedItems.front();
        if (m_item_To_absoluteFilepath.ContainsKey(selectedItem))
        {
            selectedFilepath = m_item_To_absoluteFilepath[selectedItem];
        }
    }

    *m_resultFile = selectedFilepath;
    close();
}

void DialogBrowseAssetFile::OnFileAccepted(QModelIndex modelIndex)
{
    OnFileAccepted();
}

