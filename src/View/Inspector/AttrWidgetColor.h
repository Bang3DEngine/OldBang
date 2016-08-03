#ifndef ATTRWIDGETCOLOR_H
#define ATTRWIDGETCOLOR_H

#include <QColorDialog>

#include "AttributeWidget.h"

class ColorDialog;
class ColorLabel;
class AttrWidgetColor : public AttributeWidget
{
    Q_OBJECT

private:

    ColorDialog *m_colorDialog = nullptr;
    ColorLabel *m_colorLabel = nullptr;
    Color m_selectedColor = Color::white;

    void RefreshLabelColor();

public:

    AttrWidgetColor(const std::string &labelString,
                    InspectorWidget *parent);
    virtual ~AttrWidgetColor();

    void SetValue(const Color &c);
    const Color& GetValue() const;

private slots:

    void OnColorChanged(const QColor &c);
};

//////////////////////////////////////////////////


class ColorDialog : public QColorDialog
{
    Q_OBJECT

public:
    explicit ColorDialog(QWidget *parent = nullptr);
};

//////////////////////////////////////////////////

class ColorLabel : public QLabel
{
    Q_OBJECT

private:
    ColorDialog *m_colorDialog = nullptr;
    const Color *m_selectedColor = nullptr;

public:
    explicit ColorLabel(QWidget *parent,
                        ColorDialog *colorDialog,
                        const Color *selectedColor);

    void SetColor(const Color &c);

    virtual void mousePressEvent(QMouseEvent *e) override;
};

#endif // ATTRWIDGETCOLOR_H
