#ifndef UIFILELIST_H
#define UIFILELIST_H

#include "Bang/Path.h"
#include "Bang/GameObject.h"
#include "Bang/Component.h"

NAMESPACE_BANG_BEGIN

FORWARD class UITextRenderer;
FORWARD class UIImageRenderer;

class UIFileList : public Component
{
    COMPONENT(UIFileList);

public:
    UIFileList();
    virtual ~UIFileList();

    void OnStart() override;
    void OnUpdate() override;

    using PathCallback = std::function<void(const Path&)>;

    void SetFileExtensions(const List<String> &extensions);
    void SetCurrentPath(const Path &currentPath);
    void SetFileAcceptedCallback(PathCallback callback);
    void SetPathChangedCallback(PathCallback callback);

    const Path& GetCurrentPath() const;
    const List<String> &GetFileExtensions() const;

private:
    Path m_currentPath;
    List<String> m_fileExtensions;
    PathCallback m_fileAcceptedCallback;
    PathCallback m_pathChangedCallback;

    void FilterPathsByExtension(List<Path> *paths) const;
};

class UIFileListEntry : public GameObject
{
public:
    UIFileListEntry();
    virtual ~UIFileListEntry();

    void OnMouseOver();
    void OnMouseOut();
    void OnSelectionIn();
    void OnSelectionOut();

    void SetPath(const Path &path);
    const Path& GetPath();

private:
    bool m_isSelected = false;
    Path m_path = Path::Empty;
    bool m_isMouseOver = false;
    UIImageRenderer *m_bg = nullptr;
    UITextRenderer *m_text = nullptr;

    void UpdateColor();
};

NAMESPACE_BANG_END

#endif // UIFILELIST_H
