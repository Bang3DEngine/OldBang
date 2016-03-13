#ifndef INSPECTORPARTASSETSW_H
#define INSPECTORPARTASSETSW_H

#include <vector>
#include <string>
#include <QFileDialog>
#include <QDir>

#include "Asset.h"
#include "InspectorPartSW.h"

class InspectorPartAssetSW : public InspectorPartSW
{
    Q_OBJECT

private:
    std::string assetPath;
    QLineEdit *filepathLineEdit;

public:
    InspectorPartAssetSW(const std::string &labelString,
                         const std::string &value,
                         InspectorPartWidget *parent);

    virtual void SetValue(const std::string &assetPath);
    virtual std::string GetValue();

public slots:
    void Browse();
};

#endif // INSPECTORPARTASSETSW_H
