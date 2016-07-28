#include "InspectorSW.h"

InspectorSW::InspectorSW(const std::string &label,
                         InspectorWidget *parent,
                         InspectorWidgetInfo *relatedInfo) : QWidget(nullptr),
    m_label(label), m_parent(parent), m_relatedInfo(relatedInfo)
{
}
