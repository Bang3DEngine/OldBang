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
    SystemProcess process;
    process.Start(command, argsList);
    process.WaitUntilFinished();

    String out = process.ReadStandardOutput() + process.ReadStandardError();
    if (output) { *output = out; }
    if (success) { *success = process.FinishedOk(); }
    process.Close();
}
