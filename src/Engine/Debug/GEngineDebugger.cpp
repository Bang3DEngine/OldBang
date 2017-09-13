#include "Bang/GEngineDebugger.h"

#include "Bang/Paths.h"
#include "Bang/File.h"
#include "Bang/Time.h"
#include "Bang/Input.h"

USING_NAMESPACE_BANG

Path GEngineDebugger::c_debugDir = Path("/home/sephirot47/Bang/tmp");

GEngineDebugger::GEngineDebugger()
{

}

void GEngineDebugger::Reset()
{
    if (Input::GetKeyDown(Input::Key::Q))
    {
        File::Remove(GEngineDebugger::c_debugDir);
        File::CreateDirectory(GEngineDebugger::c_debugDir);
    }
}

void GEngineDebugger::TakeGBufferShot(GBuffer *gbuffer,
                                              GL::Attachment att,
                                              const String &screenshotName)
{
    if (Input::GetKeyDown(Input::Key::Q))
    {
        String fileName = String::ToString( int(Time::GetNow()) ) + "_"
                          + screenshotName;
        Path filepath = GEngineDebugger::c_debugDir
                            .Append(fileName).AppendExtension("bmp");
        gbuffer->Export(att, filepath, true);
    }
}

void GEngineDebugger::TakeGBufferShotStencil(GBuffer *gbuffer,
                                                     const String &screenshotName)
{
}
