#include "Bang/IconManager.h"

#include "Bang/Font.h"
#include "Bang/Mesh.h"
#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Light.h"
#include "Bang/Canvas.h"
#include "Bang/Prefab.h"
#include "Bang/UIText.h"
#include "Bang/UIImage.h"
#include "Bang/Material.h"
#include "Bang/Behaviour.h"
#include "Bang/Transform.h"
#include "Bang/Texture2D.h"
#include "Bang/AudioClip.h"
#include "Bang/PointLight.h"
#include "Bang/AudioSource.h"
#include "Bang/MeshRenderer.h"
#include "Bang/LineRenderer.h"
#include "Bang/ShaderProgram.h"
#include "Bang/AudioListener.h"
#include "Bang/RectTransform.h"
#include "Bang/CircleRenderer.h"
#include "Bang/SingletonManager.h"
#include "Bang/PostProcessEffect.h"

IconManager::IconManager() :
    m_emptyPixmap(16, 16),
    m_emptyImage(16, 16, QImage::Format_ARGB32)
{
    m_emptyPixmap.fill( Color::Zero.ToQColor() );
    m_emptyImage.fill( Color::Zero.ToQColor() );

    m_overlayAsset = QPixmap(
              EPATH("Icons/AssetDistinctor.png").GetAbsolute().ToQString());
    m_overlayData = QPixmap(
              EPATH("Icons/NoAssetDistinctor.png").GetAbsolute().ToQString());
    m_materialBase = QImage(
              EPATH("Icons/MaterialAssetIcon.png").GetAbsolute().ToQString());

    m_overlayAsset = m_overlayAsset.scaled(
                32, 32, Qt::KeepAspectRatio,
                Qt::TransformationMode::SmoothTransformation);

    m_overlayData = m_overlayData.scaled(
                32, 32, Qt::KeepAspectRatio,
                Qt::TransformationMode::SmoothTransformation);
}

const QImage &IconManager::GetEmptyImage()
{
    return IconManager::GetInstance()->m_emptyImage;
}

const QPixmap &IconManager::GetEmptyPixmap()
{
    return IconManager::GetInstance()->m_emptyPixmap;
}

IconManager *IconManager::GetInstance()
{
    if (!SingletonManager::Get<IconManager>())
    {
        SingletonManager::Set( new IconManager() );
    }
    return SingletonManager::Get<IconManager>();
}

