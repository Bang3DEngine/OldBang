#include "Bang/TextFormatter.h"

#include "Bang/Font.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/RectTransform.h"

Array<TextFormatter::CharRect>
   TextFormatter::GetFormattedTextPositions(const String &content,
                                            const Font *font,
                                            HorizontalAlignment hAlignment,
                                            VerticalAlignment vAlignment,
                                            bool wrapping,
                                            int textSizePx,
                                            const RectTransform *rt,
                                            const Vector2 &spacingMultiplier)
{
    if (content.IsEmpty()) { return Array<CharRect>(); }

    // First create a list with all the character rects in the origin
    Array<CharRect> charRects;
    for (int i = 0; i < content.Size(); ++i)
    {
        const char c = content[i];
        Rectf charRectLocalNDC = TextFormatter::GetCharRect(font, rt,
                                                            textSizePx, c);
        charRects.PushBack( CharRect(c, charRectLocalNDC) );
    }

    Array< Array<CharRect> >
    linedCharRects = SplitCharRectsInLines(content, font, charRects,
                                           spacingMultiplier, rt,
                                           textSizePx, wrapping);

    TextFormatter::ApplyAlignment(&linedCharRects, hAlignment, vAlignment);

    Array<CharRect> finalCharRects; // Flattened result
    for (const Array<CharRect> &line : linedCharRects)
    {
        finalCharRects.PushBack(line);
    }
    return finalCharRects;
}

Array< Array<TextFormatter::CharRect> >
TextFormatter::SplitCharRectsInLines(const String &content,
                                     const Font *font,
                                     const Array<CharRect> &charRects,
                                     const Vector2f &spacingMultiplier,
                                     const RectTransform *rt,
                                     int textSizePx,
                                     bool wrapping)
{
    Array< Array<CharRect> > linedCharRects(1); // Result

    Vector2 penPosition(-1.0f);
    const float lineSkipNDC = GetLineSkipNDC(font, rt, textSizePx);
    for (int i = 0; i < content.Size(); ++i)
    {
        const float charAdvX = GetCharAdvanceXNDC(font, rt, content,
                                                  textSizePx, i);
        if (wrapping)
        {
            // Split the input char positions into the needed lines.
            // Each line will contain as many words as possible (split by spaces).
            bool lineBreak = false;
            if (content[i] != ' ')
            {
                lineBreak = (penPosition.x + charAdvX > 1.0f);
            }
            else
            {
                // We have arrived to a space.
                // Does the following word (after this space) still fits in
                // the current line?
                float tmpAdvX = penPosition.x + charAdvX * spacingMultiplier.x;
                for (int j = i+1; j < content.Size(); ++j)
                {
                    if (content[j] == ' ') { break; }
                    const float jCharAdvX = GetCharAdvanceXNDC(font, rt,
                                                               content,
                                                               textSizePx, j);
                    if (tmpAdvX + jCharAdvX > 1.0f)
                    {
                        lineBreak = true;
                        break;
                    }
                    tmpAdvX += (jCharAdvX * spacingMultiplier.x);
                }
            }
            lineBreak = lineBreak || content[i] == '\n';
            bool anticipatedLineBreak = ( (content[i] == ' ' && lineBreak) ||
                                           content[i] == '\n');

            if (lineBreak)
            {
                // Advance to next line! Add the current line to the result.
                penPosition.y -= (lineSkipNDC * spacingMultiplier.y);
                penPosition.x  = -1.0f;
                linedCharRects.PushBack( Array<CharRect>() );

                // Skip all next ' '
                if (content[i] == ' ')
                {
                    while (content[i] == ' ') { ++i; }
                    --i;
                }
            }

            if (!anticipatedLineBreak)
            {
                CharRect cr(content[i], charRects[i].rectLocalNDC + penPosition);
                linedCharRects.Back().PushBack(cr);
                penPosition.x += (charAdvX * spacingMultiplier.x);
            }
        }
        else // Just add them in a single line
        {
            CharRect cr(content[i], penPosition + charRects[i].rectLocalNDC);
            linedCharRects.Back().PushBack(cr);
            penPosition.x += (charAdvX * spacingMultiplier.x);
        }
    }
    return linedCharRects;
}

Vector2f FindMinCoord(const Array<TextFormatter::CharRect>&);
Vector2f FindMaxCoord(const Array<TextFormatter::CharRect>&);
Vector2f FindMinCoord(const Array< Array<TextFormatter::CharRect> >&);
Vector2f FindMaxCoord(const Array< Array<TextFormatter::CharRect> >&);

