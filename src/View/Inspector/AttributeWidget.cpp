#include "AttributeWidget.h"


#include "XMLNode.h"
#include "XMLAttribute.h"
#include "AttrWidgetBool.h"
#include "AttrWidgetEnum.h"
#include "AttrWidgetFile.h"
#include "AttrWidgetColor.h"
#include "AttrWidgetFloat.h"
#include "AttrWidgetString.h"
#include "AttrWidgetButton.h"
#include "AttrWidgetVectorFloat.h"

AttributeWidget::AttributeWidget(const XMLAttribute &xmlAttribute,
                                 InspectorWidget *inspectorWidget,
                                 bool createLabel) :
    IDroppableQWidget(),
    m_inspectorWidget(inspectorWidget)
{
    m_readonly =  xmlAttribute.HasProperty(XMLProperty::Readonly);
    m_enabled  = !xmlAttribute.HasProperty(XMLProperty::Disabled);
    m_inlined  =  xmlAttribute.HasProperty(XMLProperty::Inline);
    m_hidden   =  xmlAttribute.HasProperty(XMLProperty::Hidden);

    if (m_inlined) { m_layout = new QHBoxLayout(); }
    else { m_layout = new QVBoxLayout(); }
    setLayout(m_layout);
    m_layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    m_layout->setMargin(0);

    if (createLabel)
    {
        AddLabelWidget(xmlAttribute, m_layout);
    }

    setEnabled(m_enabled);
    setVisible(!m_hidden);
    setHidden(m_hidden);
}

void AttributeWidget::AfterConstructor()
{
    updateGeometry();
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    adjustSize();
    show();
}

void AttributeWidget::AddLabelWidget(const XMLAttribute &xmlAttribute,
                                     QBoxLayout *layout)
{
    std::string label = xmlAttribute.GetName();
    std::string fLabel = StringUtils::FormatInspectorLabel(label);
    QLabel *labelField = new QLabel(QString(fLabel.c_str()));
    labelField->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    labelField->show();

    layout->addWidget(labelField, 100);

    labelField->setMaximumHeight(15);
    labelField->setMargin(0);
}

void AttributeWidget::Refresh(const XMLAttribute &attribute)
{
    m_readonly =  attribute.HasProperty(XMLProperty::Readonly);
    m_enabled  = !attribute.HasProperty(XMLProperty::Disabled);
    m_inlined  =  attribute.HasProperty(XMLProperty::Inline);
    m_hidden   =  attribute.HasProperty(XMLProperty::Hidden);

    setEnabled(m_enabled);
    setVisible(!m_hidden);
    setHidden(m_hidden);
}

AttributeWidget *AttributeWidget::FromXMLAttribute(const XMLAttribute &xmlAttribute,
                                                   InspectorWidget *inspectorWidget)
{
    AttributeWidget *w = nullptr;
    XMLAttribute::Type attrType = xmlAttribute.GetType();

    if (xmlAttribute.HasVectoredType())
    {
        w = new AttrWidgetVectorFloat(xmlAttribute, inspectorWidget);
    }
    else if (attrType == XMLAttribute::Type::File)
    {
        w = new AttrWidgetFile(xmlAttribute, inspectorWidget);
    }
    else if (attrType == XMLAttribute::Type::String)
    {
        w = new AttrWidgetString(xmlAttribute, inspectorWidget);
    }
    else if (attrType == XMLAttribute::Type::Bool)
    {
        w = new AttrWidgetBool(xmlAttribute, inspectorWidget);
    }
    else if (attrType == XMLAttribute::Type::Enum)
    {
        w = new AttrWidgetEnum(xmlAttribute, inspectorWidget);
    }
    else if (attrType == XMLAttribute::Type::Color)
    {
        w = new AttrWidgetColor(xmlAttribute, inspectorWidget);
    }
    else if (attrType == XMLAttribute::Type::Button)
    {
        w = new AttrWidgetButton(xmlAttribute, inspectorWidget);
    }

    return w;
}
