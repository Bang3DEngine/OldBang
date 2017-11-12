#ifndef BANG_H
#define BANG_H

#include "BangDefines.h"

template<class EnumT> constexpr EnumT Undef() { return SCAST<EnumT>(-1); }

#endif // BANG_H