void TextFormatter::ApplyAlignment
(Array< Array<CharRect> > *linesCharRects,
 HorizontalAlignment hAlignment, VerticalAlignment vAlignment)
{
    // For each line save the HorizontalAlign it needs
    for (Array<CharRect> &line : *linesCharRects)
    {
        if (line.IsEmpty()) { continue; }
        Vector2f lineMinCoord = FindMinCoord(line);
        Vector2f lineMaxCoord = FindMaxCoord(line);

        float lineHorizontalOffset = 0.0f;
        if (hAlignment == HorizontalAlignment::Left)
        {
            lineHorizontalOffset = -1.0f - lineMinCoord.x;
        }
        else if (hAlignment == HorizontalAlignment::Center)
        {
            lineHorizontalOffset = 0.0f - (lineMaxCoord.x + lineMinCoord.x) / 2;
        }
        else if (hAlignment == HorizontalAlignment::Right)
        {
            lineHorizontalOffset = 1.0f - lineMaxCoord.x;
        }

        for (int i = 0; i < line.Size(); ++i)
        {
            line[i].rectLocalNDC += Vector2f(lineHorizontalOffset, 0);
        }
    }

    // Vertical align all the lines at once
    float lineVerticalOffset = 0.0f;
    Vector2f textMinCoords = FindMinCoord(*linesCharRects);
    Vector2f textMaxCoords = FindMaxCoord(*linesCharRects);
    if (vAlignment == VerticalAlignment::Top)
    {
        lineVerticalOffset = 1.0f - textMaxCoords.y;
    }
    else if (vAlignment == VerticalAlignment::Center)
    {
        lineVerticalOffset = 0.0f - (textMaxCoords.y + textMinCoords.y) * 0.5f;
    }
    else if (vAlignment == VerticalAlignment::Bot)
    {
        lineVerticalOffset = -1.0f - textMinCoords.y;
    }

    // Apply offsets
    for (Array<CharRect> &line : *linesCharRects)
    {
        for (int i = 0; i < line.Size(); ++i)
        {
            line[i].rectLocalNDC += Vector2f(0, lineVerticalOffset);
        }
    }
}

Rectf TextFormatter::GetCharRect
(const Font *font, const RectTransform *rt, int textSizePx, char c)
{
    if (!font) { return Rectf::Zero; }

    Font::GlyphMetrics
    charMetrics = font->GetCharMetrics(c, textSizePx);

    Vector2i charMin(charMetrics.bearing.x,
                     charMetrics.bearing.y - charMetrics.size.y);
    Vector2i charMax(charMetrics.bearing.x + charMetrics.size.x,
                     charMetrics.bearing.y);
    return Rectf(rt->FromPixelsAmountToLocalNDC(charMin),
                 rt->FromPixelsAmountToLocalNDC(charMax));
}

float TextFormatter::GetLineSkipNDC(const Font *font, const RectTransform *rt,
                                    int textSizePx)
{
    const int lineSkipScaled = Font::ScaleMagnitude(font->GetLineSkipPx(),
                                                      textSizePx);
    return rt->FromPixelsAmountToLocalNDC( Vector2i(0, lineSkipScaled) ).y;
}

float TextFormatter::GetCharAdvanceXNDC(const Font *font,
                                        const RectTransform *rt,
                                        const String &content, int textSizePx,
                                        int currentCharIndex)
{
    int advance = 0;
    if (currentCharIndex < content.Size()-1)
    {
        advance = font->GetKerningXPx(content[currentCharIndex],
                                      content[currentCharIndex + 1]);
    }

    if (advance <= 0)
    {
        const char c = content[currentCharIndex];
        Font::GlyphMetrics charMetrics = font->GetCharMetrics(c, textSizePx);
        advance = charMetrics.advance;
    }

    return rt->FromPixelsAmountToLocalNDC( Vector2i(advance, 0) ).x;
}

Vector2f FindMinCoord(const Array<TextFormatter::CharRect> &rects)
{
    Vector2f result;
    bool first = true;
    for (const TextFormatter::CharRect &cr : rects)
    {
        if (first) { first = false; result = cr.rectLocalNDC.GetMin(); }
        else { result = Vector2f::Min(result, cr.rectLocalNDC.GetMin()); }
    }
    return result;
}

Vector2f FindMaxCoord(const Array<TextFormatter::CharRect> &rects)
{
    Vector2f result;
    bool first = true;
    for (const TextFormatter::CharRect &cr : rects)
    {
        if (first) { first = false; result = cr.rectLocalNDC.GetMax(); }
        else { result = Vector2f::Max(result, cr.rectLocalNDC.GetMax()); }
    }
    return result;
}

Vector2f FindMinCoord(const Array< Array<TextFormatter::CharRect> > &rects)
{
    Vector2f result;
    bool first = true;
    for (const Array<TextFormatter::CharRect> &line : rects)
    {
        if (first) { first = false; result = FindMinCoord(line); }
        else { result = Vector2f::Min(result, FindMinCoord(line)); }
    }
    return result;
}

Vector2f FindMaxCoord(const Array< Array<TextFormatter::CharRect> > &rects)
{
    Vector2f result;
    bool first = true;
    for (const Array<TextFormatter::CharRect> &line : rects)
    {
        if (first) { first = false; result = FindMaxCoord(line); }
        else { result = Vector2f::Max(result, FindMaxCoord(line)); }
    }
    return result;
}
