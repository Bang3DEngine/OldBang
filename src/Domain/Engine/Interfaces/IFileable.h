#ifndef IFILEABLE_H
#define IFILEABLE_H

#include <set>

//Describes something than can be written and read to/from a file
class String;
class XMLNode;
class IFileable
{

public:
    String GetXMLInfoString() const;
    void ReadXMLInfoFromString(const String &xmlInfoString);

    virtual void ReadXMLInfo(const XMLNode *xmlInfo);
    virtual void FillXMLInfo(XMLNode *xmlInfo) const;

    /**
      This Post stuff is basically for the IFileable's that need info
      contained in other xmlNodes in the file.
      For example, a Scene might need to access to its main Camera. If the
      Camera hasnt been read yet, then Scene wont be able to set it.
     */
    virtual void PostReadXMLInfo(const XMLNode *xmlInfo);
    virtual void PostFillXMLInfo(XMLNode *xmlInfo) const;

protected:
    IFileable();
    virtual ~IFileable();
};

#endif // IFILEABLE_H
