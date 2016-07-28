#ifndef INSPECTORTEXTURE2DASSETWIDGET_H
#define INSPECTORTEXTURE2DASSETWIDGET_H

#include <string>

#include "FileWriter.h"
#include "FileTexture2DAsset.h"
#include "InspectorWidget.h"

class InspectorTexture2DAssetWidget : public InspectorWidget
{
private:
    std::string m_assetFilepath = "";
    XMLNode xmlInfo;

public:
    InspectorTexture2DAssetWidget(const FileTexture2DAsset &fileTex);
    virtual ~InspectorTexture2DAssetWidget();

public slots:
    void _OnSlotValueChanged() override;

};

#endif // INSPECTORTEXTURE2DASSETWIDGET_H
