#ifndef CURSOR_H
#define CURSOR_H

#include <SDL2/SDL_mouse.h>

#include "Bang/Bang.h"

NAMESPACE_BANG_BEGIN

class Cursor
{
public:
    enum class Type
    {
        Arrow     = SDL_SYSTEM_CURSOR_ARROW,
        Crosshair = SDL_SYSTEM_CURSOR_CROSSHAIR,
        Hand      = SDL_SYSTEM_CURSOR_HAND,
        IBeam     = SDL_SYSTEM_CURSOR_IBEAM,
        No        = SDL_SYSTEM_CURSOR_NO,
        SizeNWSE  = SDL_SYSTEM_CURSOR_SIZENWSE,
        SizeNESW  = SDL_SYSTEM_CURSOR_SIZENESW,
        SizeWE    = SDL_SYSTEM_CURSOR_SIZEWE,
        SizeNS    = SDL_SYSTEM_CURSOR_SIZENS,
        SizeAll   = SDL_SYSTEM_CURSOR_SIZEALL,
        Wait      = SDL_SYSTEM_CURSOR_WAIT,
        WaitArrow = SDL_SYSTEM_CURSOR_WAITARROW
    };

    static void Set(Type cursorType);
    static Type Get();

    Cursor() = delete;
    virtual ~Cursor() = delete;

private:
    static Type s_type;

};

NAMESPACE_BANG_END

#endif // CURSOR_H

