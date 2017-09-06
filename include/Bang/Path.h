#ifndef PATH_H
#define PATH_H

#include "Bang/List.h"
#include "Bang/Flags.h"
#include "Bang/String.h"
#include "Bang/IToString.h"

NAMESPACE_BANG_BEGIN

class Path : public IToString
{
public:
    enum FindFlag
    {
        None      = 0,
        Recursive = 1,
        Hidden    = 2,

        Simple          = FindFlag::None,
        SimpleHidden    = FindFlag::Simple | FindFlag::Hidden,
        RecursiveHidden = FindFlag::Recursive | FindFlag::Hidden,
        Default         = FindFlag::SimpleHidden
    };
    CREATE_FLAGS(FindFlags, FindFlag);

    Path();
    Path(const Path &path);
    explicit Path(const String &absolutePath);
    void SetPath(const String &path);

    bool IsDir() const;
    bool IsFile() const;
    bool Exists() const;

    List<Path> FindFiles(FindFlags findFlags = FindFlag::Default,
                         const List<String> &extensions = {}) const;
    List<Path> FindSubDirectories(FindFlags findFlags) const;
    List<Path> FindSubPaths(FindFlags findFlags) const;

    Path GetDirectory() const;
    String GetName() const;
    String GetNameExt() const;
    String GetExtension() const;
    Array<String> GetExtensions() const;
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

    bool IsHiddenFile() const;

    Path WithHidden(bool hidden) const;
    Path WithNameExt(const String& name, const String& extension = "") const;
    Path WithExtension(const String& extension) const;

    // "*.jpg *.so.*"
    bool HasExtension(const String &extension) const;
    bool HasExtension(const List<String> &extensions) const;

    explicit operator String() const;
    bool operator!=(const Path &rhs) const;
    bool operator==(const Path &rhs) const;
    bool operator<(const Path &rhs) const;

    static const Path Empty;

private:
    String m_absolutePath = "";

    static Path GetNextDuplicatePath(const Path &path);
};

NAMESPACE_BANG_END

#endif // PATH_H
