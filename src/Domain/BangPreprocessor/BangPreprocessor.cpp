#include "BangPreprocessor.h"

#include <QProcess>
#include "Bang/WinUndef.h"

#include "Bang/Array.h"
#include "Bang/BPReflectedStruct.h"
#include "Bang/BPReflectedVariable.h"

typedef BangPreprocessor BP;

const Array<String> BangPreprocessor::Modifiers =
{
    "const",
    "constexpr",
    "volatile",
    "static"
};

const Array<String> BangPreprocessor::VarTypes =
{
    "int",
    "float",
    "double",
    "string",
    "String",
    "struct",
    "class"
};

const Array<String> BangPreprocessor::RVariablePrefixes =
{
    "BP_REFLECT_VARIABLE"
};

const Array<String> BangPreprocessor::RStructPrefixes =
{
    "BP_REFLECT_CLASS",
    "BP_REFLECT_STRUCT"
};

const String BangPreprocessor::ReflectDefinitionsDefineName =
        "BP_REFLECT_DEFINITIONS";

void BangPreprocessor::Preprocess(const String &source,
                                  String *_reflectionHeaderSource,
                                  bool *preprocessedSomething)
{
    *preprocessedSomething = false;
    String &reflectionHeaderSource = *_reflectionHeaderSource;
    reflectionHeaderSource += "#include \"Bang/BPReflectedStruct.h\"\n";
    reflectionHeaderSource += "#include \"Bang/BPReflectedVariable.h\"\n\n";

    String src = source;
    BP::RemoveComments(&src);
    String::Iterator it = src.Begin();
    while (true)
    {
        // Find Structure/Class annotation
        String::Iterator itStructBegin = BP::Find(it, src.End(),
                                                  BP::RStructPrefixes);
        if (itStructBegin == src.End()) { break; }

        String::Iterator itStructScopeBegin, itStructScopeEnd;
        BP::GetNextScope(itStructBegin,
                         src.End(),
                         &itStructScopeBegin,
                         &itStructScopeEnd,
                         '{',
                         '}');
        it = itStructScopeEnd;
        if (itStructScopeBegin == src.End()) { break; }

        bool ok;
        BPReflectedStruct reflStruct;
        BPReflectedStruct::FromString(itStructBegin, itStructScopeEnd,
                                      &reflStruct, &ok);

        const String ReflectionInfoVarName = "BP_ReflectionInfo";
        String structInitCode =
                reflStruct.GetInitializationCode(ReflectionInfoVarName);
        String reflectDefineCode =
            "#define " + BP::ReflectDefinitionsDefineName + "_" +
                         reflStruct.GetStructVariableName() + "() \n"
            "private: \n"
            "  static BPReflectedStruct " + ReflectionInfoVarName + "; \n"
            "public: \n"
            "  static const BPReflectedStruct& GetReflectionInfo() \n"
            "  { \n"
            "     static bool inited = false; \n"
            "     if (!inited) \n"
            "     { \n"
            "       inited = true; \n" +
                    structInitCode +
            "       "
            "     } \n"
            "     return " + ReflectionInfoVarName + ";\n"
            "  }\n"
            "private:";
        reflectDefineCode.ReplaceInSitu("\n", "\\\n");
        reflectDefineCode += "\n";

        reflectionHeaderSource += reflectDefineCode;
        *preprocessedSomething = true;
    }
}

void BangPreprocessor::RemoveComments(String *source)
{
    String &src = *source;

    QProcess gCompilerProcess;
    gCompilerProcess.setReadChannel(QProcess::ProcessChannel::StandardOutput);
    gCompilerProcess.start("g++", {"-fpreprocessed", "-E", "-"});

    gCompilerProcess.write( src.ToCString(), src.Length() );
    gCompilerProcess.closeWriteChannel();

    bool ok = gCompilerProcess.waitForFinished(999999);
    ok = ok && (gCompilerProcess.exitCode() == 0);

    String output = String(
                       QString( gCompilerProcess.readAllStandardOutput() )
                    );
    gCompilerProcess.close();

    output.Erase(output.Begin(), output.find('\n')+1); // Erase first line
    *source = output;
}

String::Iterator BangPreprocessor::Find(String::Iterator begin,
                                        String::Iterator end,
                                        const Array<String> &toFindList)
{
    for (const String &toFind : toFindList)
    {
        String::Iterator itFound = std::search(begin, end,
                                               toFind.Begin(), toFind.End());
        if (itFound != end) { return itFound; }
    }
    return end;
}

void BangPreprocessor::GetNextScope(String::Iterator begin,
                                    String::Iterator end,
                                    String::Iterator *scopeBegin,
                                    String::Iterator *scopeEnd,
                                    char openingBrace,
                                    char closingBrace)
{
    int insideness = 0;
    String::Iterator it = begin;
    for (; it != end; ++it)
    {
        if (*it == openingBrace)
        {
            if (insideness == 0) { *scopeBegin = it; }
            ++insideness;
        }
        else if (*it == closingBrace)
        {
            --insideness;
            if (insideness == 0)
            {
                *scopeEnd = it + 1;
                break;
            }
        }
    }

    if (insideness != 0) { *scopeBegin = *scopeEnd = end; }
}

void BangPreprocessor::SkipBlanks(String::Iterator *it,
                                  String::Iterator end)
{
    char c = *(*it);
    while (c == '\n' || c == '\r' || c == '\t' || c == ' ')
    {
        ++(*it);
        if (*it == end) { break; }
        c = *(*it);
    }
}

void BangPreprocessor::SkipUntilNextBlank(String::Iterator *it,
                                          String::Iterator end)
{
    SkipUntilNext(it, end, {"\n", "\r", "\t", " "});
}

void BangPreprocessor::SkipUntilNext(String::Iterator *it,
                                     String::Iterator end,
                                     const Array<String> &particles)
{
    String c( std::string(1, *(*it)) );
    while ( !particles.Contains(c) )
    {
        ++(*it);
        if (*it == end) { break; }
        c = String( std::string(1, *(*it)) );
    }
}

void BangPreprocessor::FindNextWord(String::Iterator begin,
                                    String::Iterator end,
                                    String::Iterator *wordBegin,
                                    String::Iterator *wordEnd)
{
    *wordBegin = begin;
    BP::SkipBlanks(wordBegin, end);

    *wordEnd = (*wordBegin + 1);
    char c = *(*wordEnd);
    while (String::IsLetter(c) || String::IsNumber(c) || c == '_')
    {
        ++(*wordEnd);
        if (*wordEnd == end) { break; }
        c = *(*wordEnd);
    }
}

BangPreprocessor::BangPreprocessor()
{
}
