#ifndef IMAGEINSPECTORWIDGET_H
#define IMAGEINSPECTORWIDGET_H

#include "Bang/ImageFile.h"
#include "Bang/AssetInspectorWidget.h"

class ImageInspectorWidget : public FileInspectorWidget
{
public:
    ImageInspectorWidget(const ImageFile &ImageFile);
    virtual ~ImageInspectorWidget();

protected:
    void InitExtra() override;
    void Refresh() override;
    void OnDestroy() override;

    AttrWidgetString *m_dimensionsAW = nullptr;
};


#endif // IMAGEINSPECTORWIDGET_H
