#include "Bang/Cursor.h"

#include "Bang/Map.h"

USING_NAMESPACE_BANG

Cursor::Type Cursor::s_type = Cursor::Type::Arrow;

void Cursor::Set(Cursor::Type cursorType)
{
    Cursor::s_type = cursorType;

    Map<Cursor::Type, SDL_Cursor*> createdCursors;
    if ( !createdCursors.ContainsKey(Cursor::Get()) )
    {
        SDL_Cursor* cursor = SDL_CreateSystemCursor(
                                    SCAST<SDL_SystemCursor>(Cursor::Get()) );
        createdCursors.Add(Cursor::Get(), cursor);
    }

    SDL_SetCursor( createdCursors.Get(Cursor::Get()) );
}

Cursor::Type Cursor::Get()
{
    return Cursor::s_type;
}
