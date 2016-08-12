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
                                 bool isSubWidget,
                                 bool createLabel,
                                 bool needsLabelOnTop) :
    IDroppableQWidget(),
    m_inspectorWidget(inspectorWidget)
{
    m_xmlAttribute = xmlAttribute;

    m_readonly =  xmlAttribute.HasProperty(XMLProperty::Readonly);
    m_enabled  = !xmlAttribute.HasProperty(XMLProperty::Disabled);
    m_inlined  =  xmlAttribute.HasProperty(XMLProperty::Inline);
    m_hidden   =  xmlAttribute.HasProperty(XMLProperty::Hidden);

    m_layout = new QHBoxLayout();
    m_layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    m_layout->setContentsMargins(4, 4, 4, 4);
    setLayout(m_layout);
    setMinimumWidth(40);
    if (!needsLabelOnTop)
    {
        m_layout->addSpacing(10);
    }

    if (!m_hidden && !isSubWidget)
    {
        QString label = "";
        if (createLabel)
        {
            std::string name = xmlAttribute.GetName();
            name = StringUtils::FormatInspectorLabel(name);
            label = QString::fromStdString(name);
        }

        QFormLayout *fmLayout = m_inspectorWidget->GetFormLayout();
        if (needsLabelOnTop)
        {
            QLabel *ql = new QLabel(label);
            fmLayout->addRow(ql);
            fmLayout->addRow(this);
        }
        else
        {
            fmLayout->addRow(label, this);
        }
    }

    setEnabled(m_enabled);
    setVisible(!m_hidden);
    setHidden(m_hidden);
}

void AttributeWidget::AfterConstructor()
{
    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
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
        int numberOfFields = xmlAttribute.GetNumberOfFieldsOfType();
        if (numberOfFields == 1)
        {
            w = new AttrWidgetFloat(xmlAttribute, inspectorWidget, false);
        }
        else
        {
            w = new AttrWidgetVectorFloat(xmlAttribute, inspectorWidget);
        }
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
