#ifndef PATH_H
#define PATH_H

#include <QFileInfo>

#include "Bang/List.h"
#include "Bang/String.h"
#include "Bang/IToString.h"

class Path : public IToString
{
public:
    Path();
    Path(const Path &path);
    explicit Path(const String &absolutePath);
    void SetPath(const String &path);

    bool IsDir() const;
    bool IsFile() const;
    bool Exists() const;

    List<Path> GetFiles(bool recursively = false,
                        const List<String>& extensions = {}) const;
    List<Path> GetSubDirectories(bool recursively = false) const;

    Path GetDirectory() const;
    String GetName() const;
    String GetNameExt() const;
    String GetExtension() const;
    const String& GetAbsolute() const;
    Path GetDuplicate() const;

    virtual String ToString() const override;
    bool IsEmpty() const;

    bool BeginsWith(const Path &path) const;
    bool BeginsWith(const String &path) const;

    Path Append(const Path& path) const;
    Path Append(const String& str) const;
    Path AppendRaw(const String& str) const;
    Path AppendExtension(const String& extension) const;

    Path ChangeExtension(const String& extension) const;

    // "*.jpg *.so.*"
    bool HasExtension(const String &extension) const;
    bool HasExtension(const List<String> &extensions) const;

    explicit operator String() const;
    bool operator!=(const Path &rhs) const;
    bool operator==(const Path &rhs) const;
    bool operator<(const Path &rhs) const;

    static const Path Empty;

private:
    String m_absolutePath   = "";

    static Path GetNextDuplicatePath(const Path &path);

};

#endif // PATH_H
