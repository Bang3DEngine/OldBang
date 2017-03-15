#include "AttributeWidget.h"

#include <QLabel>
#include <QBoxLayout>

#include "XMLNode.h"
#include "Inspector.h"
#include "XMLAttribute.h"
#include "AttrWidgetInt.h"
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
    setVisible(false);
    m_xmlAttribute = xmlAttribute;
    Refresh(xmlAttribute);

    m_layout = new QHBoxLayout();
    m_layout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_layout->setContentsMargins(labelAbove ? 0 : 5,0,0,0);

    if (!isSubWidget)
    {
        QString label = "";
        if (createLabel)
        {
            String name = xmlAttribute.GetName();
            name = Inspector::FormatInspectorLabel(name);
            label = name.ToQString();
        }

        QGridLayout *gridLayout = m_inspectorWidget->GetGridLayout();
        m_rowIndexInGridLayout = m_inspectorWidget->GetNextRowIndex();

        const int widgetRow = m_rowIndexInGridLayout + (labelAbove ? 1 : 0);
        const int widgetCol = (labelAbove ? 0 : 1);
        const int colSpan   = (labelAbove ? 2 : 1);
        m_label = new QLabel(label);
        m_label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        gridLayout->addWidget(m_label, m_rowIndexInGridLayout, 0, 1,
                              colSpan, Qt::AlignLeft | Qt::AlignVCenter);
        gridLayout->addWidget(this, widgetRow, widgetCol, 1,
                              colSpan, Qt::AlignVCenter);
    }
    Refresh(xmlAttribute);

    setLayout(m_layout);
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    m_heightSizeHint = labelAbove ? 45 : 25;
    setVisible(true);
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

    const bool hasToBeHidden = m_hidden || m_inspectorWidget->IsClosed();
    if (hasToBeHidden && !isHidden()) // Only hide, to avoid window flickering
    {
        hide();
        if (m_label) { m_label->hide(); }
    }
    else if (!hasToBeHidden && isHidden())
    {
        setVisible(true); //show();
        if (m_label) { m_label->setVisible(true); } //show(); }
    }
}

int AttributeWidget::GetHeightSizeHint()
{
    if (m_hidden || m_inspectorWidget->IsClosed())
    {
        return 0;
    }
    return m_heightSizeHint;
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
            if (attrType == XMLAttribute::Type::Float)
            {
                w = new AttrWidgetFloat(xmlAttribute, inspectorWidget, false);
            }
            else if (attrType == XMLAttribute::Type::Int)
            {
                w = new AttrWidgetInt(xmlAttribute, inspectorWidget, false);
            }
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
