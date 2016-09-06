#include "Array.h"

#include "String.h"

template <class T>
String Array<T>::ToString()
{
    std::ostringstream oss;
    oss << "["; bool first = true;
    for (auto it = this->Begin(); it != this->End(); ++it)
    {
        if (!first) oss << ", ";
        oss << (*it);
        first = false;
    }
    oss <<"]";
    return String(oss.str());
}
