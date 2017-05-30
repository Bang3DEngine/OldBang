#include "BangPreprocessor.h"

#include <QProcess>
#include "Bang/WinUndef.h"

#include "Bang/Debug.h"
#include "Bang/BPStruct.h"
#include "Bang/BPProperty.h"
#include "Bang/SystemUtils.h"

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

const Array<String> BangPreprocessor::PropertyPrefixes =
{
    "BANG_PROPERTY"
};

const Array<String> BangPreprocessor::StructPrefixes =
{
    "BANG_CLASS",
    "BANG_STRUCT"
};

String BangPreprocessor::Preprocess(const String &source)
{
    String src = source;
    BP::RemoveComments(&src);

    String::Iterator it = src.Begin();
    src.ReplaceInSitu("\n", " ");
    while (true)
    {
        // Find Structure/Class annotation
        String::Iterator itStructBegin = BP::Find(it, src.End(),
                                                  BP::StructPrefixes);
        if (itStructBegin == src.End()) { break; }

        String::Iterator _, itStructEnd;
        BP::GetNextScope(itStructBegin,
                         src.End(),
                         &_, &itStructEnd);
        it = itStructEnd;

        bool ok;
        BPStruct bpStruct;
        BPStruct::FromString(itStructBegin, itStructEnd, &bpStruct, &ok);

        Debug_Log(bpStruct);
    }
    return src;
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

    output.ReplaceInSitu("# 1 \"<stdin>\"\n", "");
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
                                    String::Iterator *scopeEnd)
{
    int insideness = 0;
    String::Iterator it = begin;
    for (; it != end; ++it)
    {
        if (*it == '{')
        {
            if (insideness == 0) { *scopeBegin = it; }
            ++insideness;
        }
        else if (*it == '}')
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
        c = *(*it);
        if (*it == end) { break; }
    }
}

void BangPreprocessor::SkipUntilNewBlank(String::Iterator *it,
                                         String::Iterator end)
{
    char c = *(*it);
    while (c != '\n' && c != '\r' && c != '\t' && c != ' ')
    {
        ++(*it);
        c = *(*it);
        if (*it == end) { break; }
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
    BP::SkipUntilNewBlank(wordEnd, end);
}

BangPreprocessor::BangPreprocessor()
{
}
