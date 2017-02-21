#include "GraphicPipelineDebugger.h"

#include "Time.h"
#include "Input.h"
#include "Persistence.h"

String GraphicPipelineDebugger::c_debugDir = "/home/sephirot47/Bang/tmp";

GraphicPipelineDebugger::GraphicPipelineDebugger()
{

}

void GraphicPipelineDebugger::Reset()
{
    if (Input::GetKeyDown(Input::Key::Q))
    {
        Persistence::Remove(GraphicPipelineDebugger::c_debugDir);
        Persistence::CreateDirectory(GraphicPipelineDebugger::c_debugDir);
    }
}

void GraphicPipelineDebugger::TakeGBufferShot(GBuffer *gbuffer,
                                              GBuffer::Attachment att,
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

