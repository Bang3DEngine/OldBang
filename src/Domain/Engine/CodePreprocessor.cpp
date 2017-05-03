#include "Bang/CodePreprocessor.h"

#include <algorithm>

#include "Bang/File.h"
#include "Bang/Debug.h"
#include "Bang/IO.h"

String CodePreprocessor::
       GetIncludeReplacementString(const String &includeDirective,
                                   int includeLine,
                                   const List<Path> &includePaths)
{
    Array<String> includeParts = includeDirective.Split('"', true);
    if (includeParts.Size() < 2) { return ""; }

    Path includeFile(includeParts[1]);

    // Search if the include file combined with any of the include paths
    // gives us an existing file. Both in Engine and in Users code
    Path completeFilepath;
    for (const Path& includeDirPath : includePaths)
    {
        completeFilepath = includeDirPath.Append(includeFile);
        if (completeFilepath.IsFile()) { break; }
    }

    if (completeFilepath.IsFile())
    {
        String includeContents = File::GetContents(completeFilepath);

        // Using #line, error lines info will be reported as in the original
        // file, for a more user-friendly debugging
        if (includeLine > 0)
        {
            includeContents += "\n";
            includeContents += "#line " + String::ToString(includeLine - 1);
            includeContents += "\n";
        }

        return includeContents;
    }
    else
    {
        // Debug_Error("Include file '" << includeFile << "' could not be found.");
    }

    return "";
}

void CodePreprocessor::PreprocessCode(String *srcCode,
                                      const List<Path> &includePaths)
{
    String &code = *srcCode;

    Array<String> linesArray = code.Split('\n');
    List<String> lines = linesArray.To<List>();

    // Keep track of the user's source line number, to use #line directive
    int originalLineNum = 1;

    for (auto it = lines.Begin(); it != lines.End(); )
    {
        String line = (*it).Trim();
        if (line.BeginsWith("#include"))
        {
			lines.Remove(it++);

            String content = GetIncludeReplacementString(line, originalLineNum,
                                                         includePaths);

            // Get the include content lines, and add it to the overall lines.
            // This way we can process it recursively, since the included lines
            // are processed too
            List<String> contentLines = content.Split('\n').To<List>();
            originalLineNum -= contentLines.Size(); // Do this before the splice

            lines.Splice(it, contentLines, contentLines.Begin(),
                         contentLines.End());
            it = lines.Begin();
        }
        else { ++it; }

        ++originalLineNum;
    }

    code = String::Join(lines, "\n"); // Update the code adding all the lines
}


CodePreprocessor::CodePreprocessor()
{
}
