#include "Bang/ExplorerContextMenu.h"

#include <QTreeWidgetItem>
#include "Bang/WinUndef.h"

#include "Bang/Paths.h"
#include "Bang/Debug.h"
#include "Bang/Dialog.h"
#include "Bang/Prefab.h"
#include "Bang/MenuBar.h"
#include "Bang/Material.h"
#include "Bang/Explorer.h"
#include "Bang/AudioClip.h"
#include "Bang/Texture2D.h"
#include "Bang/Inspector.h"
#include "Bang/EditorWindow.h"
#include "Bang/AssetsManager.h"
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
    File f = p_explorer->GetSelectedFile();
    MenuBar *mb = MenuBar::GetInstance();

    Path dirPath = Explorer::GetInstance()->GetCurrentDir();
    Path newPath = dirPath.Append(f.GetPath().GetName());
    newPath = newPath.AppendExtension(Texture2D::GetFileExtensionStatic());
    newPath = newPath.GetDuplicate();

    Texture2D *tex = mb->OnCreateTexture2D(newPath);
    tex->LoadFromImage(f.GetPath() );

    AssetsManager::UpdateAsset(tex->GetFilepath(), tex->GetXMLInfo());
    //Inspector::GetInstance()->ShowInspectable(tex, newTexName);

    return tex;
}

Material* ExplorerContextMenu::OnCreateMaterialFromImageClicked()
{
    Texture2D *tex = OnCreateTextureFromImageClicked();
    return OnCreateMaterialFromTextureClicked(tex);
}

AudioClip *ExplorerContextMenu::OnCreateAudioClipFromSound()
{
    File f = p_explorer->GetSelectedFile();
    MenuBar *mb = MenuBar::GetInstance();

    Path dirPath = Explorer::GetInstance()->GetCurrentDir();
    Path newPath = dirPath.Append(f.GetPath().GetName());
    newPath = newPath.AppendExtension(AudioClip::GetFileExtensionStatic());
    newPath = newPath.GetDuplicate();

    AudioClip *audio = mb->OnCreateAudioClip( newPath );
    audio->LoadFromFile( f.GetPath() );
    AssetsManager::UpdateAsset(audio->GetFilepath(), audio->GetXMLInfo());
    //Inspector::GetInstance()->ShowInspectable(audio, newAudioName);

    return audio;
}

Material* ExplorerContextMenu::OnCreateMaterialFromTextureClicked(Texture2D *tex)
{
    Texture2D *fromTexture = tex;
    if (!fromTexture)
    {
        fromTexture = AssetsManager::Load<Texture2D>(
                    p_explorer->GetSelectedFileOrDirPath());
    }
	if (!fromTexture) { return nullptr; }

    File f (fromTexture->GetFilepath());
    MenuBar *mb = MenuBar::GetInstance();


    Path dirPath = Explorer::GetInstance()->GetCurrentDir();
    Path newPath = dirPath.Append(f.GetPath().GetName());
    newPath = newPath.AppendExtension(Material::GetFileExtensionStatic());
    newPath = newPath.GetDuplicate();

    Material *mat = mb->OnCreateMaterial( newPath );
    mat->SetTexture(fromTexture);
    AssetsManager::UpdateAsset(mat->GetFilepath(), mat->GetXMLInfo());
    //Inspector::GetInstance()->ShowInspectable(mat, newMatName);
    return mat;
}

void ExplorerContextMenu::OnDuplicateClicked()
{
    ENSURE(!p_explorer->GetSelectedFileOrDirPath().IsEmpty());

    Path fromPath = p_explorer->GetSelectedFileOrDirPath();
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
