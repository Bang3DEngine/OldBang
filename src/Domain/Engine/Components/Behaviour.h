#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dlfcn.h>
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
    void *library = nullptr;

public:
    Behaviour();
    virtual ~Behaviour();

    virtual const std::string ToString() const override;
    virtual std::string GetName() const override { return "Behaviour"; }

    #ifdef BANG_EDITOR

        virtual InspectorWidgetInfo* GetComponentInfo() override;
        virtual void OnSlotValueChanged(InspectorWidget *source) override;

        /**
         * @brief Executes a system command.
         * @param The command to be executed.
         * @param The output of the command.
         * @param Whether it's been successful or not
         */
        static void System(const std::string &command, std::string &output, bool &success);

        /**
         * @brief Call calls a function of the loaded library (if it exists).
         * @param methodName
         */
        void Call(const std::string &methodName);

        /**
         * @brief Compiles the passed script into a shared library, whom path is
         * returned.
         * @param filepath of the script to compile. Relative to the project root.
         * @return
         */
        std::string CompileToSharedObject(const std::string &filepathFromProjectRoot) const;

        /**
         * @brief Links the specified library and saves it into the internal library pointer,
         * so we can later call functions of it.
         * @param Shared library to link.
         */
        void Link(const std::string &sharedObjectFilepath);

        void Write(std::ostream &f) const override;
        void Read(std::istream &f) override;
    #endif

    void SetFilepath(const std::string &filepath);
    std::string GetFilepath() const;
};

#endif
