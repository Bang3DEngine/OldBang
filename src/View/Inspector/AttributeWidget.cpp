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

AttributeWidget::AttributeWidget(const std::string &label,
                                 InspectorWidget *parent) : QWidget(nullptr),
    m_label(label), m_parent(parent)
{
}

QLabel *AttributeWidget::GetLabelWidget(const std::string &label)
{
    std::string fLabel = StringUtils::FormatInspectorLabel(label);
    QLabel *labelField = new QLabel(QString(fLabel.c_str()));
    labelField->setAlignment(Qt::AlignLeft);
    labelField->setContentsMargins(0,0,0,0);
    labelField->show();
    return labelField;
}

AttributeWidget *AttributeWidget::FromXMLAttribute(const XMLAttribute &xmlAttribute,
                                                   InspectorWidget *inspectorWidget)
{
    AttributeWidget *w = nullptr;

    bool readonly = xmlAttribute.HasProperty(XMLProperty::Readonly);
    bool inlined  = xmlAttribute.HasProperty(XMLProperty::Inline);

    std::string attrName  = xmlAttribute.GetName();
    XMLAttribute::Type attrType = xmlAttribute.GetType();

    if (xmlAttribute.HasVectoredType())
    {
        int numFields = xmlAttribute.GetNumberOfFieldsOfType();
        w = new AttrWidgetVectorFloat(attrName, numFields, inspectorWidget);
    }
    else if (attrType == XMLAttribute::Type::File)
    {
        std::string fileExtension =
                xmlAttribute.GetPropertyValue(XMLProperty::FileExtension.GetName());
        w = new AttrWidgetFile(attrName, fileExtension, readonly, inspectorWidget);
    }
    else if (attrType == XMLAttribute::Type::String)
    {
        w = new AttrWidgetString(attrName, inspectorWidget, readonly, inlined);
    }
    else if (attrType == XMLAttribute::Type::Bool)
    {
        w = new AttrWidgetBool(attrName, inspectorWidget);
    }
    else if (attrType == XMLAttribute::Type::Enum)
    {
        w = new AttrWidgetEnum(attrName,
                               xmlAttribute.GetEnumNames(), inspectorWidget);
    }
    else if (attrType == XMLAttribute::Type::Color)
    {
        w = new AttrWidgetColor(attrName, inspectorWidget);
    }

    return w;
}
