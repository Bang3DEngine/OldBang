#include "FileInspectorWidgetGroup.h"

FileInspectorWidgetGroup::FileInspectorWidgetGroup(
        FileInspectorWidget *fileInspWidget)
    : SingleInspectorWidgetGroup(fileInspWidget)
{
    p_fileInspectorWidget = fileInspWidget;
}

FileInspectorWidgetGroup::~FileInspectorWidgetGroup()
{

}

String FileInspectorWidgetGroup::GetTitle() const
{
    return p_fileInspectorWidget->GetFile().GetPath().GetName();
}
