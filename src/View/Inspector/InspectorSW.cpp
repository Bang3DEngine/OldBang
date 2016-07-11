#include "InspectorSW.h"

InspectorSW::InspectorSW(const std::string &label, InspectorWidget *parent) : QWidget(nullptr),
    m_label(label), p_parent(parent)
{
}
