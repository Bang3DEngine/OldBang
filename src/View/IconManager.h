#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QIcon>
#include <QPixmap>
#include <QPainter>

#include "Map.h"
#include "String.h"
#include "NamedEnum.h"

class Material;

/**
 * @brief Loads Pixmaps/Icons and mantains a cache to avoid reloading them.
 */
class IconManager
{
public:
    enum IconOverlay
    {
        None = 0,
        Asset,
        Data
    };

    IconManager();

    static const QPixmap& LoadPixmap(const String &path,
                                     IconOverlay overlay = IconOverlay::None,
                                     bool isEnginePath = false);
    static const QPixmap& LoadMaterialPixmap(const Material *mat);

    static void InvalidatePixmap(Material *mat);
    static void InvalidatePixmap(const String &absPath);

    static QPixmap CenterPixmapInEmptyPixmap(QPixmap& emptyPixmap,
                                             const QPixmap& pixmapToCenter);

private:
    typedef QPainter::CompositionMode CompMode;
    enum BlendMode
    {
        Over          = QPainter::CompositionMode_SourceOver,
        Blend          = QPainter::CompositionMode_SourceAtop,
        Multiplicative = QPainter::CompositionMode_Multiply
    };

    QPixmap m_overlayAsset, m_overlayData;

    Map<String, QPixmap> m_pixmaps;

    static QPixmap AddPixmapOverlay(const QPixmap& pm, IconOverlay overlay);
    static QPixmap BlendPixmaps(const QPixmap &pmBase, const QPixmap &pmOver,
                                float pmOverOpacity, BlendMode blendMode);
    static QPixmap BlendColor(const QPixmap &pmBase, const Color &color,
                              BlendMode blendMode);

    static QPixmap SetQPixmapAlpha(const QPixmap &base, float alpha);

    static String GetStringId(const String &absPath, IconOverlay overlay);

    static IconManager* GetInstance();
};

#endif // ICONMANAGER_H
