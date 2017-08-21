#include "Bang/SystemProcess.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

constexpr int IN = 0, OUT = 1, ERR = 2;
constexpr int WRITE = OUT, READ = IN;

SystemProcess::SystemProcess()
{

}

SystemProcess::~SystemProcess()
{
}

bool SystemProcess::Start(const String &command, const List<String> &extraArgs)
{
    m_oldFileDescriptors[IN]  = dup(Channel::StandardIn);
    m_oldFileDescriptors[OUT] = dup(Channel::StandardOut);
    m_oldFileDescriptors[ERR] = dup(Channel::StandardError);

    if (pipe(m_childToParentFD) != 0 ||
        pipe(m_parentToChildFD) != 0)  { m_exitCode = -1; return false; }

    int pid = fork();
    if (pid == 0) // Child process
    {
        close(Channel::StandardIn);
        close(Channel::StandardOut);
        close(Channel::StandardError);

        // Set up parent to child(this) input
        // Input from stdin now will go to our pipe m_parentToChildFD[READ]
        close(m_parentToChildFD[WRITE]);
        dup2(m_parentToChildFD[READ],  Channel::StandardIn);

        // Set up child(this) to parent output
        // Output to stdout/stderr now will go to our
        // pipe m_childToParentFD[WRITE]
        close(m_childToParentFD[READ]);
        dup2(m_childToParentFD[WRITE], Channel::StandardOut);
        // dup2(m_childToParentFD[WRITE], Channel::StandardError);

        // Execute the program, and its in/out will come to our pipes
        String fullCommand = command + " " + String::Join(extraArgs, " ");
        int result = system(fullCommand.ToCString());

        // Close channels
        close(m_parentToChildFD[READ]);
        close(m_childToParentFD[WRITE]);

        if(result > 0) { exit(1); }
        exit(0);
    }
    else if (pid != -1) // Parent process
    {
        close(m_childToParentFD[WRITE]); // We won't write this
        close(m_parentToChildFD[READ]);  // We won't read this
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

bool SystemProcess::WaitUntilFinished()
{
    // Get its return value
    wait(&m_exitCode); // Wait for child process to finish
    m_exitCode = WEXITSTATUS(m_exitCode);
    return m_exitCode == 0;
}

void SystemProcess::Close()
{
    // Close channels
    close(m_childToParentFD[READ]);
    close(m_childToParentFD[WRITE]);
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
    return ReadFileDescriptor(m_childToParentFD[READ]);
}

String SystemProcess::ReadStandardError()
{
    return ReadFileDescriptor(m_childToParentFD[READ]);
}

String SystemProcess::ReadFileDescriptor(FileDescriptor fd)
{
    String output = "";

    constexpr int bufferSize = 1024 * 512;
    char buffer[bufferSize];
    memset(buffer, 0, bufferSize);
    while ( read(fd, buffer, bufferSize) )
    {
        output.Append(buffer);
        memset(buffer, 0, bufferSize);
    }

    return output;
}

int SystemProcess::GetExitCode()
{
    return m_exitCode;
}
