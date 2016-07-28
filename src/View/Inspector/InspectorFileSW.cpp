#include "InspectorFileSW.h"

InspectorFileSW::InspectorFileSW(const std::string &labelString,
                                 const std::string &assetPath,
                                 const std::string &fileExtension,
                                 InspectorWidget *parent,
                                 InspectorWidgetInfo *relatedInfo) :
    InspectorSW(labelString, parent, relatedInfo)
{
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setSpacing(0);
    this->setLayout(vLayout);

    QLabel *textLabel = new QLabel(QString::fromStdString(labelString));
    textLabel->setContentsMargins(0,0,0,0);
    vLayout->addWidget(textLabel);

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setSpacing(0); hLayout->setContentsMargins(0,0,0,0);
    vLayout->addLayout(hLayout);

    m_filepathLineEdit = new QLineEdit();
    m_filepathLineEdit->setReadOnly(true);
    hLayout->addWidget(m_filepathLineEdit);

    QPushButton *browseButton = new QPushButton(QString::fromStdString("Browse"));
    connect(browseButton, SIGNAL(clicked()), this, SLOT(Browse()));
    hLayout->addWidget(browseButton);

    m_fileExtension = fileExtension;
    setContentsMargins(0,0,0,0);
    show();
    SetValue(assetPath);
}

void InspectorFileSW::Browse()
{
    FileDialog fd(Persistence::GetAssetsPathAbsolute(), m_fileExtension);
    std::string selectedFile = fd.GetOpenFilename();
    if (selectedFile != "")
    {
        SetValue( selectedFile );
        m_parent->_OnSlotValueChanged();
    }
}

void InspectorFileSW::SetValue(const std::string &assetPath)
{
    if (m_assetPath != assetPath)
    {
        Logger_Log("InspectorFileSW::SetValue " << assetPath);
        m_assetPath = assetPath;
        m_filepathLineEdit->setText(QString::fromStdString(assetPath));
    }
}

std::string  InspectorFileSW::GetValue()
{
    return m_assetPath;
}
