#ifndef TEXTFORMATTER_H
#define TEXTFORMATTER_H

#include <functional>

#include "Bang/Font.h"
#include "Bang/Rect.h"
#include "Bang/Array.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/TextFormat.h"

class TextFormatter
{
public:
    struct CharRect
    {
        char character;
        Rect rect;
        CharRect(char _c, const Rect &_rect) : character(_c), rect(_rect) {}
    };

    static Array<CharRect> GetFormattedTextPositions(
                                            const String &content,
                                            const Font *font,
                                            HorizontalAlignment hAlignment,
                                            VerticalAlignment vAlignment,
                                            WrapMode hWrapMode,
                                            WrapMode vWrapMode,
                                            int textSize,
                                            const Vector2 &spacing,
                                            const Rect &limitsRectNDC);
private:
    TextFormatter() = delete;

    static Array< Array<CharRect> > SplitCharRectsInLines(
                                const String &content,
                                const Font *font,
                                const Array<CharRect> &charRects,
                                const Rect &limitsRect,
                                const Vector2 &spacing,
                                int textSize,
                                WrapMode hWrapMode);

    static void ApplyAlignment(Array< Array<CharRect> > *linedCharRects,
                               HorizontalAlignment hAlignment,
                               VerticalAlignment vAlignment,
                               const Rect &limitsRect);

    static void ApplyHorizontalHideWrap(Array< Array<CharRect> > *linedCharRects,
                                        const Rect &limitsRect);
    static void ApplyVerticalHideWrap(Array< Array<CharRect> > *linedCharRects,
                                      const Rect &limitsRect);

    static Vector2 GetTextSizeScaled(int textSize);
    static Rect GetCharRect(const Font *font,
                            int textSize,
                            char c);
    static float GetCharAdvanceX(const Font *font,
                                 int textSize,
                                 const String &content,
                                 int currentCharIndex);

    static Vector2 FindMinCoord(const Array<CharRect>& rects);
    static Vector2 FindMaxCoord(const Array<CharRect>& rects);
    static Vector2 FindMinCoord(const Array< Array<CharRect> >& rects);
    static Vector2 FindMaxCoord(const Array< Array<CharRect> >& rects);
};

#endif // TEXTFORMATTER_H
