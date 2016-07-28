#include "InspectorSW.h"

InspectorSW::InspectorSW(const std::string &label,
                         InspectorWidget *parent,
                         XMLNode *relatedXMLInfo) : QWidget(nullptr),
    m_label(label), m_parent(parent), m_relatedXMLInfo(relatedXMLInfo)
{
}
