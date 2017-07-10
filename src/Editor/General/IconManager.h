#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include "Bang/WinUndef.h"

#include "Bang/Map.h"
#include "Bang/Paths.h"
#include "Bang/String.h"

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

    static const QImage& GetEmptyImage();
    static const QPixmap& GetEmptyPixmap();
    static const QImage& LoadImage(const Path &path,
                                   IconOverlay overlay = IconOverlay::None);
    static const QPixmap& LoadPixmap(const Path &path,
                                     IconOverlay overlay = IconOverlay::None);
    static const QPixmap& LoadMaterialPixmap(const Material *mat);

    static void InvalidatePixmap(Material *mat);
    static void InvalidatePixmap(const Path &path);

    static QPixmap CenterPixmapInEmptyPixmap(QPixmap& emptyPixmap,
                                             const QPixmap& pixmapToCenter);

    template<class Class>
    static const QPixmap& GetIcon();

private:
    typedef QPainter::CompositionMode CompMode;
    enum BlendMode
    {
        Over          = QPainter::CompositionMode_SourceOver,
        Blend          = QPainter::CompositionMode_SourceAtop,
        Multiplicative = QPainter::CompositionMode_Multiply
    };

    QPixmap m_emptyPixmap;
    QImage  m_emptyImage;

    QPixmap m_overlayAsset, m_overlayData;
    QImage m_materialBase;

    Map<String, QImage>  m_images;
    Map<String, QPixmap> m_pixmaps;

    static QImage AddImageOverlay(const QImage& img, IconOverlay overlay);
    static QImage BlendImages(const QImage &imgBase, const QImage &imgOver,
                              float imgOverOpacity, BlendMode blendMode);
    static QImage BlendColor(const QImage &imgBase, const Color &color,
                             BlendMode blendMode);

    static QImage SetQImageAlpha(const QImage &base, float alpha);

    static String GetStringId(const Path &path, IconOverlay overlay);

    static IconManager* GetInstance();
};

#endif // ICONMANAGER_H
