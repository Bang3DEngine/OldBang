#include "Bang/SystemClipboard.h"

#include BANG_SDL2_INCLUDE(SDL.h)

USING_NAMESPACE_BANG

void SystemClipboard::Set(const String &str)
{
    SDL_SetClipboardText(str.ToCString());
}

String SystemClipboard::Get()
{
    return String(SDL_GetClipboardText());
}
