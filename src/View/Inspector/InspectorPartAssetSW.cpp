#include "InspectorPartAssetSW.h"

InspectorPartAssetSW::InspectorPartAssetSW(const std::string &labelString, const std::string &value, InspectorPartWidget *parent) :
    InspectorPartSW(parent)
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

    QLineEdit *filepathLineEdit = new QLineEdit();
    hLayout->addWidget(filepathLineEdit);

    QPushButton *browseButton = new QPushButton();
    connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
    hLayout->addWidget(browseButton);

    this->setContentsMargins(0,0,0,0);
    this->show();
}

void InspectorPartAssetSW::Browse()
{
    QString directory = QFileDialog::getExistingDirectory(this,
                            tr("Find Files"), QDir::currentPath());

    if (!directory.isEmpty()) {
        if (directoryComboBox->findText(directory) == -1)
            directoryComboBox->addItem(directory);
        directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
    }
}

void InspectorPartAssetSW::SetValue(const std::string &assetPath)
{
    this->assetPath = assetPath;
}

std::string  InspectorPartAssetSW::GetValue()
{
    return assetPath;
}
