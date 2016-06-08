#include "Behaviour.h"

Behaviour::Behaviour()
{
    #ifdef BANG_EDITOR
    inspectorComponentInfo.SetSlotsInfos(
    {
        new InspectorFileSWInfo( "Script", "cpp" )
    });
    #endif
}

#ifdef BANG_EDITOR
InspectorWidgetInfo* Behaviour::GetComponentInfo()
{
    static_cast<InspectorFileSWInfo*>
            (inspectorComponentInfo.GetSlotInfo(0))->filepath = filepath;

    return &inspectorComponentInfo;
}

void Behaviour::OnSlotValueChanged(InspectorWidget *source)
{
    filepath = source->GetSWFileFilepath("Script");
}

void Behaviour::Write(std::ostream &f) const
{
    /*
    f << "<Behaviour>" << std::endl;
    f << ((void*)this) << std::endl;
    f << "</Behaviour>" << std::endl;
    */
}

void Behaviour::Compile(const std::string &filepathFromProjectRoot) const
{
    //Get all subdirs recursively in a single line, and add -I in front of every path
    std::string cmdGetAllSubDirs = "";
    cmdGetAllSubDirs = "find " +                                    // Find recursively
                        Persistence::GetProjectRootPathAbsolute() + // From project root
                        " -type d " +                               // Only directories
                        " | grep -E -v \"\\.\" " +                  // Not including hidden dirs
                        " | xargs";                                 // Inline

    bool ok = false;
    std::string allSubDirs = "";
    System(cmdGetAllSubDirs, allSubDirs, ok);

    if(!ok)
    {
        Logger_Error(allSubDirs);
        Logger_Error("Error trying to find include directories to compile " << this);
        return;
    }

    //Add -I in front of every path
    std::string includes = "-I" + allSubDirs;
    for(int i = 0; i < includes.length(); ++i)
    {
        if(includes[i] == ' ')
        {
            includes.insert(i+1, "-I");
            i += 2; // Sorry
        }
    }
    //

    std::replace(includes.begin(), includes.end(), '\n', ' ');

    includes += " -I.";

    #ifdef BANG_EDITOR
    //Qt includes
    includes += " -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui";
    includes += " -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4";
    //
    #endif

    // Add objs
    //

    // Gather options
    std::string options = "";
    options += " -O1";
    options += " --std=c++11";
    options += " " + includes;
    options += " -L/usr/lib/x86_64-linux-gnu -L/usr/X11R6/lib64 ";
    options += " -lGLEW -lQtOpenGL -lQtGui -lQtCore -lGL -lpthread ";
    options += " -fPIC"; // Shared linking stuff
    //

    std::string filepath =
            Persistence::ProjectRootRelativeToAbsolute(filepathFromProjectRoot);
    std::string scriptDir = Persistence::GetDir(filepath);
    std::string scriptName = Persistence::GetFileNameWithExtension(filepath);

    // Compile
    std::string cmd = "";
    cmd += "/usr/bin/g++ -shared ";
    cmd += filepath + " " + options + " -o " + scriptDir + "/" + scriptName + ".so";

    std::string output;
    Behaviour::System(cmd, output, ok);

    if (!ok && output != "")
    {
        Logger_Error(output);
    }
}

void Behaviour::Link() const
{

}

void Behaviour::Read(std::istream &f)
{

}

void Behaviour::SetFilepath(const std::string &filepath)
{
    this->filepath = filepath;
}

std::string Behaviour::GetFilepath() const
{
    return filepath;
}

#endif

const std::string Behaviour::ToString() const
{
    std::string file =
            static_cast<InspectorFileSWInfo*>
            (inspectorComponentInfo.GetSlotInfo(0))->filepath;
    return "Behaviour ( " + file + ")";
}

void Behaviour::System(const std::string &command, std::string &output, bool &success)
{
    const int buff_size = 1024 * 1024;
    char buff[buff_size + 1];
    memset((char*)&buff, 0, buff_size + 1);

    const char *cmd = command.c_str();
    char *ret = nullptr;
    std::string str = "";

    int fd[2];
    int old_fd[3];
    int r = pipe(fd);
    success = true;

    old_fd[0] = !STDIN_FILENO;
    old_fd[1] = !STDOUT_FILENO;
    old_fd[2] = !STDERR_FILENO;

    old_fd[0] = dup(STDIN_FILENO);
    old_fd[1] = dup(STDOUT_FILENO);
    old_fd[2] = dup(STDERR_FILENO);

    int pid = fork();
    if(pid == 0)
    {
        close(fd[0]);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        dup2(fd[1], STDOUT_FILENO);
        dup2(fd[1], STDERR_FILENO);

        int r = system(cmd);
        success = (r == 0);

        close (fd[1]);
        exit(1);
    }
    else if(pid == -1)
    {
        success = false;
        exit(1);
    }

    if(pid != 0)
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);

        while (read(fd[0], buff, buff_size))
        {
            str.append(buff);
            memset(buff, 0, buff_size);
        }

        ret = (char*) malloc( str.size() );
        strcpy(ret, (char*) str.c_str());

        waitpid(pid, nullptr, 0);
        close(fd[0]);
    }

    dup2(STDIN_FILENO,  old_fd[0]);
    dup2(STDOUT_FILENO, old_fd[1]);
    dup2(STDERR_FILENO, old_fd[2]);

    output = std::string(ret);
}
