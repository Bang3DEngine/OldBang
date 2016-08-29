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
    m_inspectorWidget(inspectorWidget)
{
    m_xmlAttribute = xmlAttribute;
    Refresh(xmlAttribute);

    m_layout = new QHBoxLayout();
    m_layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    m_layout->setContentsMargins(4, 4, 4, 4);
    setLayout(m_layout);
    setMinimumWidth(40);

    if (!needsLabelOnTop)
    {
        m_layout->addSpacing(10);
    }

    if (!isSubWidget)
    {
        QString label = "";
        if (createLabel)
        {
            String name = xmlAttribute.GetName();
            name = StringUtils::FormatInspectorLabel(name);
            label = QString::fromStdString(name);
        }

        QGridLayout *gridLayout = m_inspectorWidget->GetGridLayout();
        m_rowIndexInGridLayout = m_inspectorWidget->GetNextRowIndex();
        m_label = new QLabel(label);
        if (needsLabelOnTop)
        {
            gridLayout->addWidget(m_label, m_rowIndexInGridLayout, 0, 1, 2,
                                  Qt::AlignLeft | Qt::AlignVCenter);
            gridLayout->addWidget(this, m_rowIndexInGridLayout + 1, 0, 1, 2,
                                  Qt::AlignVCenter);
        }
        else
        {
            gridLayout->addWidget(m_label, m_rowIndexInGridLayout, 0, 1, 1,
                                  Qt::AlignLeft | Qt::AlignVCenter);
            gridLayout->addWidget(this, m_rowIndexInGridLayout, 1, 1, 1,
                                  Qt::AlignVCenter);
        }
    }

    Refresh(xmlAttribute);
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

    if (m_hidden) // Only hide, to avoid window flickering
    {
        setVisible(false);
        setHidden(true);
        if (m_label)
        {
            m_label->setVisible(false);
            m_label->setHidden(true);
        }
    }
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
