#include "Bang/RenderSurface.h"

#include "Bang/GBuffer.h"
#include "Bang/SelectionFramebuffer.h"

USING_NAMESPACE_BANG

RenderSurface::RenderSurface()
{
    m_gbuffer = new GBuffer(1,1);
    m_selectionBuffer = new SelectionFramebuffer(1,1);
}

RenderSurface::RenderSurface(int width, int height) : RenderSurface()
{
    Resize(width, height);
}

RenderSurface::~RenderSurface()
{
    if (m_gbuffer) { delete m_gbuffer; }
    if (m_selectionBuffer) { delete m_selectionBuffer; }
}

void RenderSurface::Resize(int width, int height)
{
    m_gbuffer->Resize(width, height);
    m_selectionBuffer->Resize(width, height);
}

GBuffer *RenderSurface::GetGBuffer() const
{
    return m_gbuffer;
}

SelectionFramebuffer *RenderSurface::GetSelectionFramebuffer() const
{
    return m_selectionBuffer;
}
