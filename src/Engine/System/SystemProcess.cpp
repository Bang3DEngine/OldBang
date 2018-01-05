#include "Bang/SystemProcess.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <chrono>

#include "Bang/Thread.h"

USING_NAMESPACE_BANG

enum { IN = 0, OUT = 1, ERR = 2 };
enum { WRITE = OUT, READ = IN };

SystemProcess::SystemProcess()
{

}

SystemProcess::~SystemProcess()
{
}

#include "Bang/Debug.h"
bool SystemProcess::Start(const String &command, const List<String> &extraArgs)
{
    m_oldFileDescriptors[IN]  = dup(Channel::StandardIn);
    m_oldFileDescriptors[OUT] = dup(Channel::StandardOut);
    m_oldFileDescriptors[ERR] = dup(Channel::StandardError);

    if (pipe(m_childToParentOutFD) != 0 ||
        pipe(m_childToParentErrFD) != 0 ||
        pipe(m_parentToChildFD) != 0)  { m_exitCode = -1; return false; }

    Debug_Peek(command);
    Debug_Peek(extraArgs);

    int pid = fork();
    if (pid == 0) // Child process
    {
        // Set up parent to child(this) input
        // Input from stdin now will go to our pipe m_parentToChildFD[READ]
        close(m_parentToChildFD[WRITE]);
        dup2(m_parentToChildFD[READ],  Channel::StandardIn);

        // Set up child(this) to parent output && err
        // stdout/stderr now will go to our pipe m_childToParentXXXFD[WRITE]
        close(m_childToParentOutFD[READ]);
        dup2(m_childToParentOutFD[WRITE], Channel::StandardOut);

        close(m_childToParentErrFD[READ]);
        dup2(m_childToParentErrFD[WRITE], Channel::StandardError);

        // Execute the command, and its in/out will come to our pipes
        String fullCommand = command + " " + String::Join(extraArgs, " ");
        int result = system(fullCommand.ToCString()); // EXECUTE!

        // Close channels
        close(m_childToParentOutFD[WRITE]);
        close(m_childToParentErrFD[WRITE]);
        close(m_parentToChildFD[READ]);
        close(Channel::StandardIn);
        close(Channel::StandardOut);
        close(Channel::StandardError);

        if (result < 0) { exit(-1); }
        exit( WEXITSTATUS(result) );
    }
    else if (pid != -1) // Parent process
    {
        m_childPID = pid;
        close(m_childToParentOutFD[WRITE]); // We won't write this
        close(m_childToParentErrFD[WRITE]); // We won't write this
        close(m_parentToChildFD[READ]);  // We won't read this

        // Specify NonBlocking read
        fcntl(m_childToParentOutFD[READ], F_SETFL,
              fcntl(m_childToParentOutFD[READ], F_GETFL) | O_NONBLOCK);
        fcntl(m_childToParentErrFD[READ], F_SETFL,
              fcntl(m_childToParentErrFD[READ], F_GETFL) | O_NONBLOCK);
    }
    else
    {
        std::cerr << "There was an error forking to execute SystemProcess." <<
                     std::endl;
        return false;
    }

    return true;
}

bool SystemProcess::StartDettached(const String &command,
                                   const List<String> &extraArgs)
{
    List<String> args = extraArgs;
    args.PushBack("&");
    return Start(command, args);
}

long long unsigned int GetNow()
{
    return std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()).count();
}

bool SystemProcess::WaitUntilFinished(float seconds)
{
    auto beginning = GetNow();

    // Get its return value
    int status;
    bool exited = false;
    bool signaled = false;
    bool finished = false;
    while ( (GetNow() - beginning) / 1000.0f < seconds )
    {
        status = -1;
        waitpid(m_childPID, &status, WNOHANG);
        exited = WIFEXITED(status);
        signaled = WIFSIGNALED(status);
        if (status >= 0 && (exited || signaled))
        {
            finished = true;
            break;
        }
        Thread::SleepCurrentThread(0.05f);
    }

    if (finished)
    {
        if (status >= 0)
        {
            if (exited) { m_exitCode = WEXITSTATUS(status); }
            if (signaled) { m_exitCode = -2; }
        }
        else { m_exitCode = -1; }
    }
    return finished;
}

void SystemProcess::Close()
{
    // Close channels
    close(m_childToParentOutFD[READ]);
    close(m_childToParentOutFD[WRITE]);
    close(m_childToParentErrFD[READ]);
    close(m_childToParentErrFD[WRITE]);
    close(m_parentToChildFD[READ]);
    close(m_parentToChildFD[WRITE]);

    // Restore stdin / stdout / stderr
    dup2(Channel::StandardIn,    m_oldFileDescriptors[IN]);
    dup2(Channel::StandardOut,   m_oldFileDescriptors[OUT]);
    dup2(Channel::StandardError, m_oldFileDescriptors[ERR]);
    close(m_oldFileDescriptors[IN]);
    close(m_oldFileDescriptors[OUT]);
    close(m_oldFileDescriptors[ERR]);
}

void SystemProcess::Write(const String &str)
{
    write(m_parentToChildFD[WRITE],
          (str).ToCString(),
          sizeof(char) * (str.Size() + 1));
}

void SystemProcess::CloseWriteChannel()
{
    close(m_parentToChildFD[WRITE]);
}

String SystemProcess::ReadStandardOutput()
{
    return ReadFileDescriptor(m_childToParentOutFD[READ]);
}

String SystemProcess::ReadStandardError()
{
    return ReadFileDescriptor(m_childToParentErrFD[READ]);
}

String SystemProcess::ReadFileDescriptor(FileDescriptor fd)
{
    String output = "";

    constexpr int bufferSize = 4096;
    char buffer[bufferSize];
    memset(buffer, 0, bufferSize);
    int readBytes = 0;
    while ( (readBytes = read(fd, buffer, bufferSize)) >= 0 )
    {
        String readChunk(buffer);
        String::Iterator readChunkEnd = readChunk.Begin();
        std::advance(readChunkEnd, readBytes);
        String readChunkN(readChunk.Begin(), readChunkEnd);
        output += readChunkN;
        memset(buffer, 0, bufferSize);
        if (readBytes == 0) { break ; }
    }

    return output;
}

int SystemProcess::GetExitCode() const
{
    return m_exitCode;
}

bool SystemProcess::FinishedOk() const
{
    return GetExitCode() == 0;
}
