#ifndef FILEINSPECTABLE_H
#define FILEINSPECTABLE_H

#include "Bang/File.h"
#include "Bang/Inspectable.h"

template <class FileClass>
class FileInspectable : public Inspectable<FileClass>
{
    ICLONEABLE(FileInspectable)
    typedef Inspectable<FileClass> Base;

public:
    FileInspectable() {}
    FileInspectable(const FileClass& file) : m_file(file)
    {
        Base::SetRelatedSerializableObject(&m_file);
    }

    void CloneInto(ICloneable *clone) const override
    {
        IInspectable::CloneInto(clone);
        FileInspectable *finsp = Object::SCast<FileInspectable>(clone);
        finsp->m_file = m_file;
    }

private:
    FileClass m_file;
};

#endif // FILEINSPECTABLE_H
