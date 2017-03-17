#include "IconManager.h"

#include <QPainter>

#include "IO.h"
#include "Debug.h"
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
            pm = AddIconOverlay(pm, overlay);
        }
        im->m_pixmaps.Set(id, pm);
    }
    return im->m_pixmaps.Get(id);
}

const QIcon &IconManager::LoadIcon(const String &path,
                                   IconOverlay overlay,
                                   bool isEnginePath)
{
    IconManager *im = GetInstance();
    String absPath = IO::ToAbsolute(path, isEnginePath);
    String id = IconManager::GetStringId(absPath, overlay);
    if (!im->m_icons.ContainsKey(id))
    {
        const QPixmap& pixmap =
                IconManager::LoadPixmap(path, overlay, isEnginePath);
        im->m_icons.Set(id, pixmap);
    }
    return im->m_icons.Get(id);
}

QPixmap IconManager::AddIconOverlay(const QPixmap& pixmap, IconOverlay overlay)
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
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(overlayX, overlayY, overlayWidth, overlayHeight,
                       *overlayPM);
    painter.end();
    return result;
}

QPixmap IconManager::CenterPixmapInEmptyPixmap(QPixmap& emptyPixmap,
                                               const QPixmap& pixmapToCenter)
{
    QPainter painter;
    painter.begin(&emptyPixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
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

