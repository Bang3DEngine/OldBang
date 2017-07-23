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
    static Array<Vector2> GetFormattedTextPositions(
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

    static Array< Array<Vector2> > GetLinedPositions(
                                const String &content,
                                const Font *font,
                                const Array<Vector2> &charQuadPositions,
                                const Rect &limitsRect,
                                const Vector2 &spacing,
                                int textSize,
                                WrapMode hWrapMode,
                                WrapMode vWrapMode);

    static void ApplyAlignment(
                        Array< Array<Vector2> > *linesCharQuadPositions,
                        HorizontalAlignment hAlignment,
                        VerticalAlignment vAlignment,
                        const Rect &limitsRect);


    static Vector2 GetTextSizeScaled(int textSize);
    static Rect GetCharRect(const Font *font,
                            int textSize,
                            char c);
    static float GetCharAdvanceX(const Font *font,
                                int textSize,
                                const String &content,
                                int currentCharIndex);

    template<class T, class FuncType>
    static T Find(const Array<T>& values, FuncType func)
    {
        T result;
        bool first = true;
        for (const T& v : values)
        {
            if (first) { first = false; result = v; }
            result = func(result, v);
        }
        return result;
    }
    template<class T, class FuncType>
    static T Find(const Array< Array<T> >& values, FuncType func)
    {
        T result;
        bool first = true;
        for (const Array<T> &arr : values)
        {
            for (const T& v : arr)
            {
                if (first) { first = false; result = v; }
                result = func(result, v);
            }
        }
        return result;
    }

    static Vector2 FindMin(const Array<Vector2>& values);
    static Vector2 FindMax(const Array<Vector2>& values);
    static Vector2 FindMin(const Array< Array<Vector2> >& values);
    static Vector2 FindMax(const Array< Array<Vector2> >& values);
};

#endif // TEXTFORMATTER_H
