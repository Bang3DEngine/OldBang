#include "Bang/AttrWidgetFile.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include "Bang/WinUndef.h"

#include "Bang/BFile.h"
#include "Bang/Paths.h"
#include "Bang/Dialog.h"
#include "Bang/Explorer.h"
#include "Bang/Hierarchy.h"
#include "Bang/Application.h"
#include "Bang/IconManager.h"
#include "Bang/EditorWindow.h"
#include "Bang/InspectorWidget.h"
#include "Bang/DialogBrowseAssetFile.h"

AttrWidgetFile::AttrWidgetFile(const String &labelText,
                               const List<String> &allowedExtensions)
    : AttributeWidget()
{
    m_labelText = labelText;
    m_allowedExtensions = allowedExtensions;

    m_hLayout = new QHBoxLayout();
    m_horizontalLayout.addLayout(m_hLayout, 1);

    m_iconLabel = new QLabel();
    m_iconLabel->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    m_hLayout->addWidget(m_iconLabel, 0, Qt::AlignRight | Qt::AlignVCenter);


    // File Line Edit
    m_filepathLineEdit = new FileLineEdit();
    m_filepathLineEdit->setReadOnly(true);
    m_filepathLineEdit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_filepathLineEdit->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    m_hLayout->addWidget(m_filepathLineEdit, 100);
    QObject::connect(m_filepathLineEdit, SIGNAL(DoubleClicked()),
                     this, SLOT(OnDoubleClick()));
    //

    // Browse button
    static QIcon browseIcon(":/qss_icons/rc/radio_checked.png");
    bool readonly = false;
    if (!readonly)
    {
        QToolButton *browseButton = new QToolButton();
        browseButton->setIcon(browseIcon);
        browseButton->setFixedWidth(30);
        browseButton->setFixedHeight(24);
        connect(browseButton, SIGNAL(clicked()), this, SLOT(Browse()));
        m_hLayout->addWidget(browseButton, 0, Qt::AlignRight | Qt::AlignVCenter);
    }
    //


    SetHeightSizeHint(35);

    SetValue( Path::Empty );
    SetLabelText(labelText);
}

AttrWidgetFile::~AttrWidgetFile()
{
}

void AttrWidgetFile::Browse()
{
    String selectedFile = "";
    DialogBrowseAssetFile *dialog = new DialogBrowseAssetFile(&selectedFile);
    dialog->Show(EditorWindow::GetInstance()->GetMainWindow(),
                 m_labelText, m_allowedExtensions);
    while (dialog->isVisible()) { Application::processEvents(); }

    Path selectedPath(selectedFile);
    if (!selectedPath.IsEmpty())
    {
        SetValue(selectedPath);
        emit OnValueChanged(this);
    }
}

void AttrWidgetFile::RefreshIcon()
{
    const uint c_pixSize = 16;

    BFile file(m_filepath);
    BFile *f = BFile::GetSpecificFile(file);
    if (file.GetPath().IsFile() && f)
    {
        const QPixmap& pm = f->GetIcon();
        QPixmap pmCopy = pm.scaled(c_pixSize, c_pixSize,
                                   Qt::IgnoreAspectRatio,
                                   Qt::SmoothTransformation);
        m_iconLabel->setPixmap(pmCopy);
        delete f;
    }
    else
    {
        m_iconLabel->setPixmap( IconManager::GetEmptyPixmap()
                                             .scaled(c_pixSize, c_pixSize) );
    }
}

void AttrWidgetFile::SetValue(const Path &filepath)
{
    m_filepath = filepath;

    bool exists = m_filepath.Exists();
    String fileName = m_filepath.GetName();
    String fileText = exists ? fileName : "None";

    m_iconLabel->setVisible(exists);
    m_filepathLineEdit->SetBold(exists);
    blockSignals(true);
    m_filepathLineEdit->setText(fileText.ToQString());
    blockSignals(false);

    RefreshIcon();
}

String AttrWidgetFile::GetValue() const
{
    return m_filepath.GetName();
}

void AttrWidgetFile::OnDragStart(const DragDropInfo &ddi)
{
    bool readonly = false;
    if (!readonly)
    {
        Explorer *explorer = Explorer::GetInstance();
        if (ddi.sourceObject == explorer)
        {
            BFile f = explorer->GetSelectedFile();
            if (f.GetPath().HasExtension(m_allowedExtensions))
            {
                m_filepathLineEdit->setStyleSheet(
                            IDragDropListener::AcceptDragStyle);
            }
            else
            {
                m_filepathLineEdit->setStyleSheet(
                            IDragDropListener::RejectDragStyle);
            }
        }
    }
}

void AttrWidgetFile::OnDrop(const DragDropInfo &ddi)
{
    m_filepathLineEdit->setStyleSheet("/* */");

    Explorer  *explorer  = Explorer::GetInstance();
    if (ddi.currentObject == this)
    {
        if (ddi.sourceObject == explorer)
        {
            bool readonly = false;
            BFile f = explorer->GetSelectedFile();
            if (f.GetPath().HasExtension(m_allowedExtensions) &&
                !readonly)
            {
                SetValue(f.GetPath());
                emit OnValueChanged(this);
            }
        }
    }
}


void AttrWidgetFile::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (!m_filepath.IsEmpty()) { OnDoubleClick(); }
}

void AttrWidgetFile::OnUpdate()
{
    AttributeWidget::OnUpdate();
}

const Path &AttrWidgetFile::GetPath() const
{
    return m_filepath;
}

void AttrWidgetFile::OnDoubleClick()
{
    ENSURE(m_filepath.Exists());
    ENSURE(m_filepath.BeginsWith( Paths::Project() ));
    Explorer::GetInstance()->SelectPath(m_filepath);
}


////////////////////////////////////////////////////////

FileLineEdit::FileLineEdit(QWidget *parent) : QLineEdit(parent)
{
    setFixedHeight(20);
    setAlignment(Qt::AlignmentFlag::AlignLeft);
    connect (this, SIGNAL(selectionChanged()), this, SLOT(Deselect()));
}

void FileLineEdit::SetBold(bool bold)
{
    QFont currentFont = font();
    currentFont.setBold(bold);
    setFont( currentFont );
}

void FileLineEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    emit DoubleClicked();
}

void FileLineEdit::Deselect()
{
    deselect();
}
