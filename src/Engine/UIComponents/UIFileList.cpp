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

void UIFileList::SetFileExtensions(const List<String> &extensions)
{
    m_fileExtensions = extensions;
    SetCurrentPath( GetCurrentPath() );
}

void UIFileList::SetFileAcceptedCallback(UIFileList::PathCallback callback)
{
    m_fileAcceptedCallback = callback;
}

void UIFileList::SetPathChangedCallback(UIFileList::PathCallback callback)
{
    m_pathChangedCallback = callback;
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
        if (m_pathChangedCallback) { m_pathChangedCallback( GetCurrentPath() ); }
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

const List<String> &UIFileList::GetFileExtensions() const
{
    return m_fileExtensions;
}

void UIFileList::UpdateEntries()
{
    List<Path> paths = GetCurrentPath().FindSubPaths(Path::FindFlag::Simple);

    if (!GetFileExtensions().IsEmpty())
    {
        UIFileList::FilterPathsByExtension(&paths, GetFileExtensions());
    }
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
                item->OnSelectionIn();
                p_selectedItem = item;
            }
            else if (action == UIList::Action::SelectionOut)
            {
                item->OnSelectionOut();
                p_selectedItem = nullptr;
            }
            else if (action == UIList::Action::MouseOver)
            {
                item->OnMouseOver();
            }
            else if (action == UIList::Action::MouseOut)
            {
                item->OnMouseOut();
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
                    if (m_fileAcceptedCallback)
                    { m_fileAcceptedCallback(itemPath); }
                }
            }
        }
    );
}

void UIFileList::FilterPathsByExtension(List<Path> *paths,
                                        const List<String>& extensions)
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

    m_bg = AddComponent<UIImageRenderer>();
    m_bg->SetTint(Color::Zero);

    GameObject *container = GameObjectFactory::CreateUIGameObject();
    m_text = container->AddComponent<UITextRenderer>();
    m_text->SetTextSize(12);
    m_text->SetHorizontalAlign(HorizontalAlignment::Left);

    SetAsChild(container);
}

UIFileListItem::~UIFileListItem()
{

}

void UIFileListItem::OnMouseOver()
{
    m_isMouseOver = true;
    UpdateColor();
}

void UIFileListItem::OnMouseOut()
{
    m_isMouseOver = false;
    UpdateColor();
}

void UIFileListItem::OnSelectionOut()
{
    m_isSelected = false;
    UpdateColor();
}

void UIFileListItem::OnSelectionIn()
{
    m_isSelected = true;
    UpdateColor();
}

void UIFileListItem::SetPath(const Path &path)
{
    m_path = path;
    m_text->SetContent( (path.IsFile() ? "File - " : "Dir  - ") +
                         path.GetNameExt() );
}

const Path &UIFileListItem::GetPath()
{
    return m_path;
}

void UIFileListItem::UpdateColor()
{
    if (m_isSelected)
    {
        m_bg->SetTint(Color::LightBlue);
    }
    else
    {
        if (m_isMouseOver)
        {
            m_bg->SetTint(Color(0.9f, 0.95f, 1.0f));
        }
        else
        {
            m_bg->SetTint(Color::Zero);
        }
    }
}
