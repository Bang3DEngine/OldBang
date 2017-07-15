#ifndef IMAGEINSPECTORWIDGET_H
#define IMAGEINSPECTORWIDGET_H

#include "Bang/ImageFile.h"
#include "Bang/FileInspectorWidget.h"

class ImageInspectorWidget : public FileInspectorWidget
{
public:
    ImageInspectorWidget(const ImageFile &ImageFile);
    virtual ~ImageInspectorWidget();

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnDestroy() override;

    AttrWidgetString *m_dimensionsAW = nullptr;
};


#endif // IMAGEINSPECTORWIDGET_H
