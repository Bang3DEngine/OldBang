#ifndef UIFILELIST_H
#define UIFILELIST_H

#include "Bang/Path.h"
#include "Bang/GameObject.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class UITextRenderer;
FORWARD class UIFileListItem;
FORWARD class UIImageRenderer;

class UIFileList : public Component
{
    COMPONENT(UIFileList)

public:
    UIFileList();
    virtual ~UIFileList();

    void OnStart() override;
    void OnUpdate() override;

    using PathCallback = std::function<void(const Path&)>;

    void SetFileExtensions(const List<String> &extensions);
    void SetCurrentPath(const Path &currentPath);
    void AddFileAcceptedCallback(PathCallback callback);
    void AddPathChangedCallback(PathCallback callback);
    void SetShowOnlyDirectories(bool showOnlyDirectories);

    Path GetCurrentSelectedPath() const;
    const Path& GetCurrentPath() const;
    bool GetShowOnlyDirectories() const;
    const List<String> &GetFileExtensions() const;

private:
    Path m_currentPath;
    List<String> m_fileExtensions;
    UIFileListItem *p_selectedItem;
    List<PathCallback> m_fileAcceptedCallback;
    List<PathCallback> m_pathChangedCallback;
    bool m_showOnlyDirectories = false;

    void UpdateEntries();
    static void SortPathsByName(List<Path> *paths);
    static void FilterPathsByExtension(List<Path> *paths,
                                       const List<String>& extensions);
    static void RemoveFilesFromList(List<Path> *paths);
};

class UIFileListItem : public GameObject
{
    GAMEOBJECT(UIFileListItem);

public:
    void SetPath(const Path &path);
    const Path& GetPath();

protected:
    UIFileListItem();
    virtual ~UIFileListItem();

private:
    Path m_path = Path::Empty;
    UITextRenderer *m_text = nullptr;
};

NAMESPACE_BANG_END

#endif // UIFILELIST_H
