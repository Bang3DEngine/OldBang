#include "AttrWidgetColor.h"

AttrWidgetColor::AttrWidgetColor(const XMLAttribute &xmlAttribute,
                                 InspectorWidget *inspectorWidget) :
  AttributeWidget(xmlAttribute, inspectorWidget)
{
    QLayout *layout = new QHBoxLayout();
    m_layout->addLayout(layout, 1);

    m_colorDialog = new ColorDialog();
    m_colorLabel = new ColorLabel(this, m_colorDialog, &m_selectedColor);
    RefreshLabelColor();

    layout->addWidget(m_colorLabel);

    connect(m_colorDialog, SIGNAL(currentColorChanged(const QColor&)),
          this, SLOT(OnColorChanged(const QColor&)));

    AfterConstructor();
}

AttrWidgetColor::~AttrWidgetColor()
{
    delete m_colorDialog;
    delete m_colorLabel;
}

void AttrWidgetColor::RefreshLabelColor()
{
    m_colorLabel->SetColor(m_selectedColor);
    m_inspectorWidget->_OnSlotValueChanged();
}

void AttrWidgetColor::SetValue(const Color &c)
{
    m_selectedColor = c;
    RefreshLabelColor();
}

const Color& AttrWidgetColor::GetValue() const
{
    return m_selectedColor;
}

void AttrWidgetColor::Refresh(const XMLAttribute &attribute)
{
    AttributeWidget::Refresh(attribute);
    if (attribute.GetType() != XMLAttribute::Type::Color) return;
    SetValue( attribute.GetColor() );
}

void AttrWidgetColor::OnColorChanged(const QColor &c)
{
    SetValue(Color::FromQColor(c));
}


///////////////////////////////////////////////



// COLOR_DIALOG
ColorDialog::ColorDialog(QWidget *parent) : QColorDialog(parent)
{
    setModal(true);
    setOption(QColorDialog::NoButtons);
    setOption(QColorDialog::ShowAlphaChannel);
}



///////////////////////////////////////////////

//COLOR_LABEL
ColorLabel::ColorLabel(QWidget *parent,
                       ColorDialog *colorDialog,
                       const Color *selectedColor) : QLabel(parent)
{
    m_colorDialog = colorDialog;
    m_selectedColor = selectedColor;
}

void ColorLabel::SetColor(const Color &c)
{
    std::string ss = "background-color: rgb";
    ss += c.ToStringRgb255();
    setStyleSheet(QString::fromStdString(ss));
}

void ColorLabel::mousePressEvent(QMouseEvent *e)
{
    if (!m_colorDialog->isVisible())
    {
        Color c = *m_selectedColor;
        m_colorDialog->setCurrentColor(c.ToQColor());
        m_colorDialog->show();
    }
}
