#include "DialogBrowseAssetFile.h"

#include <QTabWidget>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "File.h"
#include "Color.h"
#include "Debug.h"
#include "Persistence.h"

DialogBrowseAssetFile::DialogBrowseAssetFile(String *resultFile)
{
    setWindowTitle("Browse Asset...");

    QVBoxLayout *vLayout = new QVBoxLayout();

    m_projectFileList = CreateList();
    m_engineFileList  = CreateList();

    QObject::connect(m_projectFileList, SIGNAL(doubleClicked(QModelIndex)),
                     this, SLOT(OnFileAccepted(QModelIndex)));

    m_tabWidget= new QTabWidget();
    m_tabWidget->addTab(m_projectFileList, "Project");
    m_tabWidget->addTab(m_engineFileList,  "Engine");
    vLayout->addWidget(m_tabWidget, 9999);

    QHBoxLayout *botHLayout = new QHBoxLayout();

    m_openButton = new QPushButton();
    m_openButton->setText("Open");
    m_openButton->setEnabled(false);
    QObject::connect(m_openButton, SIGNAL(pressed()),
                     this, SLOT(OnFileAccepted()));
    QObject::connect(m_projectFileList, SIGNAL(currentRowChanged(int)),
                     this, SLOT(OnSelectionChange(int)));

    botHLayout->addStretch(999);
    botHLayout->addWidget(m_openButton);

    vLayout->addLayout(botHLayout);
    setLayout(vLayout);

    m_resultFile = resultFile;
}

void DialogBrowseAssetFile::Show(QWidget *parent,
                                 const List<String> &extensions)
{
    setModal(parent != nullptr);

    List<String> projectFilepaths =
            Persistence::GetFiles(Persistence::GetProjectAssetsRootAbs(),
                                  true, extensions);
    List<String> engineFilepaths =
            Persistence::GetFiles(Persistence::GetEngineAssetsRootAbs(),
                                  true, extensions);

    ShowOnList(m_projectFileList, false, projectFilepaths);
    ShowOnList(m_engineFileList,  true,  engineFilepaths);
    show();
}

void DialogBrowseAssetFile::ShowOnList(QListWidget *listWidget,
                                       bool engineFilepaths,
                                       const List<String> &filepaths)
{
    for (const String &filepath : filepaths)
    {
        String fileName = Persistence::GetFileNameWithExtension(filepath);

        String absFilepath = Persistence::ToAbsolute(filepath, engineFilepaths);
        File file(absFilepath);

        QPixmap filePixmap;
        File *sFile = File::GetSpecificFile(file);
        if (sFile)
        {
            filePixmap = sFile->GetIcon();
            delete sFile;
        }

        if (filePixmap.size().isEmpty())
        {
            filePixmap = QPixmap(c_iconSize, c_iconSize);
            filePixmap.fill( Color::White.ToQColor() );
        }
        filePixmap = filePixmap.scaled(c_iconSize, c_iconSize);

        QIcon fileIcon(filePixmap);
        QListWidgetItem *item = new QListWidgetItem(fileIcon,
                                                    fileName.ToQString());
        item->setSizeHint(QSize(c_iconSize * 1.2f, c_iconSize * 1.2f));
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
    return listWidget;
}

void DialogBrowseAssetFile::OnSelectionChange(int newIndex)
{
    m_openButton->setEnabled(true);
}

void DialogBrowseAssetFile::OnFileAccepted()
{
    QListWidget *currentListWidget =
            static_cast<QListWidget*>(m_tabWidget->currentWidget());

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

