#include "Bang/AttributeWidget.h"

#include <QLabel>
#include <QBoxLayout>
#include "Bang/WinUndef.h"

#include "Bang/XMLNode.h"
#include "Bang/Inspector.h"
#include "Bang/XMLAttribute.h"
#include "Bang/AttrWidgetInt.h"
#include "Bang/AttrWidgetBool.h"
#include "Bang/AttrWidgetEnum.h"
#include "Bang/AttrWidgetFile.h"
#include "Bang/AttrWidgetColor.h"
#include "Bang/AttrWidgetFloat.h"
#include "Bang/InspectorWidget.h"
#include "Bang/AttrWidgetString.h"
#include "Bang/AttrWidgetButton.h"
#include "Bang/AttrWidgetVectorFloat.h"

AttributeWidget::AttributeWidget(const XMLAttribute &xmlAttribute,
                                 bool isSubWidget,
                                 bool createLabel,
                                 bool labelAbove )
{
    setVisible(false);
    m_xmlAttribute = xmlAttribute;
    Refresh(xmlAttribute);

    m_horizontalLayout.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_horizontalLayout.setContentsMargins(labelAbove ? 0 : 5,0,0,0);

    if (!isSubWidget)
    {
        QString label = "";
        if (createLabel)
        {
            String name = xmlAttribute.GetName();
            name = Inspector::FormatInspectorLabel(name);
            label = name.ToQString();
        }

        m_attrNameLabel = new QLabel(label);
        m_attrNameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        m_horizontalLayout.addWidget(m_attrNameLabel);
    }
    Refresh(xmlAttribute);

    setLayout(&m_horizontalLayout);
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

    const bool hasToBeHidden = m_hidden; // || p_inspectorWidget->IsClosed();
    if (hasToBeHidden && !isHidden()) // Only hide, to avoid window flickering
    {
        hide();
        if (m_attrNameLabel) { m_attrNameLabel->hide(); }
    }
    else if (!hasToBeHidden && isHidden())
    {
        setVisible(true); //show();
        if (m_attrNameLabel) { m_attrNameLabel->setVisible(true); } //show(); }
    }
}

int AttributeWidget::GetHeightSizeHint()
{
    if (m_hidden) // || p_inspectorWidget->IsClosed())
    {
        return 0;
    }
    return m_heightSizeHint;
}

AttributeWidget *AttributeWidget::FromXMLAttribute(const XMLAttribute &xmlAttr,
                                                   InspectorWidget *inspWidget)
{
    AttributeWidget *w = nullptr;
    XMLAttribute::Type attrType = xmlAttr.GetType();

    if (xmlAttr.HasVectoredType())
    {
        int numberOfFields = xmlAttr.GetNumberOfFieldsOfType();
        if (numberOfFields == 1)
        {
            if (attrType == XMLAttribute::Type::Float)
            {
                w = new AttrWidgetFloat(xmlAttr, false);
            }
            else if (attrType == XMLAttribute::Type::Int)
            {
                w = new AttrWidgetInt(xmlAttr, false);
            }
        }
        else
        {
            w = new AttrWidgetVectorFloat(xmlAttr);
        }
    }
    else if (attrType == XMLAttribute::Type::File)
    {
        w = new AttrWidgetFile(xmlAttr);
    }
    else if (attrType == XMLAttribute::Type::String)
    {
        w = new AttrWidgetString(xmlAttr);
    }
    else if (attrType == XMLAttribute::Type::Bool)
    {
        w = new AttrWidgetBool(xmlAttr);
    }
    else if (attrType == XMLAttribute::Type::Enum)
    {
        w = new AttrWidgetEnum(xmlAttr);
    }
    else if (attrType == XMLAttribute::Type::Color)
    {
        w = new AttrWidgetColor(xmlAttr);
    }
    else if (attrType == XMLAttribute::Type::Button)
    {
        w = new AttrWidgetButton(xmlAttr);
    }

    return w;
}
