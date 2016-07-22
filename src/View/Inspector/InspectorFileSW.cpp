#include "InspectorFileSW.h"

InspectorFileSW::InspectorFileSW(const std::string &labelString,
                                 const std::string &assetPath,
                                 const std::string &fileExtension,
                                 InspectorWidget *parent) :
    InspectorSW(labelString, parent)
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

    p_filepathLineEdit = new QLineEdit();
    p_filepathLineEdit->setReadOnly(true);
    hLayout->addWidget(p_filepathLineEdit);

    QPushButton *browseButton = new QPushButton(QString::fromStdString("Browse"));
    connect(browseButton, SIGNAL(clicked()), this, SLOT(Browse()));
    hLayout->addWidget(browseButton);

    this->m_fileExtension = fileExtension;
    this->setContentsMargins(0,0,0,0);
    this->show();
    this->SetValue(assetPath);
}

void InspectorFileSW::Browse()
{
    FileDialog fd(Persistence::GetAssetsPathAbsolute(), m_fileExtension);
    std::string selectedFile = fd.GetOpenFilename();
    if (selectedFile != "")
    {
        this->SetValue( selectedFile );
        this->p_parent->_OnSlotValueChanged();
    }
}

void InspectorFileSW::SetValue(const std::string &assetPath)
{
    if (this->m_assetPath != assetPath)
    {
        this->m_assetPath = assetPath;
        p_filepathLineEdit->setText(QString::fromStdString(assetPath));
    }
}

std::string  InspectorFileSW::GetValue()
{
    return m_assetPath;
}
