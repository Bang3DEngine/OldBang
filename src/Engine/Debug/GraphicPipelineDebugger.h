#ifndef GRAPHICPIPELINEDEBUGGER_H
#define GRAPHICPIPELINEDEBUGGER_H

#include "Bang/G_GBuffer.h"

class GraphicPipelineDebugger
{
public:
    static Path c_debugDir;
    static void Reset();
    static void TakeGBufferShot(G_GBuffer *gbuffer,
                                G_GBuffer::AttachmentId att,
                                const String &screenshotName);
    static void TakeGBufferShotStencil(G_GBuffer *gbuffer,
                                       const String &screenshotName);

private:
    GraphicPipelineDebugger();
};

#endif // GRAPHICPIPELINEDEBUGGER_H
