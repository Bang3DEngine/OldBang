#ifndef PREFABFILE_H
#define PREFABFILE_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/File.h"

#ifdef BANG_EDITOR
#include "Bang/IWindowEventManagerListener.h"
#endif

class GameObject;
class PrefabFile : public File,
                   public SerializableObject
                   #ifdef BANG_EDITOR
                   ,public IWindowEventManagerListener
                   #endif
{
public:
    PrefabFile();
    PrefabFile(const QFileSystemModel *model, const QModelIndex &index);
    PrefabFile(const String &absPath);

    virtual ~PrefabFile();

    virtual const QPixmap& GetIcon() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    #ifdef BANG_EDITOR
    void OnInspectorSlotChanged(InspectorWidget *inspectorItem) override;
    virtual SerializableObject *GetInspectable() const override;
    #endif

private:
    GameObject *m_tmpGameObject = nullptr;
};

#endif // PREFABFILE_H
