#include "Bang/TextureUnitManager.h"

#include "Bang/Texture.h"
#include "Bang/GraphicPipeline.h"

USING_NAMESPACE_BANG

TextureUnitManager::TextureUnitManager()
{
    c_numTextureUnits = GL::GetInteger(GL_MAX_TEXTURE_IMAGE_UNITS);
}

TextureUnitManager::TexUnit TextureUnitManager::BindTexture(const GLId texId)
{
    GraphicPipeline *gp = GraphicPipeline::GetActive();
    TextureUnitManager *tm = gp->GetTextureUnitManager();

    TexUnit unitToUse = 0;
    TexUnitMap::Iterator it = tm->m_textureIdToUnit.Find(texId);
    if (it != tm->m_textureIdToUnit.End())
    {
        // It was still bound, reuse it (just return where it is)
        unitToUse = it->second;
    }
    else // We need to bind the new texture to the older used texture unit
    {
        if (tm->m_usedUnits.size() < tm->c_numTextureUnits)
        {
            unitToUse = tm->m_usedUnits.size() + 1;
        }
        else
        {   // Reuse oldest texture unit
            unitToUse = tm->m_usedUnits.front();
            tm->m_usedUnits.pop();
        }
        tm->m_usedUnits.push(unitToUse);

        GL::ActiveTexture(GL_TEXTURE0 + unitToUse);
        GL::Bind(GL::BindTarget::Texture2D, texId);
    }

    return unitToUse;
}

TextureUnitManager::TexUnit TextureUnitManager::BindTexture(const Texture *tex)
{
    return TextureUnitManager::BindTexture(tex->GetGLId());
}

void TextureUnitManager::UnBindTexture(const Texture *tex)
{
    TextureUnitManager::UnBindTexture(tex->GetGLId());
}

void TextureUnitManager::UnBindTexture(GLId textureId)
{
    GraphicPipeline *gp = GraphicPipeline::GetActive();
    TextureUnitManager *tm = gp->GetTextureUnitManager();

    TexUnitMap::Iterator it = tm->m_textureIdToUnit.Find(textureId);
    if (it != tm->m_textureIdToUnit.End())
    {
        const TexUnit unit = it->second;
        tm->m_textureIdToUnit.Remove(textureId);

        GL::ActiveTexture(GL_TEXTURE0 + unit);
        GL::UnBind(GL::BindTarget::Texture2D);
    }
}
