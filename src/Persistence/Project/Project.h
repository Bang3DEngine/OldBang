#ifndef PROJECT_H
#define PROJECT_H

#include "String.h"
#include "SerializableObject.h"

class Project : public SerializableObject
{
public:
    Project();
    virtual ~Project();

    static String GetFileExtensionStatic();
    virtual String GetFileExtension();

    virtual void Read(const XMLNode *xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

    const String& GetProjectRootFilepath() const;
    String GetProjectDir() const;
    String GetProjectAssetsRootFilepath() const;
    String GetProjectFileFilepath() const;
    String GetProjectName() const;
    String GetProjectRandomId() const;

    void SetProjectRootFilepath(const String &projectDir);

private:
    String m_randomId = "123456789";
    String m_projectRootFilepath = "";

};

#endif // PROJECT_H
