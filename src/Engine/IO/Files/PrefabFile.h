#ifndef PREFABFILE_H
#define PREFABFILE_H

#include <QPixmap>
#include "Bang/WinUndef.h"

#include "Bang/Path.h"
#include "Bang/BFile.h"
#include "Bang/XMLNode.h"
#include "Bang/IInspectable.h"

#ifdef BANG_EDITOR
#include "Bang/IWindowEventManagerListener.h"
#endif

class GameObject;
class PrefabFile : public BFile
                   #ifdef BANG_EDITOR
                   ,public IWindowEventManagerListener
                   #endif
{
public:
    PrefabFile();
    PrefabFile(const Path &path);

    virtual ~PrefabFile();

    #ifdef BANG_EDITOR
    virtual const QPixmap& GetIcon() const override;
    virtual IInspectable *GetNewInspectable() override;
    #endif

private:
    GameObject *m_tmpGameObject = nullptr;
};

#endif // PREFABFILE_H
