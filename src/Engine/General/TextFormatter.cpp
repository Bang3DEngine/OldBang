#include "Bang/TextFormatter.h"

#include "Bang/Font.h"
#include "Bang/String.h"
#include "Bang/Vector2.h"

Array<TextFormatter::CharRect>
   TextFormatter::GetFormattedTextPositions(const String &content,
                                            const Font *font,
                                            HorizontalAlignment hAlignment,
                                            VerticalAlignment vAlignment,
                                            WrapMode hWrapMode,
                                            WrapMode vWrapMode,
                                            int textSize,
                                            const Vector2i &_spacing,
                                            const Recti &limitsRect)
{
    if (content.Empty()) { return Array<CharRect>(); }

    // First create a list with all the character rects in the origin
    Array<CharRect> charRects;
    for (int i = 0; i < content.Length(); ++i)
    {
        const char c = content[i];
        Recti charRect = TextFormatter::GetCharRect(font, textSize, c);
        charRects.PushBack( CharRect(c, charRect) );
    }

    Vector2i spacing =
         SCAST<Vector2i>(G_Font::ScaleMagnitude(Vector2f(_spacing), textSize));

    Array< Array<CharRect> > linedCharRects;
    linedCharRects = SplitCharRectsInLines(content, font, charRects,
                                           limitsRect, spacing, textSize,
                                           hWrapMode);

    TextFormatter::ApplyAlignment(&linedCharRects, hAlignment,
                                  vAlignment, limitsRect);

    // Hide wrap must be applied after aligning...
    if (hWrapMode == WrapMode::Hide)
    {
        TextFormatter::ApplyHorizontalHideWrap(&linedCharRects, limitsRect);
    }

    if (vWrapMode == WrapMode::Hide)
    {
        TextFormatter::ApplyVerticalHideWrap(&linedCharRects, limitsRect);
    }

    Array<CharRect> newCharRects; // Flattened result
    for (const Array<CharRect> &line : linedCharRects)
    {
        newCharRects.Add(line);
    }
    return newCharRects;
}

Array< Array<TextFormatter::CharRect> >
        TextFormatter::SplitCharRectsInLines(const String &content,
                                             const Font *font,
                                             const Array<CharRect> &charRects,
                                             const Recti &limitsRect,
                                             const Vector2i &spacing,
                                             int textSize,
                                             WrapMode hWrapMode)
{
    Array< Array<CharRect> > linedCharRects(1); // Result
    if (hWrapMode == WrapMode::Wrap)
    {
        // Split the input char positions into the needed lines.
        // Each line will contain as many words as possible (split by spaces).
        Vector2i penPosition = limitsRect.GetMin();
        for (int i = 0; i < content.Length(); ++i)
        {
            const float charAdvX = GetCharAdvanceX(font, textSize, content, i);
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
                float tmpAdvX = penPosition.x + charAdvX + spacing.x;
                for (int j = i+1; j < content.Length(); ++j)
                {
                    if (content[j] == ' ') { break; }
                    const float jCharAdvX = GetCharAdvanceX(font, textSize,
                                                            content, j);
                    if (tmpAdvX + jCharAdvX > limitsRect.GetMax().x)
                    {
                        lineBreak = true;
                        break;
                    }
                    tmpAdvX += (jCharAdvX + spacing.x);
                }
            }
            lineBreak = lineBreak || content[i] == '\n';
            bool anticipatedLineBreak = ( (content[i] == ' ' && lineBreak) ||
                                           content[i] == '\n');

            if (lineBreak)
            {
                // Advance to next line! Add the current line to the result.
                penPosition.y -= spacing.y;
                penPosition.x  = limitsRect.GetMin().x;
                linedCharRects.Add( Array<CharRect>() );

                // Skip all next ' '
                if (content[i] == ' ')
                {
                    while (content[i] == ' ') { ++i; }
                    --i;
                }
            }

            if (!anticipatedLineBreak)
            {
                CharRect cr(content[i], charRects[i].rect + penPosition);
                linedCharRects.Back().PushBack(cr);
                penPosition.x += (charAdvX + spacing.x);
            }
        }
    }
    else // Just add them in a single line
    {
        Vector2i advance = limitsRect.GetMin();
        for (int i = 0; i < content.Length(); ++i)
        {
            const float charAdvX = GetCharAdvanceX(font, textSize, content, i);
            CharRect cr(content[i], charRects[i].rect + advance);
            linedCharRects.Back().PushBack(cr);
            advance.x += (charAdvX + spacing.x);
        }
    }

    return linedCharRects;
}

