#ifndef GRAPHICPIPELINEDEBUGGER_H
#define GRAPHICPIPELINEDEBUGGER_H

#include "Bang/GBuffer.h"

class GraphicPipelineDebugger
{
public:
    static Path c_debugDir;
    static void Reset();
    static void TakeGBufferShot(GBuffer *gbuffer,
                                GL::Attachment att,
                                const String &screenshotName);
    static void TakeGBufferShotStencil(GBuffer *gbuffer,
                                       const String &screenshotName);

private:
    GraphicPipelineDebugger();
};

#endif // GRAPHICPIPELINEDEBUGGER_H
