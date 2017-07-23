#include "Bang/TextFormatter.h"

#include "Bang/Debug.h"
#include "Bang/Screen.h"

Array<Vector2> TextFormatter::GetFormattedTextPositions(
                                    const String &content,
                                    const Font *font,
                                    HorizontalAlignment hAlignment,
                                    VerticalAlignment vAlignment,
                                    WrapMode hWrapMode,
                                    WrapMode vWrapMode,
                                    int textSize,
                                    const Vector2 &spacing,
                                    const Rect &limitsRectNDC)
{
    if (content.Empty()) { return Array<Vector2>(); }

    // First create a list with all the character rects in the origin
    Array<Vector2> charQuadPositions;
    for (int i = 0; i < content.Length(); ++i)
    {
        const char c = content[i];
        Rect charRect = TextFormatter::GetCharRect(font, textSize, c);
        charQuadPositions.PushBack( charRect.GetBotLeft()  );
        charQuadPositions.PushBack( charRect.GetBotRight() );
        charQuadPositions.PushBack( charRect.GetTopRight() );
        charQuadPositions.PushBack( charRect.GetTopLeft()  );
    }

    Array< Array<Vector2> > linedCharQuads;
    linedCharQuads = GetLinedPositions(content,
                                       font,
                                       charQuadPositions,
                                       limitsRectNDC,
                                       spacing,
                                       textSize,
                                       hWrapMode,
                                       vWrapMode);

    TextFormatter::ApplyAlignment(&linedCharQuads, hAlignment,
                                  vAlignment, limitsRectNDC);

    Array<Vector2> newCharQuadPositions; // Flattened result
    for (const Array<Vector2> &line : linedCharQuads)
    {
        newCharQuadPositions.Add(line);
    }
    return newCharQuadPositions;
}

Array< Array<Vector2> > TextFormatter::GetLinedPositions(
                                    const String &content,
                                    const Font *font,
                                    const Array<Vector2> &charQuadPositions,
                                    const Rect &limitsRect,
                                    const Vector2 &spacing,
                                    int textSize,
                                    WrapMode hWrapMode,
                                    WrapMode vWrapMode)
{
    Array< Array<Vector2> > linedCharQuadPositions(1); // Result

    Vector2 advance = limitsRect.GetMin();
    if (hWrapMode == WrapMode::Wrap)
    {
        // Split the input char positions into the needed lines.
        // Each line will contain as many words as possible (split by spaces).
        float currentLineLength = 0.0f;
        const Vector2 textSizeScaled = GetTextSizeScaled(textSize);
        const Vector2 spacingNDC = spacing * textSizeScaled;
        const float rectWidth = limitsRect.GetWidth();
        for (int i = 0; i < content.Length(); ++i)
        {
            const float charAdvX = GetCharAdvanceX(font, textSize, content, i);
            currentLineLength += charAdvX;

            // We have arrived to a space.
            // Does the following word (after this space) still fits in
            // the current line?
            bool lineBreak = false;
            if (content[i] == ' ')
            {
                float tmpCurrentLineLength = currentLineLength;
                for (int j = i+1; j < content.Length(); ++j)
                {
                    if (content[j] == ' ') { break; }

                    tmpCurrentLineLength += GetCharAdvanceX(font, textSize,
                                                            content, j);

                    if (tmpCurrentLineLength >= rectWidth)
                    {
                        lineBreak = true;
                        break;
                    }
                }

                if (lineBreak)
                {
                    // Advance to next line! Add the current line to the result.
                    currentLineLength = 0.0f;
                    advance.y -= spacingNDC.y;
                    advance.x  = limitsRect.GetMin().x;
                    linedCharQuadPositions.Add( Array<Vector2>() );
                }
            }

            // Add the quad char positions into its line (with shifted y)
            for (int k = 0; k < 4; ++k)
            {
                linedCharQuadPositions.Back().PushBack(
                            charQuadPositions[i*4 + k] + advance);
            }
            if (!lineBreak) { advance.x += charAdvX + spacingNDC.x; }
        }
    }
    else // Hide or Overflow
    {
        for (int i = 0; i < content.Length(); ++i)
        {
            const Vector2& iMaxCoord = charQuadPositions[i*4 + 2];
            if (hWrapMode == WrapMode::Hide &&
                iMaxCoord.x + advance.x > limitsRect.GetMax().x)
            {
                break; // If it does not fit, dont add more characters
            }
            else
            {
                // If the it fits in the line add them
                advance.x += TextFormatter::GetCharAdvanceX(font, textSize,
                                                            content, i);
                for (int k = 0; k < 4; ++k)
                {
                    linedCharQuadPositions.Back().PushBack(
                                charQuadPositions[i*4 + k] + advance);
                }
            }
        }
    }


    // Apply vertical wrapping
    if (vWrapMode == WrapMode::Hide)
    {
        auto it = linedCharQuadPositions.Begin();
        while (it != linedCharQuadPositions.End())
        {
            const Array<Vector2> &line = *it;
            Vector2 minCoord = FindMin(line);
            Vector2 maxCoord = FindMax(line);
            if (minCoord.y < limitsRect.GetMin().y ||
                maxCoord.y > limitsRect.GetMax().y)
            {
                it = linedCharQuadPositions.Remove(it);
            }
            else { ++it; }
        }
    }

    return linedCharQuadPositions;
}

