#ifndef EDITORENTITY_H
#define EDITORENTITY_H

#include "Bang.h"

#include "Entity.h"

class EditorEntity : public Entity
{
public:
    EditorEntity();
    virtual ~EditorEntity();

    #ifdef BANG_EDITOR
    //They are empty, since you can't and shouldn't save or load
    //editor internal instances.
    void Write(std::ostream &f) const override;
    void Read(std::istream &f) override;
    #endif

};

#endif // EDITORENTITY_H
