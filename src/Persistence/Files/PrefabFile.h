#ifndef PREFABFILE_H
#define PREFABFILE_H

#include <QPixmap>
#include <QModelIndex>
#include <QFileSystemModel>
#include "Bang/WinUndef.h"

#include "Bang/File.h"
#include "Bang/IWindowEventManagerListener.h"

class GameObject;
class PrefabFile : public File,
                   public IWindowEventManagerListener,
                   public SerializableObject
{
public:
    PrefabFile();
    PrefabFile(const QFileSystemModel *model, const QModelIndex &index);
    PrefabFile(const String &absPath);

    virtual ~PrefabFile();

    virtual const QPixmap& GetIcon() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    void OnInspectorSlotChanged(InspectorWidget *inspectorItem) override;

    #ifdef BANG_EDITOR
    virtual SerializableObject *GetInspectable() const override;
    #endif

private:
    GameObject *m_tmpGameObject = nullptr;
};

#endif // PREFABFILE_H
