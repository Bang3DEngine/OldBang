#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

enum class HorizontalAlignment { Left = 0, Center, Right };
enum class VerticalAlignment   { Top  = 0, Center, Bot   };
enum class VerticalSide { Top = 0, Bot};
enum class HorizontalSide { Left = 0, Right };
enum class Side { Left = 0, Right, Top, Bot };

NAMESPACE_BANG_END

#endif // ALIGNMENT_H
