#include "AttributeWidget.h"

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
