#ifndef INSPECTORPARTASSETSW_H
#define INSPECTORPARTASSETSW_H

#include <vector>
#include <string>
#include <Asset.h>

#include "InspectorPartWidget.h"

class InspectorPartAssetSW : public InspectorPartSW
{
private:
    std::string assetPath;

    void Browse();

public:
    InspectorPartAssetSW(const std::string &labelString,
                         const std::string &value,
                         InspectorPartWidget *parent);

    virtual void SetValue(const std::string &assetPath);
    virtual std::string GetValue();
};

#endif // INSPECTORPARTASSETSW_H
