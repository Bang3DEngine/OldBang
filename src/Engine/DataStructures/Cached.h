#ifndef CACHED_H
#define CACHED_H

#include "Bang.h"
#include "Bang/Debug.h"

template <class T>
class Cached
{
public:
    void Set(const T& value) noexcept { m_value = value; m_isValid = true; }
    const T& Get() const noexcept { ASSERT(IsValid()); return m_value; }
    void Invalidate() noexcept { m_isValid = false; }
    bool IsValid() const noexcept { return m_isValid; }

private:
    T m_value;
    bool m_isValid = false;
};

#endif // CACHED_H
