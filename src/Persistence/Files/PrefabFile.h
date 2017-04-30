#ifndef PREFABFILE_H
#define PREFABFILE_H

#include <QPixmap>
#include "Bang/WinUndef.h"

#include "Bang/File.h"

#ifdef BANG_EDITOR
#include "Bang/IWindowEventManagerListener.h"
#endif

class GameObject;
class PrefabFile : public File
                   #ifdef BANG_EDITOR
                   ,public IWindowEventManagerListener
                   #endif
{
public:
    PrefabFile();
    PrefabFile(const Path &path);

    virtual ~PrefabFile();

    virtual const QPixmap& GetIcon() const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    #endif

private:
    GameObject *m_tmpGameObject = nullptr;
};

#endif // PREFABFILE_H
