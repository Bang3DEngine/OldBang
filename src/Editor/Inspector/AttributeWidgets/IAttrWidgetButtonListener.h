#ifndef ATTRWIDGETBUTTONLISTENER_H
#define ATTRWIDGETBUTTONLISTENER_H

class AttrWidgetButton;
class IAttrWidgetButtonListener
{
protected:
    IAttrWidgetButtonListener();

public:
    virtual void OnButtonClicked(const AttrWidgetButton *clickedButton) = 0;
};

#endif // ATTRWIDGETBUTTONLISTENER_H
