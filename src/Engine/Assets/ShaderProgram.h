#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Bang/Asset.h"
#include "Bang/IToString.h"
#include "Bang/G_ShaderProgram.h"

class ShaderProgram : public G_ShaderProgram,
                      public Asset
{
    OBJECT(ShaderProgram)
    ASSET_ICON(ShaderProgram, "Icons/BehaviourIcon.png")

public:
    ShaderProgram();

    static String GetFileExtensionStatic();
    virtual String GetFileExtension() const override;

    String ToString() const override;

    virtual void Read(const XMLNode &xmlInfo) override;
    virtual void Write(XMLNode *xmlInfo) const override;
};

#endif // SHADERPROGRAM_H