void TextFormatter::ApplyAlignment(Array< Array<CharRect> > *linesCharRects,
                                   HorizontalAlignment hAlignment,
                                   VerticalAlignment vAlignment,
                                   const Recti &limitsRect)
{
    // For each line save the HorizontalAlign it needs
    for (Array<CharRect> &line : *linesCharRects)
    {
        if (line.IsEmpty()) { continue; }
        Vector2i lineMinCoord = TextFormatter::FindMinCoord(line);
        Vector2i lineMaxCoord = TextFormatter::FindMaxCoord(line);

        float lineHorizontalOffset = 0.0f;
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

        for (int i = 0; i < line.Size(); ++i)
        {
            line[i].rect += Vector2i(lineHorizontalOffset, 0);
        }
    }

    // Vertical align all the lines at once
    float lineVerticalOffset = 0.0f;
    Vector2i textMinCoords = TextFormatter::FindMinCoord(*linesCharRects);
    Vector2i textMaxCoords = TextFormatter::FindMaxCoord(*linesCharRects);
    if (vAlignment == VerticalAlignment::Top)
    {
        lineVerticalOffset = limitsRect.GetMax().y - textMaxCoords.y;
    }
    else if (vAlignment == VerticalAlignment::Center)
    {
        lineVerticalOffset = limitsRect.GetCenter().y -
                             (textMaxCoords.y + textMinCoords.y) * 0.5f;
    }
    else if (vAlignment == VerticalAlignment::Bot)
    {
        lineVerticalOffset = limitsRect.GetMin().y - textMinCoords.y;
    }

    // Apply offsets
    for (Array<CharRect> &line : *linesCharRects)
    {
        for (int i = 0; i < line.Size(); ++i)
        {
            line[i].rect += Vector2i(0, lineVerticalOffset);
        }
    }
}

void TextFormatter::ApplyHorizontalHideWrap(
                        Array< Array<CharRect> > *linedCharRects,
                        const Recti &limitsRect)
{
    Array<CharRect> *line = &linedCharRects->Front();
    for (CharRect &cr : *line)
    {
        cr.visible = cr.visible &&
                     (cr.rect.GetMin().x >= limitsRect.GetMin().x &&
                      cr.rect.GetMax().x <= limitsRect.GetMax().x);
    }
}

void TextFormatter::ApplyVerticalHideWrap(
                        Array< Array<CharRect> > *linedCharRects,
                        const Recti &limitsRect)
{
    for (Array<CharRect> &line : *linedCharRects)
    {
        Vector2i minCoord = TextFormatter::FindMinCoord(line);
        Vector2i maxCoord = TextFormatter::FindMaxCoord(line);
        bool lineVisible = (minCoord.y >= limitsRect.GetMin().y &&
                            maxCoord.y <= limitsRect.GetMax().y);
        for (CharRect &cr : line) { cr.visible = cr.visible && lineVisible; }
    }
}

Recti TextFormatter::GetCharRect(const Font *font,
                                 int textSize,
                                 char c)
{
    if (!font) { return Recti::Zero; }

    Font::CharGlyphMetrics charMetrics = font->GetCharacterMetrics(c, textSize);

    Vector2i charMin(charMetrics.bearing.x,
                     charMetrics.bearing.y - charMetrics.size.y);
    Vector2i charMax(charMetrics.bearing.x + charMetrics.size.x,
                     charMetrics.bearing.y);
    return Recti(charMin, charMax);
}

int TextFormatter::GetCharAdvanceX(const Font *font,
                                   int textSize,
                                   const String &content,
                                   int currentCharIndex)
{
    const char c = content[currentCharIndex];
    Font::CharGlyphMetrics charMetrics = font->GetCharacterMetrics(c, textSize);
    if (c != ' ' && charMetrics.size.x > 0 )
    {
        return charMetrics.advance;
    }

    return font->GetCharacterMetrics('A', textSize).advance;
}

Vector2i TextFormatter::FindMinCoord(const Array<CharRect> &rects)
{
    Vector2i result;
    bool first = true;
    for (const CharRect &cr : rects)
    {
        if (first) { first = false; result = cr.rect.GetMin(); }
        else { result = Vector2i::Min(result, cr.rect.GetMin()); }
    }
    return result;
}

Vector2i TextFormatter::FindMaxCoord(const Array<CharRect> &rects)
{
    Vector2i result;
    bool first = true;
    for (const CharRect &cr : rects)
    {
        if (first) { first = false; result = cr.rect.GetMax(); }
        else { result = Vector2i::Max(result, cr.rect.GetMax()); }
    }
    return result;
}

Vector2i TextFormatter::FindMinCoord(const Array< Array<CharRect> > &rects)
{
    Vector2i result;
    bool first = true;
    for (const Array<CharRect> &line : rects)
    {
        if (first) { first = false; result = FindMinCoord(line); }
        else { result = Vector2i::Min(result, FindMinCoord(line)); }
    }
    return result;
}

Vector2i TextFormatter::FindMaxCoord(const Array< Array<CharRect> > &rects)
{
    Vector2i result;
    bool first = true;
    for (const Array<CharRect> &line : rects)
    {
        if (first) { first = false; result = FindMaxCoord(line); }
        else { result = Vector2i::Max(result, FindMaxCoord(line)); }
    }
    return result;
}