void TextFormatter::ApplyAlignment(
                            Array< Array<Vector2> > *linesCharQuadPositions,
                            HorizontalAlignment hAlignment,
                            VerticalAlignment vAlignment,
                            const Rect &limitsRect)
{
    // For each line save the HorizontalAlign it needs
    for (Array<Vector2> &line : *linesCharQuadPositions)
    {
        if (line.IsEmpty()) { continue; }
        Vector2 lineMinCoord = TextFormatter::FindMin(line);
        Vector2 lineMaxCoord = TextFormatter::FindMax(line);

        float lineHorizontalOffset;
        if (hAlignment == HorizontalAlignment::Left)
        {
            lineHorizontalOffset = limitsRect.GetMin().x - lineMinCoord.x;
        }
        else if (hAlignment == HorizontalAlignment::Center)
        {
            lineHorizontalOffset = limitsRect.GetCenter().x -
                                   (lineMaxCoord.x + lineMinCoord.x) * 0.5f;
        }
        else if (hAlignment == HorizontalAlignment::Right)
        {
            lineHorizontalOffset = limitsRect.GetMax().x - lineMaxCoord.x;
        }

        for (int i = 0; i < line.Size(); ++i)
        {
            line.at(i).x += lineHorizontalOffset;
        }
    }

    // Vertical align all the lines at once
    float lineVerticalOffset;
    Vector2 textMinCoords = TextFormatter::FindMin(*linesCharQuadPositions);
    Vector2 textMaxCoords = TextFormatter::FindMax(*linesCharQuadPositions);
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
    for (Array<Vector2> &line : *linesCharQuadPositions)
    {
        for (int i = 0; i < line.Size(); ++i)
        {
            line[i].y += lineVerticalOffset;
        }
    }
}

Vector2 TextFormatter::GetTextSizeScaled(int textSize)
{
    constexpr double scaleFactor = 0.00015;
    return Vector2(textSize * scaleFactor) /
           Vector2(Screen::GetAspectRatio(), 1.0f);
}

Rect TextFormatter::GetCharRect(const Font *font,
                                int textSize,
                                char c)
{
    if (!font) { return Rect::Empty; }

    const Vector2 textSizeNDC = GetTextSizeScaled(textSize);
    Font::CharGlyphMetrics charMetrics = font->GetCharacterMetrics(c);

    Vector2 charSize =
            Vector2(charMetrics.width, charMetrics.height) * textSizeNDC;

    const Vector2 bearing =
            Vector2(charMetrics.bearingX, charMetrics.bearingY) * textSizeNDC;

    Vector2 charMin(bearing.x, bearing.y - charSize.y);
    Vector2 charMax(bearing.x + charSize.x, bearing.y);
    return Rect(charMin, charMax);
}

float TextFormatter::GetCharAdvanceX(const Font *font,
                                     int textSize,
                                     const String &content,
                                     int currentCharIndex)
{
    int advancePx = -1;

    const int i = currentCharIndex;
    const char current = content[i];

    // TODO: Kerning
    // if (m_kerning && next != '\0')
    //{
    //    const char nextChar = i < content.Length() - 1 ? content[i + 1] : '\0';
    //    float kernX = m_font->GetKerningX(current, nextChar);
    //    if (kernX > 5) { advancePx = kernX; }
    //}

    Font::CharGlyphMetrics charMetrics = font->GetCharacterMetrics(current);
    if (current == ' ' || charMetrics.width <= 0 ) { advancePx = 100; }
    if (advancePx < 0)
    {
        advancePx = charMetrics.advance;
    }
    return advancePx * GetTextSizeScaled(textSize).x;
}

Vector2 TextFormatter::FindMin(const Array<Vector2> &values)
{
    return TextFormatter::Find<Vector2>(values, Vector2::Min);
}

Vector2 TextFormatter::FindMax(const Array<Vector2> &values)
{
    return TextFormatter::Find(values, Vector2::Max);
}

Vector2 TextFormatter::FindMin(const Array<Array<Vector2> > &values)
{
    return TextFormatter::Find(values, Vector2::Min);
}

Vector2 TextFormatter::FindMax(const Array<Array<Vector2> > &values)
{
    return TextFormatter::Find(values, Vector2::Max);
}
