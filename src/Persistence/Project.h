#ifndef PROJECT_H
#define PROJECT_H

#include "String.h"
#include "IFileable.h"

class Project : public IFileable
{
public:
    Project();
    virtual ~Project();

    const static String GetFileExtensionStatic();
    const virtual String GetFileExtension();

    virtual void ReadXMLInfo(const XMLNode *xmlInfo) override;
    virtual void FillXMLInfo(XMLNode *xmlInfo) const override;

    const String& GetProjectRootFilepath() const;
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
