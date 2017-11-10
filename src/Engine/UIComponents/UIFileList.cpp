#include "Bang/UIFileList.h"

#include "Bang/Paths.h"
#include "Bang/Input.h"
#include "Bang/UIList.h"
#include "Bang/Alignment.h"
#include "Bang/GameObject.h"
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

    RectTransform *rt = GetGameObject()->GetComponent<RectTransform>();
    if (rt->IsMouseOver())
    {
        if (Input::GetKeyDownRepeat(Key::Left))
        {
            SetCurrentPath( GetCurrentPath().GetDirectory() );
        }
    }
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

void UIFileList::SetCurrentPath(const Path &currentPath)
{
    m_currentPath = currentPath;
    if (m_pathChangedCallback) { m_pathChangedCallback(m_currentPath); }

    List<Path> paths = GetCurrentPath().FindSubPaths(Path::FindFlag::Simple);
    if (!GetFileExtensions().IsEmpty()) { FilterPathsByExtension(&paths); }
    paths.PushFront( Path("..") );

    UIList *listDriver = GetGameObject()->GetComponent<UIList>();
    listDriver->Clear();

    for (const Path &path : paths)
    {
        UIFileListEntry *entry = ObjectManager::Create<UIFileListEntry>();
        entry->SetPath(path);

        listDriver->AddElement(entry);
    }
    listDriver->SetSelection(1);

    listDriver->SetSelectionCallback(
        [this, listDriver](GameObject *go, UIList::Action action)
        {
            UIFileListEntry *entry = SCAST<UIFileListEntry*>(go);
            if (action == UIList::Action::SelectionIn)
            {
                entry->OnSelectionIn();
            }
            else if (action == UIList::Action::SelectionOut)
            {
                entry->OnSelectionOut();
            }
            else if (action == UIList::Action::MouseOver)
            {
                entry->OnMouseOver();
            }
            else if (action == UIList::Action::MouseOut)
            {
                entry->OnMouseOut();
            }
            else if (action == UIList::Action::Pressed ||
                     action == UIList::Action::DoubleClickedLeft)
            {
                Path entryPath = entry->GetPath();
                if (entryPath.GetAbsolute() == "..")
                {
                    this->SetCurrentPath(GetCurrentPath().GetDirectory());
                }
                else if (entryPath.IsDir())
                {
                    this->SetCurrentPath(entryPath);
                }
                else if (entryPath.IsFile())
                {
                    this->SetCurrentPath(entryPath);
                    if (m_fileAcceptedCallback)
                    { m_fileAcceptedCallback(entryPath); }
                }
            }
        }
    );
}

const Path &UIFileList::GetCurrentPath() const
{
    return m_currentPath;
}

const List<String> &UIFileList::GetFileExtensions() const
{
    return m_fileExtensions;
}

void UIFileList::FilterPathsByExtension(List<Path> *paths) const
{
    for (auto it = paths->Begin(); it != paths->End(); )
    {
        const Path &p = *it;
        if ( p.IsFile() && !p.HasExtension(GetFileExtensions()) )
        {
            it = paths->Remove(it);
        }
        else { ++it; }
    }
}

// UIFileListEntry
UIFileListEntry::UIFileListEntry()
{
    GameObjectFactory::CreateUIGameObjectInto(this);

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

UIFileListEntry::~UIFileListEntry()
{

}

void UIFileListEntry::OnMouseOver()
{
    m_isMouseOver = true;
    UpdateColor();
}

void UIFileListEntry::OnMouseOut()
{
    m_isMouseOver = false;
    UpdateColor();
}

void UIFileListEntry::OnSelectionOut()
{
    m_isSelected = false;
    UpdateColor();
}

void UIFileListEntry::OnSelectionIn()
{
    m_isSelected = true;
    UpdateColor();
}

void UIFileListEntry::SetPath(const Path &path)
{
    m_path = path;
    m_text->SetContent( (path.IsFile() ? "File - " : "Dir  - ") +
                         path.GetNameExt() );
}

const Path &UIFileListEntry::GetPath()
{
    return m_path;
}

void UIFileListEntry::UpdateColor()
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
