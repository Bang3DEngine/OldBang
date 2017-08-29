#include "Bang/Object.h"

Object::Object() {}
Object::~Object() {}

void Object::Start()
{
    if (!IsStarted()) { OnStart(); m_started = true; }
}

void Object::OnStart() {}

void Object::SetEnabled(bool enabled) { m_enabled = enabled; }

bool Object::IsEnabled() const { return m_enabled; }
bool Object::IsStarted() const { return m_started; }
