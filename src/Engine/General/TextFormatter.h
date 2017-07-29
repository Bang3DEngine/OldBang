#ifndef TEXTFORMATTER_H
#define TEXTFORMATTER_H

#include <functional>

#include "Bang/Font.h"
#include "Bang/Rect.h"
#include "Bang/TextFormat.h"

FORWARD   class Font;

class TextFormatter
{
public:
    struct CharRect
    {
        char character;
        Recti rect;
        CharRect(char _c, const Recti &_rect) : character(_c), rect(_rect) {}
    };

    static Array<CharRect> GetFormattedTextPositions(
                                            const String &content,
                                            const Font *font,
                                            HorizontalAlignment hAlignment,
                                            VerticalAlignment vAlignment,
                                            WrapMode hWrapMode,
                                            WrapMode vWrapMode,
                                            int textSize,
                                            const Vector2i &spacing,
                                            const Recti &limitsRect);
private:
    TextFormatter() = delete;

    static Array< Array<CharRect> > SplitCharRectsInLines(
                                const String &content,
                                const Font *font,
                                const Array<CharRect> &charRects,
                                const Recti &limitsRect,
                                const Vector2i &spacing,
                                int textSize,
                                WrapMode hWrapMode);

    static void ApplyAlignment(Array< Array<CharRect> > *linedCharRects,
                               HorizontalAlignment hAlignment,
                               VerticalAlignment vAlignment,
                               const Recti &limitsRect);

    static void ApplyHorizontalHideWrap(Array< Array<CharRect> > *linedCharRects,
                                        const Recti &limitsRect);
    static void ApplyVerticalHideWrap(Array< Array<CharRect> > *linedCharRects,
                                      const Recti &limitsRect);

    static Recti GetCharRect(const Font *font,
                             int textSize,
                             char c);
    static int GetCharAdvanceX(const Font *font,
                               int textSize,
                               const String &content,
                               int currentCharIndex);

    static Vector2i FindMinCoord(const Array<CharRect>& rects);
    static Vector2i FindMaxCoord(const Array<CharRect>& rects);
    static Vector2i FindMinCoord(const Array< Array<CharRect> >& rects);
    static Vector2i FindMaxCoord(const Array< Array<CharRect> >& rects);
};

#endif // TEXTFORMATTER_H
