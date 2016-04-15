#ifndef READERWITHPOINTERIDS_H
#define READERWITHPOINTERIDS_H

#include "Bang.h"

#include <map>
#include <string>

#include "FileReader.h"

//Abstract class which defines a base classe which handles
//the basic mechanism of registration/load of id associated to a pointer
//(basically, an id pointer is a the id of any object(first line after its opening tag),
// that you can later use in the same XML file to refer to it, you just save to a map the object id related to
// the pointer of this same object you just have created.)

class ReaderWithPointerIds
{

protected:


    //For every id(first line after a tag in the file)
    // it saves a pointer to the corresponding Part, Entity, etc.
    static std::map<std::string, void*> idToPointers;

    ReaderWithPointerIds() {}

public:

    //Since we have Prefab's and Entity's,
    // that use the same method Entity::Read(),
    // we need to have a way to avoid registering
    // the id into the StageReader, when calling
    // Read() from Prefab. This is one way :)
    // (So, in prefab files, all XMLs internal ids will be NoRegisterId)
    static const std::string NoRegisterId;


    static void ClearPointerIds();

    template <class T>
    static T* GetNextPointerAddress(std::istream &f)
    {
        std::string id = FileReader::ReadString(f);
        if(id == "-")
        {
            return nullptr;
        }

        if(idToPointers.find(id) == idToPointers.end()) return nullptr;
        return (T*)(idToPointers[id]);
    }
    static void RegisterNextPointerId(std::istream &f, void *pointer);

};

#endif // READERWITHPOINTERIDS_H
