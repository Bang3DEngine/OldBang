#include "ReaderWithPointerIds.h"

std::map<std::string, void*> ReaderWithPointerIds::idToPointers;

void ReaderWithPointerIds::ClearPointerIds()
{
    idToPointers.clear();
}

void ReaderWithPointerIds::RegisterNextPointerId(std::ifstream &f, void *pointer)
{
    idToPointers[ FileReader::ReadString(f) ] = pointer;
}
