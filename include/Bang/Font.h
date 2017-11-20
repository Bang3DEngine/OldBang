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
        Vector2f size    = Vector2f::Zero;
        Vector2f bearing = Vector2f::Zero;
        float    advance = 0;
    };

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
    float GetKerning(char leftChar, char rightChar) const;
    float GetLineSkip() const;
    float GetFontAscent() const;
    float GetFontDescent() const;
    float GetFontHeight() const;
    Vector2i GetSDFSpreadOffsetPx(char c) const;

    TTF_Font *GetTTFFont() const;

    float GetScaleProportion() const;
    float Scale(float magnitude) const;
    Vector2 Scale(const Vector2 &magnitude) const;

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
    Map<char, std::pair<Vector2, Vector2> > m_charUvsInAtlas;

    Font();
    virtual ~Font();

    void Free();
};

NAMESPACE_BANG_END

#endif // FONT_H
