#include "Bang/TextFormatter.h"

#include "Bang/Font.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"
#include "Bang/RectTransform.h"

USING_NAMESPACE_BANG

Array<TextFormatter::CharRect>
   TextFormatter::GetFormattedTextPositions(const String &content,
                                            const Font *font,
                                            const Recti &limitsRect,
                                            const Vector2 &spacingMultiplier,
                                            HorizontalAlignment hAlignment,
                                            VerticalAlignment vAlignment,
                                            bool wrapping)
{
    if (content.IsEmpty()) { return Array<CharRect>(); }

    // First create a list with all the character rects in the origin
    Array<CharRect> charRects;
    for (int i = 0; i < content.Size(); ++i)
    {
        const char c = content[i];
        Rectf charRect = TextFormatter::GetCharRect(c, font);
        charRects.PushBack( CharRect(c, charRect) );
    }

    Array< Array<CharRect> >
    linedCharRects = SplitCharRectsInLines(content, font, limitsRect,
                                           spacingMultiplier,
                                           charRects, wrapping);

    TextFormatter::ApplyAlignment(&linedCharRects, limitsRect, font,
                                   hAlignment, vAlignment);

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
                                     const Recti &limitsRect,
                                     const Vector2 &spacingMult,
                                     const Array<CharRect> &charRects,
                                     bool wrapping)
{
    Array< Array<CharRect> > linedCharRects(1); // Result

    Vector2 penPosition (limitsRect.GetMin());
    const float lineSkip = font->GetLineSkip();
    penPosition.y += lineSkip;
    for (int i = 0; i < content.Size(); ++i)
    {
        const int charAdvX = GetCharAdvanceX(content, font, i);
        if (wrapping)
        {
            // Split the input char positions into the needed lines.
            // Each line will contain as many words as possible (split by spaces).
            bool lineBreak = false;
            if (content[i] != ' ')
            {
                lineBreak = (penPosition.x + charAdvX > limitsRect.GetMax().x);
            }
            else
            {
                // We have arrived to a space.
                // Does the following word (after this space) still fits in
                // the current line?
                int tmpAdvX = penPosition.x + charAdvX;
                for (int j = i+1; j < content.Size(); ++j)
                {
                    if (content[j] == ' ') { break; }
                    const int jCharAdvX = GetCharAdvanceX(content, font, j);
                    if (tmpAdvX + jCharAdvX > limitsRect.GetMax().x)
                    {
                        lineBreak = true;
                        break;
                    }
                    tmpAdvX += jCharAdvX * spacingMult.x;
                }
            }
            lineBreak = lineBreak || content[i] == '\n';
            bool anticipatedLineBreak = ( (content[i] == ' ' && lineBreak) ||
                                           content[i] == '\n');

            if (lineBreak)
            {
                // Advance to next line! Add the current line to the result.
                penPosition.y += lineSkip * spacingMult.y;
                penPosition.x  = limitsRect.GetMin().x;
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
                CharRect cr(content[i], charRects[i].rectPx + penPosition);
                linedCharRects.Back().PushBack(cr);
                penPosition.x += charAdvX * spacingMult.x;
            }
        }
        else // Just add them in a single line
        {
            CharRect cr(content[i], penPosition + charRects[i].rectPx);
            linedCharRects.Back().PushBack(cr);
            penPosition.x += charAdvX * spacingMult.x;
        }
    }
    return linedCharRects;
}

Vector2i TextFormatter::GetTextSizeOneLined(const String &content,
                                            const Font *font,
                                            const Vector2 &spacingMultiplier)
{
    Vector2 textSize = Vector2::Zero;
    for (int i = 0; i < content.Size(); ++i)
    {
        char c = content[i];
        Rectf cr = TextFormatter::GetCharRect(c, font);
        int charAdvX = GetCharAdvanceX(content, font, i);
        textSize.x += charAdvX * spacingMultiplier.x;
        // textSize.y =  Math::Max(textSize.y, cr.GetHeight());
    }
    textSize.y = font->GetLineSkip();
    return Vector2i( Vector2::Round(textSize) );
}

