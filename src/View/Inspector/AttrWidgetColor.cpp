#include "AttrWidgetColor.h"


AttrWidgetColor::AttrWidgetColor(const std::string &labelString,
                                 InspectorWidget *parent) :
  AttributeWidget(labelString, parent)
{
  QLayout *layout = new QHBoxLayout();
  setLayout(layout);

  m_colorDialog = new ColorDialog();
  m_colorLabel = new ColorLabel(this, m_colorDialog, &m_selectedColor);
  RefreshLabelColor();

  layout->addWidget(GetLabelWidget(labelString));
  layout->addWidget(m_colorLabel);


  setContentsMargins(0,0,0,0);
  show();

  connect(m_colorDialog, SIGNAL(currentColorChanged(const QColor&)),
          this, SLOT(OnColorChanged(const QColor&)));

  setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

AttrWidgetColor::~AttrWidgetColor()
{
    delete m_colorDialog;
    delete m_colorLabel;
}

void AttrWidgetColor::RefreshLabelColor()
{
    m_colorLabel->SetColor(m_selectedColor);
    m_parent->_OnSlotValueChanged();
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
