#ifndef GRAPHICPIPELINEDEBUGGER_H
#define GRAPHICPIPELINEDEBUGGER_H

#include "Bang/GBuffer.h"

NAMESPACE_BANG_BEGIN

class GEngineDebugger
{
public:
    static Path c_debugDir;
    static void Reset();
    static void TakeGBufferShot(GBuffer *gbuffer,
                                GL_Attachment att,
                                const String &screenshotName);
    static void TakeGBufferShotStencil(GBuffer *gbuffer,
                                       const String &screenshotName);

private:
    GEngineDebugger();
};

NAMESPACE_BANG_END

#endif // GRAPHICPIPELINEDEBUGGER_H