const QImage &IconManager::LoadImage(const Path &path,
                                     IconManager::IconOverlay overlay)
{
    if (!path.IsFile()) { return IconManager::GetEmptyImage(); }

    IconManager *im = GetInstance();
    String id = IconManager::GetStringId(path, overlay);
    if (!im->m_pixmaps.ContainsKey(id))
    {
        QImage img = (overlay == IconOverlay::None) ?
                            QImage(path.GetAbsolute().ToQString())
                          : IconManager::LoadImage(path, IconOverlay::None);
        img = img.scaled(256, 256,
                         Qt::KeepAspectRatio,
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


const QPixmap& IconManager::LoadPixmap(const Path &path, IconOverlay overlay)
{
    if (!path.IsFile()) { return IconManager::GetEmptyPixmap(); }

    IconManager *im = GetInstance();
    IconManager::LoadImage(path, overlay);
    String id = IconManager::GetStringId(path, overlay);
    return im->m_pixmaps.Get(id);
}

void IconManager::InvalidatePixmap(Material *mat)
{
    IconManager::InvalidatePixmap(mat->GetFilepath());
}

void IconManager::InvalidatePixmap(const Path &path)
{
    IconManager *im = GetInstance();
    List<String> ids = im->m_images.GetKeys();
    for (const String &id : ids)
    {
        if ( id.Contains( path.GetAbsolute() ) )
        {
            im->m_images.Remove(id);
            im->m_pixmaps.Remove(id);
        }
    }
}

const QPixmap &IconManager::LoadMaterialPixmap(const Material *mat)
{
    IconManager *im = GetInstance();
    if (!mat) { return im->m_emptyPixmap; }

    Path matPath = mat->GetFilepath();
    String id = IconManager::GetStringId(matPath, IconOverlay::None);
    if (!im->m_pixmaps.ContainsKey(id))
    {
        QImage result = im->m_materialBase;

        const G_Texture2D *tex = mat->GetTexture();
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

    QPixmap *overlayPM = nullptr;
    if (overlay == IconOverlay::Asset) { overlayPM = &im->m_overlayAsset; }
    else if (overlay == IconOverlay::Data) { overlayPM = &im->m_overlayData; }

    const float pixmapAR      = float(overlayPM->width()) /
                                      overlayPM->height();
    const float overlaySize   = overlay == IconOverlay::Asset ? 0.4f : 0.3f;
    const float overlayWidth  = img.width()  * overlaySize;
    const float overlayHeight = overlayWidth / pixmapAR;
    const float overlayX      = img.width()  - overlayWidth;
    const float overlayY      = img.height() - overlayHeight;

    QImage result(img);
    QPainter painter;
    painter.begin(&result);
    painter.setCompositionMode( static_cast<CompMode>( BlendMode::Over ) );
    painter.drawPixmap(overlayX, overlayY, overlayWidth, overlayHeight,
                       *overlayPM);
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

const QPixmap &IconManager::GetIcon(const String &className)
{
    static bool init = false;
    static Map<String, String> classNames_pathStr;
    if (!init)
    {
        init = true;
        Map<String, String> *m = &classNames_pathStr;
        m->Set(AudioClip::GetClassNameStatic(), "Icons/AudioIcon.png");
        m->Set(Font::GetClassNameStatic(), "Icons/LetterIcon.png");
        m->Set(Material::GetClassNameStatic(), "Icons/MaterialAssetIcon.png");
        m->Set(Mesh::GetClassNameStatic(), "Icons/MeshAssetIcon.png");
        m->Set(Prefab::GetClassNameStatic(), "Icons/PrefabAssetIcon.png");
        m->Set(ShaderProgram::GetClassNameStatic(), "Icons/BehaviourIcon.png");
        m->Set(Texture2D::GetClassNameStatic(), "Icons/ImageIcon.png");
        m->Set(AudioListener::GetClassNameStatic(), "Icons/AudioIcon.png");
        m->Set(AudioSource::GetClassNameStatic(), "Icons/AudioIcon.png");
        m->Set(Behaviour::GetClassNameStatic(), "Icons/BehaviourIcon.png");
        m->Set(Camera::GetClassNameStatic(), "Icons/CameraIcon.png");
        m->Set(Canvas::GetClassNameStatic(), "Icons/RectTransformIcon.png");
        m->Set(CircleRenderer::GetClassNameStatic(), "Icons/eye.png");
        m->Set(Light::GetClassNameStatic(), "Icons/PointLightIcon.png");
        m->Set(LineRenderer::GetClassNameStatic(), "Icons/eye.png");
        m->Set(MeshRenderer::GetClassNameStatic(), "Icons/eye.png");
        m->Set(PointLight::GetClassNameStatic(), "Icons/PointLightIcon.png");
        m->Set(PostProcessEffect::GetClassNameStatic(), "Icons/ImageIcon.png");
        m->Set(RectTransform::GetClassNameStatic(), "Icons/RectTransformIcon.png");
        m->Set(Renderer::GetClassNameStatic(), "Icons/eye.png");
        m->Set(Transform::GetClassNameStatic(), "Icons/TransformIcon.png");
        m->Set(UIImage::GetClassNameStatic(), "Icons/ImageIcon.png");
        m->Set(UIText::GetClassNameStatic(), "Icons/LetterIcon.png");
    }

    if (classNames_pathStr.ContainsKey(className))
    {
        return IconManager::LoadPixmap( EPATH(classNames_pathStr[className]) );
    }
    return IconManager::GetInstance()->m_emptyPixmap;
}

String IconManager::GetStringId(const Path &path,
                                IconManager::IconOverlay overlay)
{
    return path.GetAbsolute() + "|" + String(overlay);
}

