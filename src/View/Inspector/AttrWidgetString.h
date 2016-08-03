#ifndef INSPECTORSTRINGSW_H
#define INSPECTORSTRINGSW_H

#include <string>
#include <QTimer>
#include <QFocusEvent>
#include <QDoubleSpinBox>

#include "AttributeWidget.h"

class LineEdit;
class AttrWidgetString : public AttributeWidget
{
    Q_OBJECT

private:
    LineEdit *m_strField = nullptr;
    bool m_editing = false;

public:
    AttrWidgetString(const std::string &labelString,
                     InspectorWidget *m_parent,
                     bool readonly = false,
                     bool inlined = false);

    virtual void SetValue(const std::string &value);
    virtual const std::string GetValue() const;

    virtual void OnFocusIn();
    virtual void OnFocusOut();
    QSize sizeHint() const override;
};


class LineEdit : public QLineEdit //Slot for a float (label + float)
{
    Q_OBJECT

public:
    LineEdit(bool readonly);

    virtual void focusInEvent(QFocusEvent * event) override;
    virtual void focusOutEvent(QFocusEvent * event) override;
    virtual void  keyPressEvent(QKeyEvent *event) override;

public slots:

    void Deselect();
    void SelectAll();
};

#endif // INSPECTORSTRINGSW_H
