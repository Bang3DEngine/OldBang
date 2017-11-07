#ifndef OBJECT_H
#define OBJECT_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class Object
{
public:
    virtual void Start();
    virtual void OnStart();
    virtual void OnEnabled();
    virtual void OnDisabled();

    void SetEnabled(bool enabled);

    bool IsEnabled() const;
    bool IsStarted() const;

protected:
    Object() = default;
    virtual ~Object() = default;

private:
    bool m_enabled = true;
    bool m_started = false;

    friend class DestroyManager;
};

NAMESPACE_BANG_END

#endif // OBJECT_H
