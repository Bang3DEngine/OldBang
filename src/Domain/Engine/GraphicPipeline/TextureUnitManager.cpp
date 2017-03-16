#include "TextureUnitManager.h"

#include "Debug.h"
#include "Texture.h"
#include "GraphicPipeline.h"

TextureUnitManager::TextureUnitManager()
{
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &c_numTextureUnits);
}

TextureUnitManager::TexUnit TextureUnitManager::BindTexture(const Texture *tex)
{
    GraphicPipeline *gp = GraphicPipeline::GetActive();
    TextureUnitManager *tm = gp->GetTextureUnitManager();

    TexUnit unitToUse = 0;
    TexUnitMap::Iterator it = tm->m_textureToUnit.Find(tex);
    if (it != tm->m_textureToUnit.End())
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

        glActiveTexture(GL_TEXTURE0 + unitToUse);
        tex->Bind();
    }

    return unitToUse;
}

void TextureUnitManager::UnBindTexture(const Texture *tex)
{
    GraphicPipeline *gp = GraphicPipeline::GetActive();
    TextureUnitManager *tm = gp->GetTextureUnitManager();

    TexUnitMap::Iterator it = tm->m_textureToUnit.Find(tex);
    if (it != tm->m_textureToUnit.End())
    {
        const TexUnit unit = it->second;
        tm->m_textureToUnit.Remove(tex);

        glActiveTexture(GL_TEXTURE0 + unit);
        tex->UnBind();
    }
}
