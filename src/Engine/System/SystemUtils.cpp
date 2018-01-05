#include "Bang/SystemUtils.h"

#include "Bang/Debug.h"
#include "Bang/Library.h"
#include "Bang/SystemProcess.h"

USING_NAMESPACE_BANG

void SystemUtils::System(const String &command,
                         const List<String> &argsList,
                         String *output,
                         bool *success)
{
    SystemUtils::_System(command, argsList, output, success, false);
}

void SystemUtils::SystemBackground(const String &command,
                                   const List<String> &argsList)
{
    SystemUtils::_System(command, argsList, nullptr, nullptr, true);
}

void SystemUtils::_System(const String &command,
                          const List<String> &argsList,
                          String *output,
                          bool *success,
                          bool background)
{
    SystemProcess process;
    if (!background)
    {
        process.Start(command, argsList);
        process.WaitUntilFinished();

        String out = process.ReadStandardOutput() +
                     process.ReadStandardError();
        if (output) { *output = out; }
        if (success) { *success = process.FinishedOk(); }
        process.Close();
    }
    else
    {
        bool ok = process.StartDettached(command, argsList);
        if (!ok)
        {
            Debug_Error("Could not start background process " << command);
        }
    }
}

void SystemUtils::CloseLibrary(Library *library)
{
    if (!library->UnLoad())
    {
        Debug_Error(library->GetErrorString());
    }
}
