#ifndef TEXTFORMATTER_H
#define TEXTFORMATTER_H

#include <functional>

#include "Bang/Font.h"
#include "Bang/Rect.h"
#include "Bang/Alignment.h"

NAMESPACE_BANG_BEGIN

FORWARD class Font;
FORWARD class RectTransform;

class TextFormatter
{
public:
    struct CharRect
    {
        Rectf rectPx;
        char character;
        CharRect(char _c, const Rectf &_rect) : rectPx(_rect), character(_c) {}
    };

    static Array<CharRect> GetFormattedTextPositions(
                                        const String &content,
                                        const Font *font,
                                        const Recti &limitsRect,
                                        const Vector2 &spacingMultiplier,
                                        HorizontalAlignment hAlignment,
                                        VerticalAlignment vAlignment,
                                        bool wrapping);

    static Vector2i GetTextSizeOneLined(const String &content,
                                        const Font *font,
                                        const Vector2 &spacingMultiplier);

    TextFormatter() = delete;

private:
    static Array< Array<CharRect> > SplitCharRectsInLines(
                                            const String &content,
                                            const Font *font,
                                            const Recti &limitsRect,
                                            const Vector2 &spacingMultiplier,
                                            const Array<CharRect> &charRects,
                                            bool wrapping);

    static void ApplyAlignment(Array< Array<CharRect> > *linedCharRects,
                               const Recti &limitsRect,
                               const Font *font,
                               HorizontalAlignment hAlignment,
                               VerticalAlignment vAlignment);

    static Rectf GetCharRect(char c, const Font *font);
    static int GetCharAdvanceX(const String &content,
                               const Font *font,
                               int currentCharIndex);
};

NAMESPACE_BANG_END

#endif // TEXTFORMATTER_H
