#include "Bang/ImageIODDS.h"

#include "Bang/GL.h"
#include "Bang/nv_dds.h"

USING_NAMESPACE_BANG

void ImageIODDS::ImportDDS(const Path &filepath, Texture2D *tex, bool *ok)
{
    nv_dds::CDDSImage ddsImg;
    ddsImg.load(filepath.GetAbsolute(), true);

    GLId prevId = GL::GetBoundId(tex->GetGLBindTarget());
    tex->Bind();

    tex->CreateEmpty(ddsImg.get_width(), ddsImg.get_height());
    ddsImg.upload_texture2D(0, GLCAST( tex->GetGLBindTarget() ));
    tex->SetInternalFormat( SCAST<GL::ColorFormat>(ddsImg.get_format()) );

    GL::Bind(tex->GetGLBindTarget(), prevId);

    if (ok) { *ok = true; }
}
