#include "Bang/ExplorerContextMenu.h"

#include <QTreeWidgetItem>
#include "Bang/WinUndef.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Dialog.h"
#include "Bang/Prefab.h"
#include "Bang/Material.h"
#include "Bang/Explorer.h"
#include "Bang/AudioClip.h"
#include "Bang/Texture2D.h"
#include "Bang/Inspector.h"
#include "Bang/EditorWindow.h"
#include "Bang/AssetsManager.h"
#include "Bang/AssetFileCreator.h"
#include "Bang/GameObjectClipboard.h"


ExplorerContextMenu::ExplorerContextMenu(Explorer *explorer) :
    ContextMenu(explorer), p_explorer(explorer)
{
}

void ExplorerContextMenu::OnCustomContextMenuRequested(QPoint point)
{
    QMenu contextMenu(tr("Explorer context menu"), p_explorer);

    QAction       actionDuplicate("Duplicate",     p_explorer);
    QAction          actionDelete("Delete",        p_explorer);
    QAction    actionCreateDir("Create directory", p_explorer);

    connect(&actionDuplicate,SIGNAL(triggered()),
            this, SLOT(OnDuplicateClicked()));
    connect(&actionDelete, SIGNAL(triggered()),
            this, SLOT(OnDeleteClicked()));
    connect(&actionCreateDir, SIGNAL(triggered()),
            this, SLOT(OnCreateDirClicked()));

    QPoint mousePoint = point;//p_explorer->viewport()->mapFromGlobal(point);
    if (p_explorer->indexAt(mousePoint).isValid())
    {
        if (p_explorer->IsSelectedAFile())
        {
            const File &f = p_explorer->GetSelectedFile();
            if (f.IsImageFile())
            {
                QAction *actionCreateTextureFromImage =
                        contextMenu.addAction("Create Texture from Image");
                connect(actionCreateTextureFromImage, SIGNAL(triggered()),
                        this, SLOT(OnCreateTextureFromImageClicked()));

                QAction *actionCreateMaterialFromImage =
                        contextMenu.addAction("Create Material from Image");
                connect(actionCreateMaterialFromImage, SIGNAL(triggered()),
                        this, SLOT(OnCreateMaterialFromImageClicked()));

                contextMenu.addSeparator();
            }
            else if (f.IsTexture2DFile())
            {
                QAction *actionCreateMaterialFromTexture =
                        contextMenu.addAction("Create Material from Texture");
                connect(actionCreateMaterialFromTexture, SIGNAL(triggered()),
                        this, SLOT(OnCreateMaterialFromTextureClicked()));
                contextMenu.addSeparator();
            }
            else if (f.IsSound())
            {
                QAction *actionCreateAudioFromSound =
                        contextMenu.addAction("Create AudioClip from Sound");
                connect(actionCreateAudioFromSound, SIGNAL(triggered()),
                        this, SLOT(OnCreateAudioClipFromSound()));
                contextMenu.addSeparator();
            }

            contextMenu.addAction(&actionDuplicate);
            contextMenu.addAction(&actionDelete);
        }
        else if (p_explorer->IsSelectedADir())
        {
            contextMenu.addAction(&actionDuplicate);
            contextMenu.addAction(&actionDelete);
        }
    }
    else
    {
        QMenu *assetsMenu = EditorWindow::GetInstance()->menuAssets;
        QMenu *createAssetsMenu = new QMenu("Create Asset");
        createAssetsMenu->addActions(assetsMenu->actions());
        contextMenu.addMenu(createAssetsMenu);
        contextMenu.addSeparator();
        contextMenu.addAction(&actionCreateDir);
    }

    contextMenu.exec(p_explorer->viewport()->mapToGlobal(point));
}

Texture2D* ExplorerContextMenu::OnCreateTextureFromImageClicked()
{
    Path selectedPath = p_explorer->GetSelectedPath();
    String fileName = selectedPath.GetName();
    Path texPath = AssetFileCreator::CreateOnExplorer<Texture2D>(fileName);

    Texture2D *tex = AssetsManager::Load<Texture2D>(texPath);
    tex->LoadFromImage(selectedPath);

    AssetsManager::UpdateAsset(tex->GetFilepath(), tex->GetXMLInfo());

    return tex;
}

Material* ExplorerContextMenu::OnCreateMaterialFromImageClicked()
{
    Texture2D *tex = OnCreateTextureFromImageClicked();
    return OnCreateMaterialFromTextureClicked(tex);
}

AudioClip *ExplorerContextMenu::OnCreateAudioClipFromSound()
{
    Path selectedPath = p_explorer->GetSelectedPath();
    String fileName = selectedPath.GetName();
    Path audioClipPath = AssetFileCreator::CreateOnExplorer<AudioClip>(fileName);

    AudioClip *audio = AssetsManager::Load<AudioClip>(audioClipPath);
    audio->LoadFromFile(selectedPath);

    AssetsManager::UpdateAsset(audio->GetFilepath(), audio->GetXMLInfo());

    return audio;
}

Material* ExplorerContextMenu::OnCreateMaterialFromTextureClicked(Texture2D *tex)
{
    Texture2D *fromTex = tex;
    if (!fromTex)
    {
        fromTex = AssetsManager::Load<Texture2D>(p_explorer->GetSelectedPath());
    }
    if (!fromTex) { return nullptr; }

    String fileName = p_explorer->GetSelectedPath().GetName();
    Path matFilepath = AssetFileCreator::CreateOnExplorer<Material>(fileName);

    Material *mat = AssetsManager::Load<Material>(matFilepath);
    mat->SetTexture(fromTex);

    AssetsManager::UpdateAsset(mat->GetFilepath(), mat->GetXMLInfo());

    return mat;
}

void ExplorerContextMenu::OnDuplicateClicked()
{
    ENSURE(!p_explorer->GetSelectedPath().IsEmpty());

    Path fromPath = p_explorer->GetSelectedPath();
    Path toPath = fromPath.GetDuplicate();
    if (fromPath.IsFile())
    {
        File::DuplicateFile(fromPath, toPath);
    }
    else if (fromPath.IsDir())
    {
        File::DuplicateDir(fromPath, toPath);
    }

    p_explorer->SelectPath( Path(toPath) );
}

void ExplorerContextMenu::OnDeleteClicked()
{
    Path path = p_explorer->GetSelectedFile().GetPath();
    ENSURE( path.Exists() );

    String name = path.GetNameExt();
    Dialog::Reply reply = Dialog::GetYesNo(
                "Delete file or directory",
                "Are you sure you want to remove '" + name + "' ? \n" +
                "This can NOT be undone.");

    if (reply == Dialog::Reply::Yes)
    {
        Inspector::GetInstance()->Clear();
        File::Remove(path);
    }
}

void ExplorerContextMenu::OnCreateDirClicked()
{
    Path dirPath = p_explorer->GetCurrentDir().Append("New_Folder");
    dirPath = dirPath.GetDuplicate();
    File::CreateDirectory(dirPath);
    p_explorer->StartRenaming(dirPath);
}
