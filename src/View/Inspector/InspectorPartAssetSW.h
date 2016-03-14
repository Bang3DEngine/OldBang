#ifndef INSPECTORPARTASSETSW_H
#define INSPECTORPARTASSETSW_H

#include <vector>
#include <string>
#include <QFileDialog>
#include <QDir>

#include "Asset.h"
#include "Mesh.h"
#include "Explorer.h"
#include "FileDialogAsset.h"
#include "InspectorPartSW.h"
#include "WindowEventManager.h"

class InspectorPartAssetSW : public InspectorPartSW
{
    Q_OBJECT

private:
    std::string fileExtension;
    std::string assetPath;
    QLineEdit *filepathLineEdit;

public:
    InspectorPartAssetSW(const std::string &labelString,
                         const std::string &value,
                         const std::string &fileExtension,
                         InspectorPartWidget *parent);

    virtual void SetValue(const std::string &assetPath);
    virtual std::string GetValue();

public slots:
    void Browse();
};

#endif // INSPECTORPARTASSETSW_H
