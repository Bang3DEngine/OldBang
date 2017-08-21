#ifndef SYSTEMPROCESS_H
#define SYSTEMPROCESS_H

#include <unistd.h>

#include "Bang/List.h"
#include "Bang/String.h"

class SystemProcess
{
public:
    using FileDescriptor = int;

    enum Channel
    {
        StandardIn    = STDIN_FILENO,
        StandardOut   = STDOUT_FILENO,
        StandardError = STDERR_FILENO
    };

    SystemProcess();
    ~SystemProcess();

    bool Start(const String &command,
               const List<String> &extraArgs = {});
    bool StartDettached(const String &command,
                        const List<String> &extraArgs = {});
    bool WaitUntilFinished();
    void Close();

    void Write(const String &str);
    void CloseWriteChannel();

    String ReadStandardOutput();
    String ReadStandardError();
    String ReadFileDescriptor(FileDescriptor fd);

    int GetExitCode();

private:
    int m_exitCode = 0;

    FileDescriptor m_oldFileDescriptors[3];
    FileDescriptor m_childToParentFD[2];
    FileDescriptor m_parentToChildFD[2];
};

#endif // SYSTEMPROCESS_H