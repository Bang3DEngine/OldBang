#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "Bang.h"
#include "Part.h"

class Behaviour : public Part
{
private:
    std::string filepath = "";

public:
    Behaviour();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override { return "Behaviour"; }

    #ifdef BANG_EDITOR
        virtual InspectorWidgetInfo* GetPartInfo() override;
        virtual void OnSlotValueChanged(InspectorWidget *source) override;
    #endif

    void SetFilepath(const std::string &filepath);
    std::string GetFilepath() const;
};

#endif
