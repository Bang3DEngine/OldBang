#ifndef PROJECT_H
#define PROJECT_H

#include "Bang/Path.h"
#include "Bang/String.h"
#include "Bang/SerializableObject.h"

class Project : public SerializableObject
{
public:
    Project();
    virtual ~Project();

    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override ;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    const Path& GetProjectDirPath() const;
    Path GetProjectDir() const;
    Path GetProjectAssetsRootFilepath() const;
    Path GetProjectFileFilepath() const;
    String GetProjectName() const;
    String GetProjectRandomId() const;

    void SetProjectRootFilepath(const Path &projectDir);

private:
    String m_randomId = "";
    Path m_projectRootFilepath;
};

#endif // PROJECT_H
