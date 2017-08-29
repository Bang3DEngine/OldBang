#ifndef PROJECT_H
#define PROJECT_H

#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/SerializableObject.h"

class Project : public SerializableObject
{
    SERIALIZABLE_OBJECT(Project)

public:
    Project();
    virtual ~Project();

    const Path& GetProjectDirPath() const;
    Path GetProjectAssetsRootFilepath() const;
    Path GetProjectFileFilepath() const;
    String GetProjectName() const;
    String GetProjectRandomId() const;

    void SetProjectRootFilepath(const Path &projectDir);

    virtual bool OpenFirstFoundScene() const;

    // SerializableObject
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    String m_randomId = "";
    Path m_projectRootFilepath;
};

#endif // PROJECT_H
