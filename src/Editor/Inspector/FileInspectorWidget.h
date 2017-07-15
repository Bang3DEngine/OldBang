#ifndef FILEINSPECTORWIDGET_H
#define FILEINSPECTORWIDGET_H

#include "Bang/File.h"
#include "Bang/InspectorWidget.h"

class FileInspectorWidget : public InspectorWidget
{
public:
    FileInspectorWidget(const File &file);
    virtual ~FileInspectorWidget();

     const File &GetFile() const;

protected:
    void InitExtra() override;
    void OnUpdate() override;
    void OnDestroy() override;

    File m_file;
};

#endif // FILEINSPECTORWIDGET_H
