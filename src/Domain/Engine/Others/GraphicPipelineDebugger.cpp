#include "Bang/GraphicPipelineDebugger.h"

#include "Bang/Time.h"
#include "Bang/Input.h"
#include "Bang/IO.h"

String GraphicPipelineDebugger::c_debugDir = "/home/sephirot47/Bang/tmp";

GraphicPipelineDebugger::GraphicPipelineDebugger()
{

}

void GraphicPipelineDebugger::Reset()
{
    if (Input::GetKeyDown(Input::Key::Q))
    {
        IO::Remove(GraphicPipelineDebugger::c_debugDir);
        IO::CreateDirectory( Path(GraphicPipelineDebugger::c_debugDir) );
    }
}

void GraphicPipelineDebugger::TakeGBufferShot(GBuffer *gbuffer,
                                              GBuffer::AttachmentId att,
                                              const String &screenshotName)
{
    if (Input::GetKeyDown(Input::Key::Q))
    {
        String fileName = String::ToString( int(Time::GetNow()) ) + "_"
                          + screenshotName;
        String filepath = GraphicPipelineDebugger::c_debugDir + "/"
                          + fileName + ".bmp";
        gbuffer->SaveToImage(att, filepath, true);
    }
}

