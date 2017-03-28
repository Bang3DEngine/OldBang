#ifndef GRAPHICPIPELINEDEBUGGER_H
#define GRAPHICPIPELINEDEBUGGER_H

#include "Bang/GBuffer.h"

class GraphicPipelineDebugger
{
public:
    static String c_debugDir;
    static void Reset();
    static void TakeGBufferShot(GBuffer *gbuffer,
                                GBuffer::AttachmentId att,
                                const String &screenshotName);

private:
    GraphicPipelineDebugger();
};

#endif // GRAPHICPIPELINEDEBUGGER_H
