#include "ReaderWithPointerIds.h"

const std::string ReaderWithPointerIds::NoRegisterId = "-";

std::map<std::string, void*> ReaderWithPointerIds::idToPointers;

void ReaderWithPointerIds::ClearPointerIds()
{
    idToPointers.clear();
}

void ReaderWithPointerIds::RegisterNextPointerId(std::istream &f, void *pointer)
{
    std::string id = FileReader::ReadString(f);
    if(id != ReaderWithPointerIds::NoRegisterId)
        idToPointers[id] = pointer;
}
