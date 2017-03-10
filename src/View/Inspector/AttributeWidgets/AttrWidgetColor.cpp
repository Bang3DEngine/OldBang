#include "AttrWidgetColor.h"

#include "Math.h"
#include "EditorWindow.h"

AttrWidgetColor::AttrWidgetColor(const XMLAttribute &xmlAttribute,
                                 InspectorWidget *inspectorWidget) :
  AttributeWidget(xmlAttribute, inspectorWidget)
{
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setMargin(0); vLayout->setSpacing(0);
    m_layout->addLayout(vLayout, 1);

    m_colorDialog = new ColorDialog();
    m_colorLabel = new ColorLabel(this, m_colorDialog, &m_selectedColor);
    m_colorLabel->setFixedHeight(17);
    m_colorLabel->SetColor(m_selectedColor);
    m_colorLabel->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    vLayout->addWidget(m_colorLabel, 1);

    m_alphaWhiteLabel = new ColorLabel(this, m_colorDialog, &m_selectedColor);
    m_alphaBlackLabel = new ColorLabel(this, m_colorDialog, &m_selectedColor);
    m_alphaWhiteLabel->setFixedHeight(3);
    m_alphaBlackLabel->setFixedHeight(3);
    m_alphaWhiteLabel->SetColor( Color::White );
    m_alphaBlackLabel->SetColor( Color::Black );
    m_alphaWhiteLabel->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    m_alphaBlackLabel->setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    m_alphaHorizontalLayout = new QHBoxLayout();
    m_alphaHorizontalLayout->addWidget(m_alphaWhiteLabel, 1);
    m_alphaHorizontalLayout->addWidget(m_alphaBlackLabel, 0);
    vLayout->addLayout(m_alphaHorizontalLayout, 0);

    SetValue(m_selectedColor);

    connect(m_colorDialog, SIGNAL(currentColorChanged(const QColor&)),
            this, SLOT(OnColorChanged(const QColor&)));

    AfterConstructor();
}

AttrWidgetColor::~AttrWidgetColor()
{
    if (m_colorDialog) { delete m_colorDialog; }
    if (m_colorLabel)  { delete m_colorLabel; }
    if (m_alphaWhiteLabel)  { delete m_alphaWhiteLabel; }
    if (m_alphaBlackLabel)  { delete m_alphaBlackLabel; }
}

void AttrWidgetColor::SetValue(const Color &c)
{
    m_selectedColor = c;
    m_colorLabel->SetColor(m_selectedColor);

    int whiteStretch = Math::Round(1000 * m_selectedColor.a);
    int blackStretch = 1000 - whiteStretch;
    m_alphaHorizontalLayout->setStretch(0, whiteStretch);
    m_alphaHorizontalLayout->setStretch(1, blackStretch);
    m_alphaBlackLabel->setHidden( (blackStretch == 0) );
    m_alphaWhiteLabel->setHidden( (whiteStretch == 0) );
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
    m_inspectorWidget->_OnSlotValueChanged();
}


///////////////////////////////////////////////



// COLOR_DIALOG
ColorDialog::ColorDialog(QWidget *parent) : QColorDialog(parent)
{
    //setModal(true);
    setOption(QColorDialog::NoButtons);
    setOption(QColorDialog::ShowAlphaChannel);
    setOption(QColorDialog::DontUseNativeDialog);
}



///////////////////////////////////////////////

//COLOR_LABEL
ColorLabel::ColorLabel(QWidget *parent,
                       ColorDialog *colorDialog,
                       const Color *selectedColor) : QLabel(parent)
{
    m_colorDialog = colorDialog;
    m_selectedColor = selectedColor;
    setMinimumHeight(10);
    setMaximumHeight(10);
}

void ColorLabel::SetColor(const Color &c)
{
    String ss = "background-color: rgb";
    ss += c.ToStringRgb255();
    setStyleSheet( ss.ToQString() );
}

void ColorLabel::mousePressEvent(QMouseEvent *e)
{
    //if (!m_colorDialog->isVisible())
    {
        Color c = *m_selectedColor;
        //m_colorDialog->setModal(true);
        //m_colorDialog->setParent(EditorWindow::GetInstance()->GetMainWindow());
        m_colorDialog->open(nullptr, "");
        m_colorDialog->setCurrentColor(c.ToQColor());
    }
}
