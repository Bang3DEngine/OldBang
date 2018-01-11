#include "Bang/UIFileList.h"

#include "Bang/Paths.h"
#include "Bang/Input.h"
#include "Bang/UIList.h"
#include "Bang/Alignment.h"
#include "Bang/GameObject.h"
#include "Bang/UIFocusable.h"
#include "Bang/RectTransform.h"
#include "Bang/UITextRenderer.h"
#include "Bang/UIImageRenderer.h"
#include "Bang/UILayoutElement.h"
#include "Bang/UIVerticalLayout.h"
#include "Bang/GameObjectFactory.h"

USING_NAMESPACE_BANG

UIFileList::UIFileList()
{
}

UIFileList::~UIFileList()
{
}

void UIFileList::OnStart()
{
    Component::OnStart();
    SetCurrentPath( GetCurrentPath() );
}

void UIFileList::OnUpdate()
{
    Component::OnUpdate();
}

void UIFileList::SetFileExtensions(const Array<String> &extensions)
{
    m_fileExtensions = extensions;
    UpdateEntries();
}

void UIFileList::AddFileAcceptedCallback(UIFileList::PathCallback callback)
{
    m_fileAcceptedCallback.PushBack(callback);
}

void UIFileList::AddPathChangedCallback(UIFileList::PathCallback callback)
{
    m_pathChangedCallback.PushBack(callback);
}

void UIFileList::SetShowOnlyDirectories(bool showOnlyDirectories)
{
    if (m_showOnlyDirectories != showOnlyDirectories)
    {
        m_showOnlyDirectories = showOnlyDirectories;
        UpdateEntries();
    }
}

Path UIFileList::GetCurrentSelectedPath() const
{
    if (p_selectedItem) { return p_selectedItem->GetPath(); }
    return GetCurrentPath();
}

void UIFileList::SetCurrentPath(const Path &currentPath)
{
    if (m_currentPath != currentPath)
    {
        m_currentPath = currentPath;
        for (auto cb : m_pathChangedCallback)  { cb( GetCurrentPath() ); }
        UpdateEntries();
    }
}

const Path &UIFileList::GetCurrentPath() const
{
    return m_currentPath;
}

bool UIFileList::GetShowOnlyDirectories() const
{
    return m_showOnlyDirectories;
}

const Array<String> &UIFileList::GetFileExtensions() const
{
    return m_fileExtensions;
}

void UIFileList::UpdateEntries()
{
    List<Path> paths = GetCurrentPath().GetSubPaths(Path::FindFlag::Simple);

    if (!GetFileExtensions().IsEmpty())
    {
        UIFileList::FilterPathsByExtension(&paths, GetFileExtensions());
    }
    UIFileList::SortPathsByName(&paths);
    if (GetShowOnlyDirectories()) { UIFileList::RemoveFilesFromList(&paths); }
    paths.PushFront( Path("..") );

    UIList *uiList = GetGameObject()->GetComponent<UIList>();
    uiList->Clear();

    for (const Path &path : paths)
    {
        UIFileListItem *item = GameObject::Create<UIFileListItem>();
        item->SetPath(path);

        uiList->AddItem(item);
    }
    uiList->SetSelection(1);

    uiList->SetSelectionCallback(
        [this, uiList](GameObject *go, UIList::Action action)
        {
            UIFileListItem *item = Cast<UIFileListItem*>(go);
            if (action == UIList::Action::SelectionIn)
            {
                p_selectedItem = item;
            }
            else if (action == UIList::Action::SelectionOut)
            {
                p_selectedItem = nullptr;
            }
            else if (action == UIList::Action::Pressed ||
                     action == UIList::Action::DoubleClickedLeft)
            {
                Path itemPath = item->GetPath();
                if (itemPath.GetAbsolute() == "..")
                {
                    this->SetCurrentPath(GetCurrentPath().GetDirectory());
                }
                else if (itemPath.IsDir())
                {
                    this->SetCurrentPath(itemPath);
                }
                else if (itemPath.IsFile())
                {
                    this->SetCurrentPath(itemPath);
                    for (auto cb : m_fileAcceptedCallback)  { cb(itemPath); }
                }
            }
        }
    );
}

void UIFileList::SortPathsByName(List<Path> *paths)
{
    Array<Path> pathsArr;
    pathsArr.PushBack(paths->Begin(), paths->End());
    std::sort(pathsArr.Begin(), pathsArr.End(),
              [](const Path &lhs, const Path &rhs)
              {
                  return lhs.GetNameExt() < rhs.GetNameExt();
              }
    );

    paths->Clear();
    paths->Insert(paths->End(), pathsArr.Begin(), pathsArr.End());
}

void UIFileList::FilterPathsByExtension(List<Path> *paths,
                                        const Array<String>& extensions)
{
    for (auto it = paths->Begin(); it != paths->End(); )
    {
        const Path &p = *it;
        if ( p.IsFile() && !p.HasExtension(extensions) )
        {
            it = paths->Remove(it);
        }
        else { ++it; }
    }
}

void UIFileList::RemoveFilesFromList(List<Path> *paths)
{
    for (auto it = paths->Begin(); it != paths->End(); )
    {
        const Path &p = *it;
        if (p.IsFile()) { it = paths->Remove(it); }
        else { ++it; }
    }
}

// UIFileListItem
UIFileListItem::UIFileListItem()
{
    GameObjectFactory::CreateUIGameObjectInto(this);
    AddComponent<UIFocusable>();

    UIVerticalLayout *vl = AddComponent<UIVerticalLayout>();
    vl->SetPaddings(5);

    GameObject *container = GameObjectFactory::CreateUIGameObject();
    m_text = container->AddComponent<UITextRenderer>();
    m_text->SetTextSize(12);
    m_text->SetHorizontalAlign(HorizontalAlignment::Left);

    container->SetParent(this);
}

UIFileListItem::~UIFileListItem()
{

}

void UIFileListItem::SetPath(const Path &path)
{
    if (path != GetPath())
    {
        m_path = path;
        m_text->SetContent( (path.IsFile() ? "File - " : "Dir  - ") +
                             path.GetNameExt() );
    }
}

const Path &UIFileListItem::GetPath()
{
    return m_path;
}
