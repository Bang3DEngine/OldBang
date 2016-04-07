#ifndef LISTINSPECTORITEMWIDGET_H
#define LISTINSPECTORITEMWIDGET_H

#include "Bang.h"

#include "InspectorWidget.h"

class Part;
class InspectorPartWidget : public InspectorWidget
{
    Q_OBJECT

private:
    Part *relatedPart;
    QCheckBox *enabledCheckbox;

public:
    explicit InspectorPartWidget(Part *relatedPart);
    virtual ~InspectorPartWidget();

    virtual void OnCustomContextMenuRequested(QPoint point) override;

    void OnContextMenuRemovePartSelected();
    void OnContextMenuMoveUpSelected();
    void OnContextMenuMoveDownSelected();

    void OnEnabledCheckboxPressed(bool checked);
};

#endif // LISTINSPECTORITEM_H