Vector2 FindMinCoord(const Array<TextFormatter::CharRect>&);
Vector2 FindMaxCoord(const Array<TextFormatter::CharRect>&);

void TextFormatter::ApplyAlignment(Array< Array<CharRect> > *linesCharRects,
                                   const Recti &limitsRect,
                                   const Font *font,
                                   HorizontalAlignment hAlignment,
                                   VerticalAlignment vAlignment)
{
    // For each line apply the HorizontalAlign
    for (Array<CharRect> &line : *linesCharRects)
    {
        if (line.IsEmpty()) { continue; }
        Vector2 lineMinCoord = FindMinCoord(line);
        Vector2 lineMaxCoord = FindMaxCoord(line);

        int lineHorizontalOffset = 0;
        if (hAlignment == HorizontalAlignment::Left)
        {
            lineHorizontalOffset = limitsRect.GetMin().x - lineMinCoord.x;
        }
        else if (hAlignment == HorizontalAlignment::Center)
        {
            lineHorizontalOffset = limitsRect.GetCenter().x -
                                   (lineMaxCoord.x + lineMinCoord.x) / 2;
        }
        else if (hAlignment == HorizontalAlignment::Right)
        {
            lineHorizontalOffset = limitsRect.GetMax().x - lineMaxCoord.x;
        }

        for (CharRect &cr : line)
        {
            cr.rectPx += Vector2(lineHorizontalOffset, 0);
        }
    }

    // Vertical align all the lines at once
    int textVerticalOffset = 0;
    const uint textHeight = (linesCharRects->Size() * font->GetLineSkip() );
    if (vAlignment == VerticalAlignment::Top)
    {
        textVerticalOffset = 0;
    }
    else if (vAlignment == VerticalAlignment::Center)
    {
        textVerticalOffset = limitsRect.GetSize().y / 2 - (textHeight/2);

    }
    else if (vAlignment == VerticalAlignment::Bot)
    {
        textVerticalOffset = limitsRect.GetSize().y - textHeight;
    }

    // Apply offsets
    for (Array<CharRect> &line : *linesCharRects)
    {
        for (CharRect &cr : line)
        {
            cr.rectPx += Vector2(0, textVerticalOffset);
        }
    }
}

Rectf TextFormatter::GetCharRect(char c, const Font *font)
{
    if (!font) { return Rectf::Zero; }

    Font::GlyphMetrics charMetrics = font->GetCharMetrics(c);

    Vector2 bearing(charMetrics.bearing.x, -charMetrics.bearing.y);
    Vector2 charMin(bearing);
    Vector2 charMax(bearing + charMetrics.size);
    return Rectf(charMin, charMax);
}

int TextFormatter::GetCharAdvanceX(const String &content,
                                   const Font *font,
                                   int currentCharIndex)
{
    int advance = 0;
    if (currentCharIndex < content.Size()-1)
    {
        advance = font->GetKerning(content[currentCharIndex],
                                      content[currentCharIndex + 1]);
    }

    if (advance <= 0)
    {
        const char c = content[currentCharIndex];
        Font::GlyphMetrics charMetrics = font->GetCharMetrics(c);
        advance = charMetrics.advance;
    }

    return advance + 1;
}

Vector2 FindMinCoord(const Array<TextFormatter::CharRect> &rects)
{
    Vector2 result;
    bool first = true;
    for (const TextFormatter::CharRect &cr : rects)
    {
        if (first) { first = false; result = cr.rectPx.GetMin(); }
        else { result = Vector2::Min(result, cr.rectPx.GetMin()); }
    }
    return result;
}

Vector2 FindMaxCoord(const Array<TextFormatter::CharRect> &rects)
{
    Vector2 result;
    bool first = true;
    for (const TextFormatter::CharRect &cr : rects)
    {
        if (first) { first = false; result = cr.rectPx.GetMax(); }
        else { result = Vector2::Max(result, cr.rectPx.GetMax()); }
    }
    return result;
}
