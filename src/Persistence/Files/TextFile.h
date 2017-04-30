#ifndef TEXTFILE_H
#define TEXTFILE_H

#include "Bang/File.h"

class TextFile : public File
{
public:
    TextFile();
    TextFile(const Path& path);

    String GetContents() const;

    void Read(const XMLNode &xmlInfo) override;
    void Write(XMLNode *xmlInfo) const override;

    #ifdef BANG_EDITOR
    virtual IInspectable *GetNewInspectable() override;
    #endif

    virtual bool IsAsset() const override;

protected:
    String m_contents = "";
};

#endif // TEXTFILE_H
