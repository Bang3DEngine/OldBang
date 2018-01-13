#include "Bang/CodePreprocessor.h"

#include <algorithm>

#include "Bang/Path.h"
#include "Bang/List.h"
#include "Bang/File.h"
#include "Bang/Debug.h"
#include "Bang/Paths.h"
#include "Bang/Array.h"
#include "Bang/String.h"

USING_NAMESPACE_BANG

String CodePreprocessor::GetIncludeString(const String &includeDirective)
{
    Array<String> includeParts = includeDirective.Split<Array>('"', true);
    if (includeParts.Size() < 2)
    {
        includeParts = includeDirective.Split<Array>('<', true);
        if (includeParts.Size() < 2) { return ""; }
    }
    return includeParts[1];
}

Path CodePreprocessor::GetIncludePath(const String &includeDirective,
                                      const List<Path> &includeDirs)
{
    List<Path> includeDirPathCombinations =
            CodePreprocessor::GetIncludePathCombinations(includeDirective, includeDirs);

    // Search if the include file combined with any of the include paths
    // gives us an existing file. Return the first found.
    for (const Path& includePath : includeDirPathCombinations)
    {
        if (includePath.IsFile()) { return includePath; }
    }

    return Path::Empty;
}

List<Path> CodePreprocessor::GetIncludePathCombinations(const String &includeDirective,
                                                        const List<Path> &includeDirs)
{
    String includeStr = CodePreprocessor::GetIncludeString(includeDirective);

    List<Path> combinations;
    for (const Path& includeDirPath : includeDirs)
    {
        Path completeFilepath = includeDirPath.Append(includeStr);
        combinations.PushBack(completeFilepath);
    }

    return combinations;
}

String CodePreprocessor::GetIncludeContents(const String &includeDirective,
                                            const List<Path> &includeDirs)
{
    Path includePath = CodePreprocessor::GetIncludePath(includeDirective,
                                                        includeDirs);
    if (includePath.IsFile()) { return File::GetContents(includePath); }
    return "";
}

void CodePreprocessor::PreprocessCode(String *srcCode,
                                      const List<Path> &includeDirs)
{
    String &outputCode = *srcCode;

    Array<String> linesArray = outputCode.Split<Array>('\n');
    List<String> lines = linesArray.To<List>();

    // Keep track of the user's source line number, to use #line directive
    int originalLineNum = 1;
    for (auto it = lines.Begin(); it != lines.End(); )
    {
        String line = (*it).Trim();
        if (line.BeginsWith("#include"))
        {
            it = lines.Remove(it);
            String includeContent = GetIncludeContents(line, includeDirs);
            CodePreprocessor::PreprocessCode(&includeContent, includeDirs);

            List<String> contentLines = includeContent.Split<List>('\n', false);
            contentLines.PushBack("#line " + String(originalLineNum-1));
            lines.Splice(it,
                         contentLines,
                         contentLines.Begin(),
                         contentLines.End());
            std::advance(it, contentLines.Size());
        }
        else { ++it; }

        ++originalLineNum;
    }

    if (lines.Size() > 0)
    {
        if (lines.Front().BeginsWith("#version")) { lines.Insert(1, "#line 1\n"); }
        else { lines.Insert(0, "#line 0\n"); }

        outputCode = String::Join(lines, "\n");
    }
}

List<String> CodePreprocessor::GetSourceIncludeDirectives(const String &srcCode)
{
    List<String> includes;
    Array<String> lines = srcCode.Split<Array>('\n');
    for (const String &line : lines)
    {
        if (line.Replace(" ", "").BeginsWith("#include"))
        {
            includes.PushBack(line);
        }
    }
    return includes;
}

List<Path> CodePreprocessor::GetSourceIncludePaths(const Path &srcPath,
                                                   const List<Path> &includeDirs,
                                                   bool onlyExisting)
{
    return CodePreprocessor::GetSourceIncludePaths(File::GetContents(srcPath),
                                                   includeDirs,
                                                   onlyExisting);
}

List<Path> CodePreprocessor::GetSourceIncludePaths(const String &srcCode,
                                                   const List<Path> &includeDirs,
                                                   bool onlyExisting)
{
    List<Path> sourceIncludePaths;

    List<String> includeDirectives =
                CodePreprocessor::GetSourceIncludeDirectives(srcCode);
    for (const String &includeDirective : includeDirectives)
    {
        Path incPath;
        if (!onlyExisting)
        {
            CodePreprocessor::GetIncludePath(includeDirective, includeDirs);
            if (incPath.IsFile()) { sourceIncludePaths.PushBack(incPath); }
        }
        else
        {
            List<Path> combinations =
                 CodePreprocessor::GetIncludePathCombinations(includeDirective,
                                                              includeDirs);
            sourceIncludePaths.PushBack(combinations);
        }
    }
    return sourceIncludePaths;
}


CodePreprocessor::CodePreprocessor()
{
}
