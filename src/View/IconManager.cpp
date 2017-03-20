#include "IconManager.h"

#include "IO.h"
#include "Debug.h"
#include "Material.h"
#include "Texture2D.h"
#include "SingletonManager.h"

IconManager::IconManager()
{
    m_overlayAsset = QImage(
              IO::ToAbsolute("Icons/AssetDistinctor.png", true).ToQString());
    m_overlayData = QImage(
              IO::ToAbsolute("Icons/NoAssetDistinctor.png", true).ToQString());
    m_materialBase = QImage(
              IO::ToAbsolute("Icons/MaterialAssetIcon.png", true).ToQString());

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

const QImage &IconManager::LoadImage(const String &absPath,
                                     IconManager::IconOverlay overlay)
{
    IconManager *im = GetInstance();
    String id = IconManager::GetStringId(absPath, overlay);
    if (!im->m_pixmaps.ContainsKey(id))
    {
        QImage img = (overlay == IconOverlay::None) ?
                            QImage(absPath.ToQString())
                          : IconManager::LoadImage(absPath, IconOverlay::None);
        img = img.scaled(256, 256, Qt::KeepAspectRatio,
                         Qt::TransformationMode::SmoothTransformation);

        if (overlay != IconOverlay::None)
        {
            img = AddImageOverlay(img, overlay);
        }

        im->m_images.Set(id, img);
        im->m_pixmaps.Set(id, QPixmap::fromImage(img));
    }
    return im->m_images.Get(id);
}


const QPixmap& IconManager::LoadPixmap(const String &absPath,
                                       IconOverlay overlay)
{
    IconManager *im = GetInstance();
    IconManager::LoadImage(absPath, overlay);
    String id = IconManager::GetStringId(absPath, overlay);
    return im->m_pixmaps.Get(id);
}

void IconManager::InvalidatePixmap(Material *mat)
{
    IconManager::InvalidatePixmap(mat->GetFilepath());
}

void IconManager::InvalidatePixmap(const String &absPath)
{
    IconManager *im = GetInstance();
    List<String> ids = im->m_images.GetKeys();
    for (const String &id : ids)
    {
        if ( id.Contains(absPath) )
        {
            im->m_images.Remove(id);
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
        QImage result = im->m_materialBase;

        const Texture2D *tex = mat->GetTexture();
        if (tex)
        {
            const QImage& textureImg =
                    IconManager::LoadImage(tex->GetImageFilepath(),
                                           IconOverlay::None);
            result = IconManager::BlendImages(result, textureImg, 0.6f,
                                              BlendMode::Blend);
        }

        result = IconManager::BlendColor(result, mat->GetDiffuseColor(),
                                         BlendMode::Multiplicative);
        result =  IconManager::AddImageOverlay(result, IconOverlay::Asset);

        im->m_images.Set(id, result);
        im->m_pixmaps.Set( id, QPixmap::fromImage(result) );
    }

    return im->m_pixmaps.Get(id);
}

QImage IconManager::AddImageOverlay(const QImage& img, IconOverlay overlay)
{
    IconManager *im = IconManager::GetInstance();

    QImage *overlayImg = nullptr;
    if (overlay == IconOverlay::Asset) { overlayImg = &im->m_overlayAsset; }
    else if (overlay == IconOverlay::Data) { overlayImg = &im->m_overlayData; }

    const float pixmapAR      = float(overlayImg->width()) /
                                      overlayImg->height();
    const float overlaySize   = overlay == IconOverlay::Asset ? 0.4f : 0.3f;
    const float overlayWidth  = img.width()  * overlaySize;
    const float overlayHeight = overlayWidth / pixmapAR;
    const float overlayX      = img.width()  - overlayWidth;
    const float overlayY      = img.height() - overlayHeight;

    QImage result(img);
    QPainter painter;
    painter.begin(&result);
    painter.setCompositionMode( static_cast<CompMode>( BlendMode::Over ) );
    painter.drawImage(overlayX, overlayY, *overlayImg,
                      overlayWidth, overlayHeight);
    painter.end();
    return result;
}

QImage IconManager::SetQImageAlpha(const QImage &base, float alpha)
{
    QImage image(base.size(), QImage::Format_ARGB32);

    image.fill(Qt::transparent);
    QPainter painter(&image);
    painter.setOpacity(alpha);
    painter.drawImage(0, 0, base);
    painter.end();

    return image;
}

QImage IconManager::BlendImages(const QImage &imgBase, const QImage &imgOver,
                                float imgOverOpacity, BlendMode blendMode)
{
    QImage result(imgBase);
    QImage over = IconManager::SetQImageAlpha(imgOver, imgOverOpacity);
    over = over.scaled(imgBase.width(), imgBase.height());

    if (blendMode == BlendMode::Multiplicative)
    {
        for (int i = 0; i < result.height(); ++i)
        {
            for (int j = 0; j < result.width(); ++j)
            {
                Color srcColor  = Color::FromQColor( result.pixel(j, i) );
                srcColor.a = qAlpha(result.pixel(j, i)) / 255.0f;
                Color overColor = Color::FromQColor( over.pixel(j, i) );

                QColor totalColor = (srcColor * overColor).ToQColor();
                result.setPixel(j, i, qRgba(totalColor.red(),
                                            totalColor.green(),
                                            totalColor.blue(),
                                            totalColor.alpha()));
            }
        }
    }
    else
    {
        QPainter painter;
        painter.begin(&result);
        painter.setCompositionMode( static_cast<CompMode>(blendMode) );
        painter.drawImage(0, 0, over);
        painter.end();
    }
    return result;
}

QImage IconManager::BlendColor(const QImage &imgBase, const Color &color,
                                BlendMode blendMode)
{
    QImage colorImg(1, 1, QImage::Format_ARGB32);
    colorImg.fill( color.ToQColor() );
    return BlendImages(imgBase, colorImg, color.a, blendMode);
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

