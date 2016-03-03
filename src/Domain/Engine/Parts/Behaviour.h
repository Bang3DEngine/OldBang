#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "Bang.h"
#include "Part.h"

class Behaviour : public Part
{
public:
    Behaviour();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override { return "Behaviour"; }

    #ifdef BANG_EDITOR
        virtual InspectorPartInfo* GetInfo() override { return &inspectorPartInfo; }
        virtual void OnInspectorSlotChanged(InspectorPartWidget *partWidget) override {}
    #endif
};

#endif
