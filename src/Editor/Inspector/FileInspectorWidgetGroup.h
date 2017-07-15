#ifndef FILEINSPECTORWIDGETGROUP_H
#define FILEINSPECTORWIDGETGROUP_H

#include "Bang/FileInspectorWidget.h"
#include "Bang/SingleInspectorWidgetGroup.h"

class FileInspectorWidgetGroup : public SingleInspectorWidgetGroup
{
public:
    FileInspectorWidgetGroup(FileInspectorWidget *fileInspWidget);
    virtual ~FileInspectorWidgetGroup();

    virtual String GetTitle() const override;

protected:
    FileInspectorWidget *p_fileInspectorWidget = nullptr;
};

#endif // FILEINSPECTORWIDGETGROUP_H
