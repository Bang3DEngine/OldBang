#include "AttributeWidget.h"

#include <QLabel>
#include <QBoxLayout>

#include "XMLNode.h"
#include "XMLAttribute.h"
#include "AttrWidgetBool.h"
#include "AttrWidgetEnum.h"
#include "AttrWidgetFile.h"
#include "AttrWidgetColor.h"
#include "AttrWidgetFloat.h"
#include "InspectorWidget.h"
#include "AttrWidgetString.h"
#include "AttrWidgetButton.h"
#include "AttrWidgetVectorFloat.h"

AttributeWidget::AttributeWidget(const XMLAttribute &xmlAttribute,
                                 InspectorWidget *inspectorWidget,
                                 bool isSubWidget,
                                 bool createLabel,
                                 bool labelAbove ) :
    m_inspectorWidget(inspectorWidget)
{
    m_xmlAttribute = xmlAttribute;
    Refresh(xmlAttribute);

    m_layout = new QHBoxLayout();
    m_layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    m_layout->setContentsMargins(2, 2, 2, 2);
    setLayout(m_layout);
    setMinimumWidth(40);

    if (!labelAbove)
    {
        m_layout->setSpacing(5); // Margin to the right of the label
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

        int widgetRow = m_rowIndexInGridLayout + (labelAbove ? 1 : 0);
        int widgetCol = (labelAbove ? 0 : 1);
        int colSpan   = (labelAbove ? 2 : 1);
        m_label = new QLabel(label);
        gridLayout->addWidget(m_label, m_rowIndexInGridLayout,         0, 1, colSpan, Qt::AlignLeft | Qt::AlignVCenter);
        gridLayout->addWidget(this,    widgetRow,              widgetCol, 1, colSpan, Qt::AlignVCenter);
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
