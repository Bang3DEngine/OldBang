#ifndef ICONMANAGER_H
#define ICONMANAGER_H

#include <QIcon>
#include <QPixmap>

#include "Map.h"
#include "String.h"
#include "NamedEnum.h"

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
    static const QIcon &LoadIcon(const String &path,
                                 IconOverlay overlay = IconOverlay::None,
                                 bool isEnginePath = false);

private:
    QPixmap m_overlayAsset, m_overlayData;

    Map<String, QPixmap> m_pixmaps;
    Map<String, QIcon> m_icons;

    static QPixmap AddIconOverlay(const QPixmap& pm, IconOverlay overlay);
    static QPixmap CenterPixmapInEmptyPixmap(QPixmap& emptyPixmap,
                                             const QPixmap& pixmapToCenter);

    static String GetStringId(const String &absPath, IconOverlay overlay);

    static IconManager* GetInstance();
};

#endif // ICONMANAGER_H
