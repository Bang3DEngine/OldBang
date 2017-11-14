#ifndef PROJECT_H
#define PROJECT_H

#include "Bang/GUID.h"
#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/Serializable.h"

NAMESPACE_BANG_BEGIN

class Project : public Serializable
{
    ICLONEABLE(Project)
    SERIALIZABLE(Project)

public:
    Project();
    virtual ~Project();

    const Path& GetProjectDirPath() const;
    Path GetProjectAssetsRootFilepath() const;
    Path GetProjectFileFilepath() const;
    String GetProjectName() const;
    const GUID& GetProjectRandomId() const;

    void SetProjectRootFilepath(const Path &projectDir);

    virtual bool OpenFirstFoundScene() const;
    Path GetFirstFoundScenePath() const;

    // Serializable
    virtual void ImportXML(const XMLNode &xmlInfo) override;
    virtual void ExportXML(XMLNode *xmlInfo) const override;

private:
    GUID m_id;
    Path m_projectRootFilepath;
};

NAMESPACE_BANG_END

#endif // PROJECT_H
