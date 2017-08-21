#include "Bang/SystemUtils.h"

#include <QProcess>

#include "Bang/Debug.h"
#include "Bang/Library.h"

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
    QStringList argsListQ;
    for (const String &arg : argsList) { argsListQ.push_back(arg.ToQString()); }

    QProcess process;
    process.setReadChannel(QProcess::ProcessChannel::StandardOutput);
    process.start(command.ToQString(), argsListQ);
    if (!background)
    {
        bool ok = process.waitForFinished(999999);
        ok = ok && (process.exitCode() == 0);

        QString out;
        out = QString( process.readAllStandardOutput() ) +
              QString( process.readAllStandardError() );
        if (output) { *output = String(out) ; }
        if (success) { *success = ok; }
        process.close();
    }
    else
    {
        bool ok = process.startDetached(command.ToQString(), argsListQ);
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

