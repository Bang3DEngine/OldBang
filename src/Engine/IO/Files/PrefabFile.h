#ifndef PREFABFILE_H
#define PREFABFILE_H

#include <QPixmap>

#include "Bang/Path.h"
#include "Bang/BFile.h"
#include "Bang/XMLNode.h"

class GameObject;
class PrefabFile : public BFile
{
public:
    PrefabFile();
    PrefabFile(const Path &path);

    virtual ~PrefabFile();

private:
    GameObject *m_tmpGameObject = nullptr;
};

#endif // PREFABFILE_H
