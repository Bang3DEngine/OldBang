#include "IconManager.h"

#include "IO.h"
#include "Debug.h"
#include "Material.h"
#include "Texture2D.h"
#include "SingletonManager.h"

IconManager::IconManager()
{
    m_overlayAsset = QPixmap(
              IO::ToAbsolute("Icons/AssetDistinctor.png", true).ToQString());
    m_overlayData = QPixmap(
              IO::ToAbsolute("Icons/NoAssetDistinctor.png", true).ToQString());

    m_overlayAsset = m_overlayAsset.scaled(
                32, 32, Qt::KeepAspectRatio,
                Qt::TransformationMode::SmoothTransformation);

    m_overlayData = m_overlayData.scaled(
                32, 32, Qt::KeepAspectRatio,
                Qt::TransformationMode::SmoothTransformation);
}

IconManager *IconManager::GetInstance()
{
    if (!SingletonManager::Get<IconManager>())
    {
        SingletonManager::Set( new IconManager() );
    }
    return SingletonManager::Get<IconManager>();
}

const QPixmap& IconManager::LoadPixmap(const String &path,
                                       IconOverlay overlay,
                                       bool isEnginePath)
{
    IconManager *im = GetInstance();
    String absPath = IO::ToAbsolute(path, isEnginePath);
    String id = IconManager::GetStringId(absPath, overlay);
    if (!im->m_pixmaps.ContainsKey(id))
    {
        QPixmap pm(absPath.ToQString());
        if (overlay != IconOverlay::None)
        {
            pm = AddPixmapOverlay(pm, overlay);
        }
        im->m_pixmaps.Set(id, pm);
    }
    return im->m_pixmaps.Get(id);
}

void IconManager::InvalidatePixmap(Material *mat)
{
    IconManager::InvalidatePixmap(mat->GetFilepath());
}

void IconManager::InvalidatePixmap(const String &absPath)
{
    IconManager *im = GetInstance();
    List<String> ids = im->m_pixmaps.GetKeys();
    for (const String &id : ids)
    {
        if ( id.Contains(absPath) )
        {
            im->m_pixmaps.Remove(id);
        }
    }
}

const QPixmap &IconManager::LoadMaterialPixmap(const Material *mat)
{
    IconManager *im = GetInstance();
    String matPath = mat->GetFilepath();
    String id = IconManager::GetStringId(matPath, IconOverlay::None);
    if (!im->m_pixmaps.ContainsKey(id))
    {
        QPixmap result =
            IconManager::LoadPixmap("Icons/MaterialAssetIcon.png",
                                    IconOverlay::None, true);

        const Texture2D *tex = mat->GetTexture();
        if (tex)
        {
            const QPixmap& texturePixmap =
                    IconManager::LoadPixmap(tex->GetImageFilepath(),
                                            IconOverlay::None);
            result = IconManager::BlendPixmaps(result, texturePixmap, 0.6f,
                                               BlendMode::Blend);
        }

        //result = IconManager::BlendColor(result, mat->GetDiffuseColor(),
        //                                 BlendMode::Multiplicative);
        result =  IconManager::AddPixmapOverlay(result, IconOverlay::Asset);

        im->m_pixmaps.Set(id, result);
    }

    return im->m_pixmaps.Get(id);
}


QPixmap IconManager::AddPixmapOverlay(const QPixmap& pixmap, IconOverlay overlay)
{
    IconManager *im = IconManager::GetInstance();

    QPixmap *overlayPM = nullptr;
    if (overlay == IconOverlay::Asset) { overlayPM = &im->m_overlayAsset; }
    else if (overlay == IconOverlay::Data) { overlayPM = &im->m_overlayData; }

    const float pixmapAR      = float(overlayPM->width()) / overlayPM->height();
    const float overlaySize   = overlay == IconOverlay::Asset ? 0.4f : 0.3f;
    const float overlayWidth  = pixmap.width()  * overlaySize;
    const float overlayHeight = overlayWidth / pixmapAR;
    const float overlayX      = pixmap.width()  - overlayWidth;
    const float overlayY      = pixmap.height() - overlayHeight;

    QPixmap result(pixmap);
    QPainter painter;
    painter.begin(&result);
    painter.setCompositionMode( static_cast<CompMode>( BlendMode::Over ) );
    painter.drawPixmap(overlayX, overlayY, overlayWidth, overlayHeight,
                       *overlayPM);
    painter.end();
    return result;
}

QPixmap IconManager::SetQPixmapAlpha(const QPixmap &base, float alpha)
{
    QImage image(base.size(), QImage::Format_ARGB32_Premultiplied);

    image.fill(Qt::transparent);
    QPainter painter(&image);
    painter.setOpacity(alpha);
    painter.drawPixmap(0, 0, base);
    painter.end();

    return QPixmap::fromImage(image);
}

QPixmap IconManager::BlendPixmaps(const QPixmap &pmBase, const QPixmap &pmOver,
                                  float pmOverOpacity, BlendMode blendMode)
{
    QPixmap result(pmBase);
    QPixmap over = IconManager::SetQPixmapAlpha(pmOver, pmOverOpacity);
    over = over.scaled(pmBase.width(), pmBase.height());

    QPainter painter;
    painter.begin(&result);
    painter.setCompositionMode( static_cast<CompMode>(blendMode) );
    painter.drawPixmap(0, 0, over.width(), over.height(), over);
    painter.end();

    return result;
}

QPixmap IconManager::BlendColor(const QPixmap &pmBase, const Color &color,
                                BlendMode blendMode)
{
    QImage img(1, 1, QImage::Format_ARGB32_Premultiplied);
    img.fill( color.ToQColor() );
    QPixmap colorPM = QPixmap::fromImage(img);
    return BlendPixmaps(pmBase, colorPM, color.a, blendMode);
}

QPixmap IconManager::CenterPixmapInEmptyPixmap(QPixmap& emptyPixmap,
                                               const QPixmap& pixmapToCenter)
{
    QPainter painter;
    painter.begin(&emptyPixmap);
    painter.setCompositionMode( static_cast<CompMode>(BlendMode::Over) );
    painter.drawPixmap(emptyPixmap.width()  / 2 - pixmapToCenter.width()  / 2,
                       emptyPixmap.height() / 2 - pixmapToCenter.height() / 2,
                       pixmapToCenter.width(), pixmapToCenter.height(),
                       pixmapToCenter);
    painter.end();
    return emptyPixmap;
}

String IconManager::GetStringId(const String &absPath,
                                IconManager::IconOverlay overlay)
{
    return absPath + "|" + String(overlay);
}

