#ifndef SHADERPROGRAMASSETFILEINSPECTABLE_H
#define SHADERPROGRAMASSETFILEINSPECTABLE_H

#include "Bang/SoundFile.h"
#include "Bang/SerializableObject.h"
#include "Bang/ShaderProgramAssetFile.h"
#include "Bang/IAttrWidgetButtonListener.h"

class XMLNode;
class ShaderProgramAssetFileInspectable : public SerializableObject
{
public:
    ShaderProgramAssetFileInspectable(const ShaderProgramAssetFile &spFile);

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;

private:
    ShaderProgramAssetFile m_shaderProgramFile;
};

#endif // SHADERPROGRAMASSETFILEINSPECTABLE_H
