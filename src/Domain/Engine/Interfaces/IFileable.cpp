#include "IFileable.h"

#include "FileWriter.h"
#include "FileReader.h"

void IFileable::Write(std::ostream &f) const
{
    FileWriter::Write("<" + GetTag() + ">", f);
    WriteInternal(f);
    FileWriter::Write("</" + GetTag() + ">", f);
}

void IFileable::Read(std::istream &f)
{
    ReadInternal(f);
    FileReader::ReadNextLine(f); //Consume close tag
}
