#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL_ttf.h>

#include "Bang/Map.h"
#include "Bang/Asset.h"
#include "Bang/Vector2.h"
#include "Bang/Resource.h"

NAMESPACE_BANG_BEGIN

FORWARD class Texture2D;

class Font : public Asset
{
    ASSET(Font)

public:
    /**
     * @brief Structure to hold metrics for a character glyph.
     * They are all relative to the baseline.
     */
    struct GlyphMetrics
    {
        Vector2i size = Vector2i::Zero; // Size of character (of the actual char pixels)
        Vector2i bearing = Vector2i::Zero; // Offset from the baseline where the char pixels begin
        int advance  = 0; // Distance to be moved in X to right when drawing the next character
    };

    Font();
    virtual ~Font();

    void SetLoadSize(int loadSize);
    void SetMetricsSize(int metricsSize);

    int GetLoadSize() const;
    int GetMetricsSize() const;
    bool IsUsingDistanceField() const;
    Font::GlyphMetrics GetCharMetrics(char c) const;
    Vector2 GetCharMinUvInAtlas(char c) const;
    Vector2 GetCharMaxUvInAtlas(char c) const;
    bool HasCharacter(char c) const;
    Texture2D *GetAtlasTexture() const;
    int GetKerningXPx(char leftChar, char rightChar) const;
    int GetLineSkip() const;
    Vector2i GetSDFSpreadOffsetPx(char c) const;

    TTF_Font *GetTTFFont() const;

    float GetScaleProportion() const;
    int Scale(int magnitude, bool ceil) const;
    Vector2 Scale(const Vector2 &magnitude) const;
    Vector2i Scale(const Vector2i &magnitude) const;

    // Resource
    void Import(const Path &ttfFilepath) override;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    int m_metricsSize = 0;
    int m_ttfLoadSize = 128;
    TTF_Font *m_ttfFont = nullptr;
    bool m_usingDistanceField = false;
    Texture2D *m_atlasTexture = nullptr;

    Map<char, Vector2i> m_sdfSpreadOffsetPxInAtlas;
    Map<char, ::std::pair<Vector2, Vector2> > m_charUvsInAtlas;

    void Free();
};

NAMESPACE_BANG_END

#endif // FONT_H
