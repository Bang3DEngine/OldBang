#include "Bang/GraphicPipelineDebugger.h"

#include "Bang/Paths.h"
#include "Bang/File.h"
#include "Bang/Time.h"
#include "Bang/Input.h"

USING_NAMESPACE_BANG

Path GraphicPipelineDebugger::c_debugDir = Path("/home/sephirot47/Bang/tmp");

GraphicPipelineDebugger::GraphicPipelineDebugger()
{

}

void GraphicPipelineDebugger::Reset()
{
    if (Input::GetKeyDown(Input::Key::Q))
    {
        File::Remove(GraphicPipelineDebugger::c_debugDir);
        File::CreateDirectory(GraphicPipelineDebugger::c_debugDir);
    }
}

void GraphicPipelineDebugger::TakeGBufferShot(GBuffer *gbuffer,
                                              GL::Attachment att,
                                              const String &screenshotName)
{
    if (Input::GetKeyDown(Input::Key::Q))
    {
        String fileName = String::ToString( int(Time::GetNow()) ) + "_"
                          + screenshotName;
        Path filepath = GraphicPipelineDebugger::c_debugDir
                            .Append(fileName).AppendExtension("bmp");
        gbuffer->Export(att, filepath, true);
    }
}

void GraphicPipelineDebugger::TakeGBufferShotStencil(GBuffer *gbuffer,
                                                     const String &screenshotName)
{
}
