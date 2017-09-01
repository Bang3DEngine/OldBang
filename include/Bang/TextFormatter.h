#ifndef TEXTFORMATTER_H
#define TEXTFORMATTER_H

#include <functional>

#include "Bang/Font.h"
#include "Bang/Rect.h"
#include "Bang/TextFormat.h"

FORWARD class Font;
FORWARD class RectTransform;

class TextFormatter
{
public:
    struct CharRect
    {
        Rectf rectLocalNDC;
        char character;
        CharRect(char _c, const Rectf &_rect) : rectLocalNDC(_rect), character(_c) {}
    };

    static Array<CharRect> GetFormattedTextPositions(
                                            const String &content,
                                            const Font *font,
                                            HorizontalAlignment hAlignment,
                                            VerticalAlignment vAlignment,
                                            bool wrapping,
                                            int textSizePx,
                                            const RectTransform *rt,
                                            const Vector2i &extraSpacingPx);
    TextFormatter() = delete;

private:
    static Array< Array<CharRect> > SplitCharRectsInLines(
                                const String &content,
                                const Font *font,
                                const Array<CharRect> &charRects,
                                const Vector2f &extraSpacingLocalNDC,
                                const RectTransform *rt,
                                int textSizePx,
                                bool wrapping);

    static void ApplyAlignment(Array< Array<CharRect> > *linedCharRects,
                               HorizontalAlignment hAlignment,
                               VerticalAlignment vAlignment);

    static Rectf GetCharRect(const Font *font,
                             const RectTransform *rt,
                             int textSizePx,
                             char c);
    static float GetLineSkipNDC(const Font *font,
                                const RectTransform *rt,
                                int textSizePx);
    static float GetCharAdvanceXNDC(const Font *font,
                                    const RectTransform *rt,
                                    const String &content,
                                    int textSizePx,
                                    int currentCharIndex);
};

#endif // TEXTFORMATTER_H
