#ifndef FILEINSPECTABLE_H
#define FILEINSPECTABLE_H

#include "Bang/File.h"
#include "Bang/Inspectable.h"

template <class FileClass>
class FileInspectable : public Inspectable<FileClass>
{
    typedef Inspectable<FileClass> Base;

public:
    FileInspectable(const FileClass& file) : m_file(file)
    {
        Base::SetSerializableObject(&m_file);
    }

private:
    FileClass m_file;
};

#endif // FILEINSPECTABLE_H
