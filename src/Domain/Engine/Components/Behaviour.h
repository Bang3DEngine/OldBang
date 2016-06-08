#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "Bang.h"
#include "Component.h"

class Behaviour : public Component
{
private:
    std::string filepath = "";

public:
    Behaviour();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override { return "Behaviour"; }

    #ifdef BANG_EDITOR
        static void System(const std::string &command, std::string &output, bool &success);
        virtual InspectorWidgetInfo* GetComponentInfo() override;
        virtual void OnSlotValueChanged(InspectorWidget *source) override;
        void Write(std::ostream &f) const override;
        void Compile(const std::string &filepathFromProjectRoot) const;
        void Link() const;
        void Read(std::istream &f) override;
    #endif

    void SetFilepath(const std::string &filepath);
    std::string GetFilepath() const;
};

#endif
