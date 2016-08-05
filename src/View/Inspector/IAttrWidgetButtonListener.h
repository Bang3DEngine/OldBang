#ifndef ATTRWIDGETBUTTONLISTENER_H
#define ATTRWIDGETBUTTONLISTENER_H

class IAttrWidgetButtonListener
{
protected:
    IAttrWidgetButtonListener();

public:
    virtual void OnButtonClicked() = 0;
};

#endif // ATTRWIDGETBUTTONLISTENER_H
